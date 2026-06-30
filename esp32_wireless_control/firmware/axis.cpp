#include "soc/gpio_struct.h"

#include "axis.h"
#include "uart.h"

#if MICROSTEPPING_MOTOR_DRIVER == USE_MSx_PINS_MICROSTEPPING
#include "drivers/msx_motor_driver.h"
MSxMotorDriver ra_driver(RA_MS1, RA_MS2, AXIS1_DIR);
#elif MICROSTEPPING_MOTOR_DRIVER == USE_TMC_DRIVER_MICROSTEPPING
#include "drivers/tmc_motor_driver.h"
TmcMotorDriver ra_driver(&AXIS_SERIAL_PORT, AXIS1_ADDR, TMC_R_SENSE, AXIS_RX, AXIS_TX);
#else
#error Unknown Motor Driver
#endif

Axis ra_axis(1, &ra_driver, AXIS1_DIR, RA_INVERT_DIR_PIN);

volatile bool ra_axis_step_phase = 0;

void IRAM_ATTR stepTimerRA_ISR()
{
    // ra ISR
    ra_axis_step_phase = !ra_axis_step_phase;
    if (ra_axis_step_phase)
    {
#ifdef BOARD_HAS_PIN_REMAP
        digitalWrite(AXIS1_STEP, HIGH);
#else
        GPIO.out_w1ts = (1 << AXIS1_STEP); // Set pin high
#endif
    }
    else
    {
#ifdef BOARD_HAS_PIN_REMAP
        digitalWrite(AXIS1_STEP, LOW);
#else
        GPIO.out_w1tc = (1 << AXIS1_STEP); // Set pin low
#endif
    }

    int64_t position = ra_axis.getPosition();
    uint16_t uStep = ra_axis.getMicrostep();
    if (ra_axis_step_phase)
    {
        if (ra_axis.direction.absolute ^ ra_axis.direction.tracking)
        {
            position -= MAX_MICROSTEPS / (uStep ? uStep : 1);
        }
        else
        {
            position += MAX_MICROSTEPS / (uStep ? uStep : 1);
        }
        ra_axis.setPosition(position);
    }

    if (ra_axis.counterActive && ra_axis_step_phase)
    { // if counter active
        int64_t temp = ra_axis.getAxisCount();
        if (ra_axis.direction.absolute ^ ra_axis.direction.tracking)
        {
            temp--;
        }
        else
        {
            temp++;
        }
        ra_axis.setAxisCount(temp);
        if (ra_axis.goToTarget && ra_axis.getAxisCount() == ra_axis.getAxisTargetCount())
        {
            print_out("axisCountValue: %lld", ra_axis.getAxisCount());
            print_out("targetCount: %lld", ra_axis.getAxisTargetCount());
            ra_axis.goToTarget = false;
            ra_axis.stopSlew();
        }
    }
}

void IRAM_ATTR slewTimeOutTimer_ISR()
{
    ra_axis.stopSlew();
}

HardwareTimer slewTimeOut(2000, &slewTimeOutTimer_ISR);

// Position class implementation
Position::Position(int degrees, int minutes, float seconds)
{
    arcseconds = toArcseconds(degrees, minutes, seconds);
}

float Position::toDegrees() const
{
    return arcseconds / 3600.0f;
}

int64_t Position::toArcseconds(int degrees, int minutes, float seconds)
{
    return (degrees * 3600) + (minutes * 60) + static_cast<int>(seconds);
}

void axisTask(void* parameter)
{
    Axis* axis = (Axis*) parameter;
    for (;;)
    {
        if (axis->trackingRequested())
        {
            axis->startTracking(axis->rate.requested, axis->direction.requested);
        }
        vTaskDelay(1);
    }
}

Axis::Axis(uint8_t axis, MotorDriver* motorDriver, uint8_t dirPinforAxis, bool invertDirPin)
    : stepTimer(TIMER_APB_CLK_FREQ), startRequested(false)
{
    driver = motorDriver;
    axisNumber = axis;
    direction.tracking = c_DIRECTION;
    dirPin = dirPinforAxis;
    invertDirectionPin = invertDirPin;
    rate.tracking = trackingRates.getRate();

    pinMode(dirPin, OUTPUT);

    switch (axisNumber)
    {
        case 1:
            stepTimer.attachInterupt(&stepTimerRA_ISR);
            break;
    }
}

void Axis::begin()
{
    if (xTaskCreatePinnedToCore(axisTask, "axis_task", 4096, this, 1, NULL, 1))
        print_out_nonl("Started axis task\n");
}

void Axis::startTracking(uint64_t rateArg, bool directionArg)
{
    if (!motorActive){
        enableMotorPower();
    }
    startRequested = false;
    rate.tracking = rateArg;
    direction.tracking = directionArg;
    setDirection(directionArg);
    trackingActive = true;
    stepTimer.stop();
    setMicrostep(TRACKER_MOTOR_MICROSTEPPING);
    stepTimer.start(rate.tracking, true);
}

void Axis::stopTracking()
{
    trackingActive = false;
    stepTimer.stop();
}

void Axis::gotoTarget(uint16_t microstep, uint64_t rateArg, const Position& current,
                      const Position& target, bool hemisphereDirection)
{
    setMicrostep(microstep);
    // delta in RA seconds-of-time (NOT angular arcseconds) TODO: investigate
    int64_t deltaRASeconds = target.arcseconds - current.arcseconds;

    print_out_nonl("deltaRASeconds: %lld\n", deltaRASeconds);

    int64_t absDelta = (deltaRASeconds < 0) ? -deltaRASeconds : deltaRASeconds;
    if (absDelta > RA_SECONDS_PER_FULL_REV / 2)
    {
        if (deltaRASeconds > 0)
        {
            deltaRASeconds = (deltaRASeconds - RA_SECONDS_PER_FULL_REV) % RA_SECONDS_PER_FULL_REV;
        }
        else
        {
            deltaRASeconds = (deltaRASeconds + RA_SECONDS_PER_FULL_REV) % RA_SECONDS_PER_FULL_REV;
        }
    }

    int64_t stepsPerRASecondAtMax = STEPS_PER_TRACKER_FULL_REV_INT / RA_SECONDS_PER_FULL_REV;
    int64_t stepsToMoveAtCurrentMicrostep =
        (deltaRASeconds * stepsPerRASecondAtMax * microstep) / MAX_MICROSTEPS;
    int64_t positionDeltaAtMax = deltaRASeconds * stepsPerRASecondAtMax;

    // Calculate motor direction based on hemisphere and movement direction
    // North hemisphere: direction=0 is LEFT (increasing RA), direction=1 is RIGHT (decreasing RA)
    // South hemisphere: direction=1 is LEFT (increasing RA), direction=0 is RIGHT (decreasing RA)
    bool motorDirection = (positionDeltaAtMax < 0) == hemisphereDirection;

    // For goto, set direction.absolute to make counter count in the correct direction
    // The ISR counter uses (direction.absolute XOR direction.tracking)
    // - XOR = 0: counter increments (for positive target)
    // - XOR = 1: counter decrements (for negative target)
    // For positive stepsToMove: want counter to increment, so direction.absolute =
    // direction.tracking For negative stepsToMove: want counter to decrement, so direction.absolute
    // != direction.tracking
    bool positionTrackingDirection =
        (positionDeltaAtMax >= 0) ? direction.tracking : !direction.tracking;

    print_out_nonl("stepsToMove: %lld (at microstep %d), positionDelta: %lld (at MAX)\n",
                   stepsToMoveAtCurrentMicrostep, microstep, positionDeltaAtMax);
    print_out_nonl(
        "hemisphereDirection: %d, motorDirection: %d, positionTrackingDir: %d (should move %s)\n",
        hemisphereDirection, motorDirection, positionTrackingDirection,
        (positionDeltaAtMax >= 0) ? "LEFT/EAST" : "RIGHT/WEST");

    // Set current position (normalized to MAX_MICROSTEPS) and prepare counter for relative movement
    setPosition(current.arcseconds * stepsPerRASecondAtMax);
    resetAxisCount();
    // Use signed target - counter will count up for positive, down for negative
    // Counter tracks actual motor steps at current microstep setting
    setAxisTargetCount(stepsToMoveAtCurrentMicrostep);

    if (targetCount != axisCountValue)
    {
        counterActive = true;
        goToTarget = true;
        stepTimer.stop();

        // Set direction.absolute equal to direction.tracking to make counter increment
        // Set physical motor direction
        // This is a workaround to move the motor in the correct direction while
        // keeping the counter logic consistent
        direction.absolute = positionTrackingDirection;
        driver->setDirection(motorDirection ^ invertDirectionPin);

        slewActive = true;
        stepTimer.start(rateArg, true);
    }
}

void Axis::stopGotoTarget()
{
    goToTarget = false;
    counterActive = false;
    stepTimer.stop();
    slewTimeOut.start(1, true);
}

bool Axis::panByDegrees(float degrees, int speed, uint16_t microstep)
{
    if (slewActive || goToTarget || (degrees == 0.0f))
        return false;

    // For a full 360° rotation, the ISR will count to STEPS_PER_TRACKER_FULL_REV_INT /
    // (MAX_MICROSTEPS / microstep) This is because the ISR increments once per step, regardless of
    // microstepping
    int64_t stepsPerFullRotation =
        STEPS_PER_TRACKER_FULL_REV_INT / (MAX_MICROSTEPS / (microstep ? microstep : 1));

    // Calculate target count for the given degrees
    int64_t stepsToMove = (int64_t) ((std::abs(degrees) / 360.0f) * stepsPerFullRotation + 0.5f);

    // Apply sign based on pan direction
    if (degrees < 0)
        stepsToMove = -stepsToMove;

    // Determine direction based on sign of stepsToMove
    bool directionTmp = (stepsToMove < 0) ^ direction.tracking;

    print_out("Pan: %.2f degrees => %lld ISR steps (microstep %d)", degrees, stepsToMove,
              microstep);
    print_out("stepsPerFullRotation: %lld, STEPS_FULL_REV: %lld", stepsPerFullRotation,
              (int64_t) STEPS_PER_TRACKER_FULL_REV_INT);

    // Set up the goto directly without Position wrapping
    setMicrostep(microstep);
    resetAxisCount();
    setAxisTargetCount(stepsToMove);

    if (stepsToMove != 0)
    {
        counterActive = true;
        goToTarget = true;
        stepTimer.stop();
        setDirection(directionTmp);
        slewActive = true;
        stepTimer.start((2 * rate.tracking) / speed, true);
        print_out("Pan started: counterActive=%d, goToTarget=%d, targetCount=%lld", counterActive,
                  goToTarget, getAxisTargetCount());
    }

    return goToTarget;
}

bool Axis::stopPanByDegrees()
{
    if (slewActive || goToTarget)
    {
        counterActive = false;
        goToTarget = false;
        stopSlew();
        return true;
    }
    return false;
}

void Axis::startSlew(uint64_t rate, bool directionArg)
{
    if (!motorActive){
        enableMotorPower();
    }
    stepTimer.stop();
    setDirection(directionArg);
    slewActive = true;
    setMicrostep(TRACKER_MOTOR_MICROSTEPPING / 2);
    slewTimeOut.start(12000, true);
    stepTimer.start(rate, true);
}

void Axis::stopSlew()
{
    slewActive = false;
    stepTimer.stop();
    slewTimeOut.stop();
    if (trackingActive)
    {
        requestTracking(rate.tracking, direction.tracking);
    }
}

void Axis::setAxisTargetCount(int64_t count)
{
    targetCount = count;
}

int64_t Axis::getAxisTargetCount()
{
    return targetCount;
}

void Axis::resetAxisCount()
{
    axisCountValue = 0;
}

void Axis::setAxisCount(int64_t count)
{
    axisCountValue = count;
}

int64_t Axis::getAxisCount()
{
    return axisCountValue;
}

void Axis::setDirection(bool directionArg)
{
    direction.absolute = directionArg;
    driver->setDirection(directionArg ^ invertDirectionPin);
}

void Axis::setMicrostep(uint16_t microstep)
{
    if (microStep != microstep)
    {
        microStep = microstep;
        driver->setMicrosteps(microstep);
    }
}

void Axis::print_status()
{
    driver->print_status();
}

void Axis::enableMotorPower()
{
    digitalWrite(EN12_n, LOW);
    motorActive = true;
}

void Axis::disableMotorPower()
{
    digitalWrite(EN12_n, HIGH);
    motorActive = false;
}

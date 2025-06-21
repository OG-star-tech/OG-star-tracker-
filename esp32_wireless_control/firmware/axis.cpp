#include "soc/gpio_struct.h"

#include "axis.h"
#include "uart.h"

#include "msx_motor_driver.h"

MSxMotorDriver ra_driver(RA_MS1, RA_MS2);
MSxMotorDriver dec_driver(DEC_MS1, DEC_MS2);

Axis ra_axis(1, &ra_driver, AXIS1_DIR, RA_INVERT_DIR_PIN);
Axis dec_axis(2, &dec_driver, AXIS2_DIR, DEC_INVERT_DIR_PIN);

volatile bool ra_axis_step_phase = 0;
volatile bool dec_axis_step_phase = 0;

void IRAM_ATTR stepTimerRA_ISR()
{
    // ra ISR
    ra_axis_step_phase = !ra_axis_step_phase;
    if (ra_axis_step_phase)
        GPIO.out_w1ts = (1 << AXIS1_STEP); // Set pin high
    else
        GPIO.out_w1tc = (1 << AXIS1_STEP); // Set pin low

    if (ra_axis.counterActive && ra_axis_step_phase)
    { // if counter active
        int temp = ra_axis.getAxisCount();
        ra_axis.axisAbsoluteDirection ? temp++ : temp--;
        ra_axis.setAxisCount(temp);
        if (ra_axis.goToTarget && ra_axis.getAxisCount() == ra_axis.getAxisTargetCount())
        {
            print_out("GotoTarget reached");
            print_out("GotoTarget axisCountValue: %lld", ra_axis.getAxisCount());
            print_out("GotoTarget targetCount: %lld", ra_axis.getAxisTargetCount());
            ra_axis.goToTarget = false;
            ra_axis.stopSlew();
        }
    }
}

void IRAM_ATTR stepTimerDEC_ISR()
{
    // dec ISR
    dec_axis_step_phase = !dec_axis_step_phase;
    if (dec_axis_step_phase)
        GPIO.out_w1ts = (1 << AXIS2_STEP); // Set pin high
    else
        GPIO.out_w1tc = (1 << AXIS2_STEP); // Set pin low

    if (dec_axis_step_phase && dec_axis.counterActive)
    { // if counter active
        int temp = dec_axis.axisCountValue;
        dec_axis.axisAbsoluteDirection ? temp++ : temp--;
        dec_axis.axisCountValue = temp;
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

Axis::Axis(uint8_t axis, MotorDriver* motorDriver, uint8_t dirPinforAxis, bool invertDirPin) :
    stepTimer(40000000)
{
    driver = motorDriver;
    axisNumber = axis;
    trackingDirection = c_DIRECTION;
    dirPin = dirPinforAxis;
    invertDirectionPin = invertDirPin;
    trackingRate = TRACKING_RATE;
    switch (axisNumber)
    {
        case 1:
            stepTimer.attachInterupt(&stepTimerRA_ISR);
            break;
        case 2:
            stepTimer.attachInterupt(&stepTimerDEC_ISR);
            break;
    }

    if (DEFAULT_ENABLE_TRACKING == 1 && axisNumber == 1)
    {
        startTracking(trackingRate, trackingDirection);
    }
}

void Axis::startTracking(trackingRateS rate, bool directionArg)
{
    trackingRate = rate;
    trackingDirection = directionArg;
    axisAbsoluteDirection = directionArg;
    setDirection(axisAbsoluteDirection);
    trackingActive = true;
    stepTimer.stop();
    setMicrostep(64);
    stepTimer.start(trackingRate, true);
}

void Axis::stopTracking()
{
    trackingActive = false;
    stepTimer.stop();
}

void Axis::gotoTarget(uint64_t rate, const Position& current, const Position& target)
{
    int64_t deltaArcseconds = target.arcseconds - current.arcseconds;
    int64_t stepsToMove = deltaArcseconds / ARCSEC_PER_STEP;
    bool direction = stepsToMove > 0;

    resetAxisCount();
    setAxisTargetCount(stepsToMove);

    if (targetCount != axisCountValue)
    {
        counterActive = true;
        goToTarget = true;
        stepTimer.stop();
        axisAbsoluteDirection = direction;
        setDirection(axisAbsoluteDirection);
        slewActive = true;
        setMicrostep(8);
        stepTimer.start(rate, true);
    }
}

void Axis::stopGotoTarget()
{
    goToTarget = false;
    counterActive = false;
    stepTimer.stop();
    slewTimeOut.start(1, true);
}

void Axis::startSlew(uint64_t rate, bool directionArg)
{
    stepTimer.stop();
    axisAbsoluteDirection = directionArg;
    setDirection(axisAbsoluteDirection);
    slewActive = true;
    setMicrostep(8);
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
        startTracking(trackingRate, trackingDirection);
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
    digitalWrite(dirPin, directionArg ^ invertDirectionPin);
}

void Axis::setMicrostep(uint8_t microstep)
{
    driver->setMicrosteps(microstep);
}

#include "soc/gpio_struct.h"

#include "axis.h"
#include "uart.h"
#include "consts.h"


#if MICROSTEPPING_MOTOR_DRIVER == USE_MSx_PINS_MICROSTEPPING
#include "msx_motor_driver.h"
MSxMotorDriver ra_driver(RA_MS1, RA_MS2);
MSxMotorDriver dec_driver(DEC_MS1, DEC_MS2);
#elif MICROSTEPPING_MOTOR_DRIVER == USE_TMC_DRIVER_MICROSTEPPING
#include "tmc_motor_driver.h"
TmcMotorDriver ra_driver(&AXIS_SERIAL_PORT, AXIS1_ADDR, TMC_R_SENSE, AXIS_RX, AXIS_TX);
TmcMotorDriver dec_driver(&AXIS_SERIAL_PORT, AXIS2_ADDR, TMC_R_SENSE, AXIS_RX, AXIS_TX);
#else
#   error Unknown Motor Driver
#endif

Axis ra_axis(1, &ra_driver, AXIS1_DIR, RA_INVERT_DIR_PIN);
Axis dec_axis(2, &dec_driver, AXIS2_DIR, DEC_INVERT_DIR_PIN);

volatile bool ra_axis_step_phase = 0;
volatile bool dec_axis_step_phase = 0;

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
//    	gpio_ll_set_level(&GPIO, AXIS1_STEP, 1);
#endif
    }
    else
    {
#ifdef BOARD_HAS_PIN_REMAP
        digitalWrite(AXIS1_STEP, LOW);
#else
        GPIO.out_w1tc = (1 << AXIS1_STEP); // Set pin low
//    	gpio_ll_set_level(&GPIO, AXIS1_STEP, 0);
#endif
    }

    int64_t position = ra_axis.getPosition();
    uint8_t uStep = ra_axis.getMicrostep();
    if(ra_axis_step_phase)
    {
		if(ra_axis.axisAbsoluteDirection)
		{
			position += MAX_MICROSTEPS/(uStep ? uStep : 1);
		}
		else
		{
			position -= MAX_MICROSTEPS/(uStep ? uStep : 1);
		}
		ra_axis.setPosition(position);
    }

    if (ra_axis.counterActive && ra_axis_step_phase)
    { // if counter active
        int temp = ra_axis.getAxisCount();
        if(ra_axis.axisAbsoluteDirection)
        {
            temp++;
        }
		else
		{
			temp--;
		}
        ra_axis.setAxisCount(temp);
        if (ra_axis.goToTarget && ra_axis.getAxisCount() == ra_axis.getAxisTargetCount())
        {
            print_out("GotoTarget reached");
            print_out("GotoTarget axisCountValue: %lld", ra_axis.getAxisCount());
            print_out("GotoTarget targetCount: %lld", ra_axis.getAxisTargetCount());
            ra_axis.goToTarget = false;
            ra_axis.slewActive = false;
            ra_axis.stepTimer.stop();
//            ra_axis.stopSlew();
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
        int temp = dec_axis.getAxisCount();
        int64_t position = dec_axis.getPosition();
        uint8_t uStep = dec_axis.getMicrostep();
        if(dec_axis.axisAbsoluteDirection)
        {
        	temp++;
        	position += MAX_MICROSTEPS/(uStep ? uStep : 1);
        }
		else
		{
			temp--;
        	position -= MAX_MICROSTEPS/(uStep ? uStep : 1);
		}
        dec_axis.setAxisCount(temp);
        dec_axis.setPosition(position);
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

    pinMode(dirPin, OUTPUT);

    switch (axisNumber)
    {
        case 1:
            stepTimer.attachInterupt(&stepTimerRA_ISR);
            break;
        case 2:
            stepTimer.attachInterupt(&stepTimerDEC_ISR);
            break;
    }

//    if (DEFAULT_ENABLE_TRACKING == 1 && axisNumber == 1)
//    {
//        startTracking(trackingRate, trackingDirection);
//    }
}

void Axis::startTracking(trackingRateS rate, bool directionArg)
{
    trackingRate = rate;
    trackingDirection = directionArg;
    setDirection(directionArg);
    trackingActive = true;
    stepTimer.stop();
    setMicrostep(TRACKER_MOTOR_MICROSTEPPING);
    stepTimer.start(trackingRate, true);
}

void Axis::stopTracking()
{
    trackingActive = false;
    stepTimer.stop();
}

void Axis::gotoTarget(uint64_t rate, const Position& current, const Position& target)
{
    setMicrostep(TRACKER_MOTOR_MICROSTEPPING/2);
    int64_t deltaArcseconds = target.arcseconds - current.arcseconds;
//    int64_t stepsToMove = deltaArcseconds / ARCSEC_PER_STEP;
    // Value of 60 refers to resolution of second, if 256 microsteps used. 60 for 1.8deg stepper, 120 for 0.9
//    int64_t stepsToMove = (deltaArcseconds * 60) / (MAX_MICROSTEPS/(microStep ? microStep : 1));
//    int64_t stepsToMove = (deltaArcseconds * 281.6) / (MAX_MICROSTEPS/(microStep ? microStep : 1));
//    int64_t stepsToMove = (deltaArcseconds * 277.3) / (MAX_MICROSTEPS/(microStep ? microStep : 1));

    int64_t stepsToMove = (deltaArcseconds * STEPS_PER_SECOND_256MICROSTEP) / (MAX_MICROSTEPS/(microStep ? microStep : 1));
    bool direction = stepsToMove > 0;

    setPosition(current.arcseconds*STEPS_PER_SECOND_256MICROSTEP);
    resetAxisCount();
    setAxisTargetCount(stepsToMove);

    if (targetCount != axisCountValue)
    {
        counterActive = true;
        goToTarget = true;
        stepTimer.stop();
        setDirection(direction);
        slewActive = true;
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
    setDirection(directionArg);
    slewActive = true;
    setMicrostep(TRACKER_MOTOR_MICROSTEPPING/2);
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
	axisAbsoluteDirection = directionArg;
    digitalWrite(dirPin, directionArg ^ invertDirectionPin);
}

void Axis::setMicrostep(uint16_t microstep)
{
	microStep = microstep;
    driver->setMicrosteps(microstep);
}

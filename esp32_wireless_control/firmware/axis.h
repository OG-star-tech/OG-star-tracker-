#ifndef AXIS_H
#define AXIS_H

#include "config.h"
#include "hardwaretimer.h"
#include "motor_driver.h"

#include "tracking_rates.h"

extern HardwareTimer slewTimeOut;

class Position
{
  public:
    int64_t arcseconds;

    Position(int degrees = 0, int minutes = 0, float seconds = 0.0f);
    float toDegrees() const;
    static int64_t toArcseconds(int degrees, int minutes, float seconds);
};

class Axis
{
  public:
    Axis(uint8_t axisNumber, MotorDriver* driver, uint8_t dirPinforAxis, bool invertDirPin);

    void setAxisTargetCount(int64_t count);
    int64_t getAxisTargetCount();
    void resetAxisCount();
    void setAxisCount(int64_t count);
    int64_t getAxisCount();

    void startTracking(trackingRateS rate, bool directionArg);
    void stopTracking();
    void startSlew(uint64_t rate, bool directionArg);
    void stopSlew();

    void gotoTarget(uint64_t rate, const Position& current, const Position& target);
    void stopGotoTarget();

    volatile int64_t axisCountValue;
    volatile int64_t targetCount;
    volatile bool goToTarget;
    bool slewActive;
    bool trackingActive;
    volatile bool axisAbsoluteDirection;
    bool trackingDirection;
    volatile bool counterActive;

    trackingRateS trackingRate;

    uint16_t getMicrostep() { return microStep; }

    volatile int64_t position;
    void resetPosition() { setPosition(0); }
    void setPosition(int64_t pos) { position = pos; }
    int64_t getPosition() { return position; }

    HardwareTimer stepTimer;
  private:
    void setDirection(bool directionArg);
    void setMicrostep(uint16_t microstep);

    uint16_t microStep;
    uint8_t stepPin;
    uint8_t dirPin;
    uint8_t axisNumber;
    bool invertDirectionPin;
    MotorDriver* driver;
};

extern Axis ra_axis;
extern Axis dec_axis;

#endif

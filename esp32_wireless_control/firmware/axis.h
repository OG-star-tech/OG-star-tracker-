#ifndef AXIS_H
#define AXIS_H

#include "config.h"
#include "hardwaretimer.h"
#include <Arduino.h>
#include <TMCStepper.h>

extern HardwareTimer slewTimeOut;

// Values below assume 64 micro steps with f_cpu @ 240MHz
#if STEPPER_TYPE == STEPPER_0_9
enum trackingRateS
{
    TRACKING_SIDEREAL = 1994537, // SIDEREAL (23h,56 min)
    TRACKING_SOLAR = 2000000,    // SOLAR (24h)
    TRACKING_LUNAR = 2042900,    // LUNAR (24h, 31 min)
};
#else // stepper 1.8 deg
enum trackingRateS
{
    TRACKING_SIDEREAL = 3989074, // SIDEREAL (23h,56 min)
    TRACKING_SOLAR = 4000000,    // SOLAR (24h)
    TRACKING_LUNAR = 4085801,    // LUNAR (24h, 31 min)
};
#endif

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
    Axis(uint8_t axisNumber, uint8_t dirPinforAxis, bool invertDirPin, HardwareSerial *serialPort, uint8_t addr);

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

    uint8_t getMicrostep() { return microStep; }

    volatile int64_t position;
    void resetPosition() { setPosition(0); }
    void setPosition(int64_t pos) { position = pos; }
    int64_t getPosition() { return position; }

  private:
    void setDirection(bool directionArg);
    void setMicrostep(uint8_t microstep);

    HardwareSerial *serialPort;
    uint8_t addr;
    TMC2209Stepper tmc_driver;
    HardwareTimer stepTimer;
    uint8_t microStep;
    uint8_t stepPin;
    uint8_t dirPin;
    uint8_t axisNumber;
    bool invertDirectionPin;
//    static const uint8_t MS1Pin = MS1;
//    static const uint8_t MS2Pin = MS2;
};

extern Axis ra_axis;
extern Axis dec_axis;

#endif

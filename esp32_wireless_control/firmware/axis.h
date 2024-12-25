#ifndef AXIS_H
#define AXIS_H

#include "config.h"
#include "hardwaretimer.h"

extern HardwareTimer slewTimeOut;

#ifdef STEPPER_0_9
enum trackingRateS
{
    TRACKING_SIDEREAL = 2659383, // SIDEREAL (23h,56 min)
    TRACKING_SOLAR = 2666666,    // SOLAR (24h)
    TRACKING_LUNAR = 2723867,    // LUNAR (24h, 31 min)
};
#else // stepper 1.8 deg
enum trackingRateS
{
    TRACKING_SIDEREAL = 5318765, // SIDEREAL (23h,56 min)
    TRACKING_SOLAR = 5333333,    // SOLAR (24h)
    TRACKING_LUNAR = 5447735,    // LUNAR (24h, 31 min)
};
#endif

class Axis
{
  public:
    Axis(uint8_t axisNumber, uint8_t dirPinforAxis, bool invertDirPin);
    void startTracking(trackingRateS rate, bool directionArg);
    void stopTracking();
    void startSlew(uint64_t rate, bool directionArg);
    void stopSlew();
    volatile int64_t axisCountValue;
    volatile int64_t targetCount;
    volatile bool goToTarget;
    bool slewActive;
    bool trackingActive;
    volatile bool axisAbsoluteDirection;
    bool trackingDirection;
    volatile bool counterActive;
    void setAxisTargetCount(int64_t count);
    void resetAxisCount();

    trackingRateS trackingRate;

  private:
    void setDirection(bool directionArg);
    static void setMicrostep(uint8_t microstep);
    HardwareTimer stepTimer;
    uint8_t stepPin;
    uint8_t dirPin;
    uint8_t axisNumber;
    bool invertDirectionPin;
    static const uint8_t MS1Pin = MS1;
    static const uint8_t MS2Pin = MS2;
};

extern Axis ra_axis;
extern Axis dec_axis;

#endif
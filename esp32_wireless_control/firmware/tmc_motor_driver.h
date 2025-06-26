#ifndef _TMC_MOTOR_DRIVER_H_
#define _TMC_MOTOR_DRIVER_H_ 1

#include "config.h"
#include "motor_driver.h"
#include <Arduino.h>
#include <TMCStepper.h>

class TmcMotorDriver: public MotorDriver
{
public:
    TmcMotorDriver(HardwareSerial *serialPort, uint8_t addr, float rSense , uint8_t rxPin, uint8_t txPin);
    ~TmcMotorDriver(){};
    void setMicrosteps(uint16_t microsteps);

private:
    HardwareSerial *serialPort;
    uint8_t addr;
#if TMC_DRIVER_MODEL == 2208
    TMC2208Stepper tmc_driver;
#else
    TMC2209Stepper tmc_driver;
#endif
};

#endif /* _TMC_MOTOR_DRIVER_H_ */

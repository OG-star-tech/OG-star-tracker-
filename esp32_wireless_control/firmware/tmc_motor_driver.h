#ifndef _TMC_MOTOR_DRIVER_H_
#define _TMC_MOTOR_DRIVER_H_ 1

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
    TMC2209Stepper tmc_driver;
};

#endif /* _TMC_MOTOR_DRIVER_H_ */

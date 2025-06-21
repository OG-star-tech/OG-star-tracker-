#ifndef _MOTOR_DRIVER_H_
#define _MOTOR_DRIVER_H_ 1

#include <stdint.h>

class MotorDriver
{
  public:
    MotorDriver() {};
    virtual void setMicrosteps(uint8_t microsteps) = 0;
};

#endif /* _MOTOR_DRIVER_H_ */

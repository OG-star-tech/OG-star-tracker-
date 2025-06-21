#ifndef _MSX_MOTOR_DRIVER_H_
#define _MSX_MOTOR_DRIVER_H_ 1

#include "motor_driver.h"

class MSxMotorDriver: public MotorDriver
{
public:
  MSxMotorDriver(uint8_t ms1Pin, uint8_t ms2Pin);
  ~MSxMotorDriver() {};
  void setMicrosteps(uint8_t microsteps);

private:
  uint8_t _ms1Pin;
  uint8_t _ms2Pin;
};

#endif /* _MSX_MOTOR_DRIVER_H_ */

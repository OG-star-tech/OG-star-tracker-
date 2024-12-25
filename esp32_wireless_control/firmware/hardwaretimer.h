#ifndef TIMER_H
#define TIMER_H
#include "esp32-hal-timer.h"

class HardwareTimer
{
  private:
    hw_timer_t* timer_pointer = NULL;
    void (*ISR_Function)();

  public:
    HardwareTimer(uint64_t frequency);
    HardwareTimer(uint64_t frequency, void (*functionToCall)());
    void attachInterupt(void (*functionToCall)());
    void start(uint64_t alarmValue, bool autoReload);
    void stop();
    void setCountValue(uint64_t countValue);
};

#endif
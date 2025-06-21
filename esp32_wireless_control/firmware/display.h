#ifndef _DISPLAY_H_
#define _DISPLAY_H_

#include <Arduino.h>

class Display
{
  public:
    Display(uint8_t sda_pin, uint8_t scl_pin);

    void begin();

    void updateDisplay();
    const char* getCurrentStatusMessage();

  private:
    static void displayTask(void* pvParameters);
    uint8_t _sda_pin;
    uint8_t _scl_pin;
};

extern Display display;

#endif /* _DISPLAY_H_ */

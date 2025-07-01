#ifndef _ARDUINO_NANO_ESP32_H_
#define _ARDUINO_NANO_ESP32_H_ 1

#include "pins_config.h"

//#undef RA_INVERT_DIR_PIN          // if need to invert direction pin set to 1
//#define RA_INVERT_DIR_PIN 1          // if need to invert direction pin set to 1

#define HAVE_ENCODER 1
#define ENCODER_A_PIN Coolant
#define ENCODER_B_PIN Resume

#define HAVE_BUTTON 1
#define BUTTON_PIN Hold


/*****DO NOT MODIFY BELOW*****/
#define MOTOR_TRACKING_RATE	TRACKING_RATE_NATGEO

#define TMC_DRIVER_MODEL 2208
#define MICROSTEPPING_MOTOR_DRIVER USE_TMC_DRIVER_MICROSTEPPING

// LEDs for intervalometer status and general purpose status led

#define STATUS_LED_INVERTED 1
#define STATUS_LED LED_RED // (Red)
#define INTERV_PIN SpinDir

#define LANG_EEPROM_ADDR 0
#define PRESETS_EEPROM_START_LOCATION 1

// Stepper driver pins -- intended for TMC2209 for now
// AXIS 1 - RA

#define AXIS1_STEP Y_DIR
#define AXIS1_DIR Y_STEP

#define AXIS2_STEP A6
#define AXIS2_DIR A7

#define EN12_n MotorEnable

#define HAVE_DISPLAY 1
// LCD Display settings
//#define LCD_COLUMNS 16
//#define LCD_ROWS	2
#define LCD_COLUMNS 20
#define LCD_ROWS	4

#define SDA_PIN		SDA
#define SCL_PIN		SCL

#endif /* _ARDUINO_NANO_ESP32_H_ */

#ifndef CONFIG
#define CONFIG

/*To build in Arduino IDE, install esp32 boards V3.0x and the Arduinojson library by Benoit
 * Blanchon*/
/*****USER DEFINED*****/
// AP mode by default: ESP32 will create a wifi network which you can connect to
#define AP // comment this line if you want ESP32 to connect to your existing wifi network/hotspot
#define c_DIRECTION 1                // 1 is for north hemisphere and 0 for south hemisphere
#define RA_INVERT_DIR_PIN 0          // if need to invert direction pin set to 1
#define DEC_INVERT_DIR_PIN 0         // if need to invert direction pin set to 1
#define DEFAULT_ENABLE_TRACKING 0    // set to 1 to enable tracking at startup
#define DITHER_DISTANCE_X10_PIXELS 5 // set max distance to dither in multiple of 10 pixels
#define MAX_CUSTOM_SLEW_RATE 400     // Set max custom slew rate to  X tracking rate
#define STEPPER_0_9                  // uncomment this line if you have a 0.9 degree NEMA17
// #define STEPPER_1_8   //uncomment this line if you have a 1.8 degree NEMA17, and comment the
// above line
/**********************/

/*****DO NOT MODIFY BELOW*****/
// LEDs for intervalometer status and general purpose status led
#define INTERV_PIN 25
#define STATUS_LED 26
#define LANG_EEPROM_ADDR 0
#define PRESETS_EEPROM_START_LOCATION 1

// Stepper driver pins -- intended for TMC2209 for now
// AXIS 1 - RA
#define AXIS1_STEP 5
#define AXIS1_DIR 15
#define SPREAD_1 4
// AXIS 2 - DEC
#define AXIS2_STEP 19
#define AXIS2_DIR 18
#define SPREAD_2 21
// common pins
#define MS1 23
#define MS2 22
#define EN12_n 17

#endif

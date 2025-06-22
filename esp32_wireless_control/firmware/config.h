#ifndef CONFIG
#define CONFIG

#define STEPPER_0_9 0 // 0.9 degree stepper motor
#define STEPPER_1_8 1 // 1.8 degree stepper motor

/*To build in Arduino IDE, install esp32 boards V3.0x and the Arduinojson library by Benoit
 * Blanchon*/
/*****USER DEFINED*****/
// AP mode by default: ESP32 will create a wifi network which you can connect to
#define AP // comment this line if you want ESP32 to connect to your existing wifi network/hotspot
#define c_DIRECTION 1                // 1 is for north hemisphere and 0 for south hemisphere
#define RA_INVERT_DIR_PIN 0          // if need to invert direction pin set to 1
#define DEC_INVERT_DIR_PIN 0         // if need to invert direction pin set to 1
#define DEFAULT_ENABLE_TRACKING 1    // set to 1 to enable tracking at startup
#define DITHER_DISTANCE_X10_PIXELS 5 // set max distance to dither in multiple of 10 pixels
#define MAX_CUSTOM_SLEW_RATE 400     // Set max custom slew rate to X tracking rate
#define MIN_CUSTOM_SLEW_RATE 2       // Set min custom slew rate to X tracking rate

#ifndef TRACKING_RATE
// Available tracking rates:
// TRACKING_SIDEREAL
// TRACKING_SOLAR
// TRACKING_LUNAR
#define TRACKING_RATE TRACKING_SIDEREAL // default tracking rate
#endif

// Configure the stepper motor type
#ifndef STEPPER_TYPE
#define STEPPER_TYPE STEPPER_0_9 // 0.9 degree stepper motor
#endif
// Configure the wifi settings if you are not using platformio
#ifndef WIFI_SSID
#define WIFI_SSID "OG Star Tracker" // change to your SSID
#endif
#ifndef WIFI_PASSWORD
#define WIFI_PASSWORD "password123" // change to your password, must be 8+ characters
#endif
// If you are using AP mode, you can access the website using the below URL
#ifndef MDNS_NAME
#define MDNS_NAME "ogtracker"
#endif
#ifndef WEBSERVER_PORT
#define WEBSERVER_PORT 80
#endif

// try to update every time there is breaking change
#ifndef INTERNAL_VERSION
#define INTERNAL_VERSION 7
#endif
/**********************/

/*****DO NOT MODIFY BELOW*****/
// Set the resolution per step for the stepper motor
#if STEPPER_TYPE == STEPPER_0_9
#define ARCSEC_PER_STEP 2.0
#else
#define ARCSEC_PER_STEP 4.0
#endif

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

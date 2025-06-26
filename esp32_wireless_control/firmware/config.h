#ifndef CONFIG
#define CONFIG 1

#ifdef HAVE_WIFI_CONFIG
#include "wifi_config.h"
#endif

#define STEPPER_0_9 0 // 0.9 degree stepper motor
#define STEPPER_1_8 1 // 1.8 degree stepper motor

#define TRACKING_RATE_BOARD_V2	1
#define TRACKING_RATE_D1_R32	2
#define TRACKING_RATE_NATGEO	3

//#define MOTOR_TRACKING_RATE	TRACKING_RATE_BOARD_V2
//#define MOTOR_TRACKING_RATE	TRACKING_RATE_D1_R32
//#define MOTOR_TRACKING_RATE	TRACKING_RATE_NATGEO

/*To build in Arduino IDE, install esp32 boards V3.0x and the Arduinojson library by Benoit
 * Blanchon*/
/*****USER DEFINED*****/
// AP mode by default: ESP32 will create a wifi network which you can connect to
//#define AP // comment this line if you want ESP32 to connect to your existing wifi network/hotspot
#define c_DIRECTION 1                // 1 is for north hemisphere and 0 for south hemisphere
#define RA_INVERT_DIR_PIN 0          // if need to invert direction pin set to 1
#define DEC_INVERT_DIR_PIN 0         // if need to invert direction pin set to 1
#define DEFAULT_ENABLE_TRACKING 1    // set to 1 to enable tracking at startup
#define DITHER_DISTANCE_X10_PIXELS 5 // set max distance to dither in multiple of 10 pixels
#define MAX_CUSTOM_SLEW_RATE 1500     // Set max custom slew rate to X tracking rate
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

#define TMC_R_SENSE 0.11f // Match to your driver
#define AXIS_SERIAL_PORT Serial2
#define AXIS_RX X_STOP
#define AXIS_TX Z_DIR

#define AXIS1_ADDR 0
#define AXIS2_ADDR 1

/**********************/

/*****DO NOT MODIFY BELOW*****/

#if STEPPER_TYPE == STEPPER_0_9
#define ARCSEC_PER_STEP 2.0
#else
#define ARCSEC_PER_STEP 4.0
#endif

#define USE_MSx_PINS_MICROSTEPPING 1
#define USE_TMC_DRIVER_MICROSTEPPING 2

#if defined(BOARD_BOARD_V2)
#	include "config/board_v2.h"
#elif defined(BOARD_WEMOS_D1_R32)
#	include "config/wemos_d1_r32.h"
#elif defined(BOARD_ARDUINO_NANO_ESP32)
#	include "config/arduino_nano_esp32.h"
#else
#	error Unknown board
#endif

#ifndef MICROSTEPPING_MOTOR_DRIVER
#define MICROSTEPPING_MOTOR_DRIVER USE_MSx_PINS_MICROSTEPPING
#endif

#ifndef MOTOR_TRACKING_RATE
#define MOTOR_TRACKING_RATE TRACKING_RATE_BOARD_V2
#endif


#endif

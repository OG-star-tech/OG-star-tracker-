#ifndef CONFIG
#define CONFIG

/*****USER DEFINED*****/
//AP mode by default: ESP32 will create a wifi network which you can connect to
#define AP              //comment this line if you want ESP32 to connect to your existing wifi network/hotspot
#define c_DIRECTION 1   //1 is for north hemisphere and 0 for south hemisphere
#define MAX_CUSTOM_SLEW_RATE 400
#define STEPPER_0_9     //uncomment this line if you have a 0.9 degree NEMA17
//#define STEPPER_1_8   //uncomment this line if you have a 1.8 degree NEMA17, and comment the above line
/**********************/



/*****DO NOT MODIFY BELOW*****/
//LEDs for intervalometer status and general purpose status led
#define INTERV_PIN    25
#define STATUS_LED    26

//Stepper driver pins -- intended for TMC2209 for now
//AXIS 1 - RA
#define AXIS1_STEP     5 
#define AXIS1_DIR     15
#define SPREAD_1       4
//AXIS 2 - DEC
#define AXIS2_STEP    19
#define AXIS2_DIR     18
#define SPREAD_2      21
//common pins
#define MS1           23
#define MS2           22
#define EN12_n        17

// EEPROM addresses
#define LANG_EEPROM_ADDR 0x10  // Language setting EEPROM address

#endif

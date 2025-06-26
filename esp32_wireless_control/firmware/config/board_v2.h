#ifndef _BOARD_V2_H_
#define _BOARD_V2_H_ 1

/*****DO NOT MODIFY BELOW*****/
// Set the resolution per step for the stepper motor
// LEDs for intervalometer status and general purpose status led
// Set the resolution per step for the stepper motor

// LEDs for intervalometer status and general purpose status led
#define STATUS_LED_INVERTED 0
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

#define RA_MS1	MS1
#define RA_MS2	MS2

#define DEC_MS1	MS1
#define DEC_MS2	MS2

#endif /* _BOARD_V2_H_ */

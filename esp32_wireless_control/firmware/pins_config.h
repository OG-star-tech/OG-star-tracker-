#ifndef _PINS_CONFIG_H
#define _PINS_CONFIG_H

// CNC Shield v3 pins

#define X_DIR		D5
#define Y_DIR		D6
#define Z_DIR		D7
#define X_STEP		D2
#define Y_STEP		D3
#define Z_STEP		D4
#define MotorEnable	D8

#define X_STOP		D9
#define Y_STOP		D10
#define Z_STOP		D11

#define SpinEnable	D12
#define SpinDir		D13

#define Abort		A0
#define Hold		A1
#define Resume		A2
#define Coolant		A3

/*
 * Helper macro for getting a macro definition as string
 */
#if !defined(STR_HELPER)
#define STR_HELPER(x) #x
#define STR(x) STR_HELPER(x)
#endif


#endif /* _PINS_CONFIG_H */

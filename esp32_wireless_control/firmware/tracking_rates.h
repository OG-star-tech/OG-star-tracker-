#ifndef _TRACKING_RATES_H_
#define _TRACKING_RATES_H_ 1

#include "config.h"

#if MOTOR_TRACKING_RATE == TRACKING_RATE_BOARD_V2
#if STEPPER_TYPE == STEPPER_0_9
// gear ratio 101.25, 0.9deg motor, 64?? msteps, f_cpu@240MHz
#define TRACKER_MOTOR_MICROSTEPPING 64
enum trackingRateS
{
    TRACKING_SIDEREAL = 2659383, // SIDEREAL (23h,56 min)
    TRACKING_SOLAR = 2666667,    // SOLAR (24h)
    TRACKING_LUNAR = 2723867,    // LUNAR (24h, 31 min)
};
#else // stepper 1.8 deg
// gear ratio 101.25, 1.8deg motor, 64?? msteps, f_cpu@240MHz
#define TRACKER_MOTOR_MICROSTEPPING 64
enum trackingRateS
{
    TRACKING_SIDEREAL = 5318765, // SIDEREAL (23h,56 min)
    TRACKING_SOLAR = 5333333,    // SOLAR (24h)
    TRACKING_LUNAR = 5447735,    // LUNAR (24h, 31 min)
};
#endif
#elif MOTOR_TRACKING_RATE == TRACKING_RATE_D1_R32
// gear ratio 101.25, 1.8deg motor, 64 msteps, f_cpu@240MHz
//#define TRACKER_MOTOR_MICROSTEPPING 64
//enum trackingRateS
//{
//    TRACKING_SIDEREAL = 3989074, // SIDEREAL (23h,56 min)
//    TRACKING_SOLAR = 4000000,    // SOLAR (24h)
//    TRACKING_LUNAR = 4085801,    // LUNAR (24h, 31 min)
//};
// gear ratio 101.25, 1.8deg motor, 64?? msteps, f_cpu@240MHz
#define TRACKER_MOTOR_MICROSTEPPING 64
enum trackingRateS
{
    TRACKING_SIDEREAL = 5318765, // SIDEREAL (23h,56 min)
    TRACKING_SOLAR = 5333333,    // SOLAR (24h)
    TRACKING_LUNAR = 5447735,    // LUNAR (24h, 31 min)
};

#elif MOTOR_TRACKING_RATE == TRACKING_RATE_NATGEO
// gear ratio 475.2, 1.8deg motor, 16 msteps, f_cpu@240MHz
#define TRACKER_MOTOR_MICROSTEPPING 16
enum trackingRateS
{
    TRACKING_SIDEREAL = 3399779, // SIDEREAL (23h,56 min)
    TRACKING_SOLAR = 3409091,    // SOLAR (24h)
    TRACKING_LUNAR = 3482217,    // LUNAR (24h, 31 min)
};
#else
#	error Unknown tracking rate setting
#endif


#endif /* _TRACKING_RATES_H_ */

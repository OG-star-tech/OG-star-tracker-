#ifndef ENGLISH_LANG_H
#define ENGLISH_LANG_H
#include "web_languages.h"

const char* englishLangLanguageNames[LANG_COUNT] = {
    // LANGUAGE NAMES
    "English", // ENGLISH
    "Chinese", // CHINESE
    "German",  // GERMAN
};

const char* englishLangMessageStrings[NUMBER_OF_MESSAGES] = {
    // MESSAGE STRINSS
    "Tracking On",                                // MSG_TRACKING_ON
    "Tracking Off",                               // MSG_TRACKING_OFF
    "Slewing",                                    // MSG_SLEWING
    "Slew Cancelled",                             // MSG_SLEW_CANCELLED
    "OK",                                         // MSG_OK
    "Saved Preset",                               // MSG_SAVED_PRESET
    "TRACKING NOT ACTIVE! Please Start Tracking", // MSG_TRACKING_NOT_ACTIVE
    "Capture On",                                 // MSG_CAPTURE_ON
    "Capture OFF",                                // MSG_CAPTURE_OFF
    "Capture Already ON",                         // MSG_CAPTURE_ALREADY_ON
    "Capture Already OFF",                        // MSG_CAPTURE_ALREADY_OFF
    "Capture: Predelay",                          // MSG_CAP_PREDELAY
    "Capture: Exposing",                          // MSG_CAP_EXPOSING
    "Capture: Dither",                            // MSG_CAP_DITHER
    "Capture: Panning",                           // MSG_CAP_PANNING
    "Capture: Delay",                             // MSG_CAP_DELAY
    "Capture: Rewind",                            // MSG_CAP_REWIND
    "Goto RA: Panning ON",                        // MSG_GOTO_RA_PANNING_ON
    "Goto RA: Panning OFF",                       // MSG_GOTO_RA_PANNING_OFF
    "Idle",                                       // MSG_IDLE
};

const char* englishLangErrorMessageStrings[NUMBER_OF_ERROR_MESSAGES] = {
    // ERROR MESSAGE STRINGS
    "Invalid Capture Mode",          // ERR_MSG_INVALID_CAPTURE_MODE
    "Invalid Exposure Length",       // ERR_MSG_INVALID_EXPOSURE_LENGTH
    "Invalid Number of Exposures",   // ERR_MSG_INVALID_EXPOSURE_AMOUNT
    "Invalid Predelay Time",         // ERR_MSG_INVALID_PREDELAY_TIME
    "Invalid Delay Time",            // ERR_MSG_INVALID_DELAY_TIME
    "Invalid Frame Amount",          // ERR_MSG_INVALID_FRAME_AMOUNT
    "Invalid Pan Angle",             // ERR_MSG_INVALID_PAN_ANGLE
    "Invalid Pan Direction",         // ERR_MSG_INVALID_PAN_DIRECTION
    "Invalid Enable Tracking Value", // ERR_MSG_INVALID_ENABLE_TRACKING_VALUE
    "Invalid Dither Choice",         // ERR_MSG_INVALID_DITHER_CHOICE
    "Invalid Dither Frequency",      // ERR_MSG_INVALID_DITHER_FREQUENCY
    "Invalid Focal Length",          // ERR_MSG_INVALID_FOCAL_LENGTH
    "Invalid Pixel Size"             // ERR_MSG_INVALID_PIXEL_SIZE
};

const char* englishLangHTMLStrings[numberOfHTMLStrings] = {
    // HTML STRINGS
    "OG Star Tracker Control Panel", // STR_TITLE
    "TRACKER CONTROLLER",            // STR_TRACKER_CONTOLLER
    "Tracking",                      // STR_TRACKING
    "Hemisphere",                    // STR_HEMISPHERE
    "North",                         // STR_NORTH
    "South",                         // STR_SOUTH
    "Tracking Rate",                 // STR_TRACKING_RATE
    "Sidereal",                      // STR_SIDEREAL
    "Solar",                         // STR_SOLAR
    "Lunar",                         // STR_LUNAR
    "ON",                            // STR_BTN_ON
    "OFF",                           // STR_BTN_OFF
    "Slew Control",                  // STR_SLEW_CONTROL
    "Speed Multiplier",              // STR_SPEED_MULTIPLIER
    "Custom",                        // STR_CUSTOM
    "Custom Speed",                  // STR_CUSTOM_SPEED
    "Hold direction button to slew", // STR_SLEW_HINT
    "Slew Left",                     // STR_SLEW_LEFT
    "Slew Right",                    // STR_SLEW_RIGHT
    "Abort Slew",                    // STR_ABORT_SLEW
    "INTERVALOMETER",                // STR_INTERVALOMETER
    "Preset",                        // STR_PRESET
    "SAVE PRESET",                   // STR_SAVE_PRESET
    "LOAD PRESET",                   // STR_LOAD_PRESET
    "Mode",                          // STR_MODE
    "Long Exposure Still",           // STR_LONG_EXPOSURE_STILL
    "Long Exposure Movie",           // STR_LONG_EXPOSURE_MOVIE
    "Day Time Lapse",                // STR_DAY_TIME_LAPSE
    "Day Time Lapse Pan",            // STR_DAY_TIME_LAPSE_PAN
    "Exposure Length",               // STR_EXPOSURE_LENGTH
    "in seconds (Ex. 30)",           // STR_EXPOSURE_HINT
    "Number of exposures",           // STR_NO_EXPOSURES
    "number of photos (Ex. 20)",     // STR_NUM_EXPOSURES_HINT
    "Pre Delay Time (5s)",           // STR_PRE_DELAY
    "Delay",                         // STR_DELAY
    "Number of Frames",              // STR_FRAMES
    "nº of frames",                  // STR_NO_FRAMES
    "Pan Angle",                     // STR_PAN_ANGLE
    "hours (Ex. 6)",                 // STR_HOURS
    "Pan Direction",                 // STR_PAN_DIRECTION
    "Left",                          // STR_LEFT
    "Right",                         // STR_RIGHT
    "Goto RA Control",               // STR_GOTO_RA_CONTROL
    "Goto RA Current",               // STR_GOTO_RA_CURRENT
    "Goto RA Target",                // STR_GOTO_RA_TARGET
    "Center your camera on\n"
    "memorable star like Deneb,Altair..\n"
    "then enter coordinates of target\n", // STR_GOTO_RA_HINT
    "Start Goto RA",                      // STR_START_GOTO_RA
    "Stop Goto RA",                       // STR_STOP_GOTO_RA
    "Disable Tracking On End",            // STR_ENABLE_TRACKING
    "No",                                 // STR_NO
    "Yes",                                // STR_YES
    "Dither",                             // STR_DITHER
    "Dither Frequency",                   // STR_DITHER_FREQ
    "Lens focal length",                  // STR_FOCAL_LENGTH
    "in millimeters (Ex. 135)",           // STR_FOCAL_LENGTH_HINT
    "Camera pixel size",                  // STR_PIXEL_SIZE
    "in micrometers (Ex. 4.1)",           // STR_PIXEL_SIZE_HINT
    "Start Capture",                      // STR_START_CAPTURE
    "Abort Capture",                      // STR_ABORT_CAPTURE
    "Capture Time",                       // STR_CAPTURE_TIME
    "Status",                             // STR_STATUS
    "Ready",                              // STR_STATUS_MSG
    "Firmware Version"                    // STR_FIRMWARE_VERSION
};

#endif

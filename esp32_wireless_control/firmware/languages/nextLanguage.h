#ifndef NEXT_LANG_H
#define NEXT_LANG_H
#include "web_languages.h"

/*Use this template to add an additional lamguage.
After finishing adjust web_languages.h and .cpp to include the new language
*/

const char* nextLanguageStrings[LANG_COUNT] = {
    // LANGUAGE NAMES
    "", // ENGLISH
    "", // CHINESE
};

const char* nextLangMessageStrings[NUMBER_OF_MESSAGES] = {
    // MESSAGE STRINSS
    "", // MSG_TRACKING_ON
    "", // MSG_TRACKING_OFF
    "", // MSG_SLEWING
    "", // MSG_SLEW_CANCELLED
    "", // MSG_OK
    "", // MSG_SAVED_PRESET
    "", // MSG_TRACKING_NOT_ACTIVE
    "", // MSG_CAPTURE_ON
    "", // MSG_CAPTURE_OFF
    "", // MSG_CAPTURE_ALREADY_ON
    "", // MSG_CAPTURE_ALREADY_OFF
    "", // MSG_CAP_PREDELAY
    "", // MSG_CAP_EXPOSING
    "", // MSG_CAP_DITHER
    "", // MSG_CAP_PANNING
    "", // MSG_CAP_DELAY
    "", // MSG_CAP_REWIND
    "", // MSG_GOTO_RA_PANNING_ON
    "", // MSG_GOTO_RA_PANNING_OFF
    "", // MSG_IDLE
};

const char* nextLangErrorMessageStrings[NUMBER_OF_ERROR_MESSAGES] = {
    // ERROR MESSAGE STRINGS
    "", // ERR_MSG_INVALID_CAPTURE_MODE
    "", // ERR_MSG_INVALID_EXPOSURE_LENGTH
    "", // ERR_MSG_INVALID_EXPOSURE_AMOUNT
    "", // ERR_MSG_INVALID_PREDELAY_TIME
    "", // ERR_MSG_INVALID_DELAY_TIME
    "", // ERR_MSG_INVALID_FRAME_AMOUNT
    "", // ERR_MSG_INVALID_PAN_ANGLE
    "", // ERR_MSG_INVALID_PAN_DIRECTION
    "", // ERR_MSG_INVALID_ENABLE_TRACKING_VALUE
    "", // ERR_MSG_INVALID_DITHER_CHOICE
    "", // ERR_MSG_INVALID_DITHER_FREQUENCY
    "", // ERR_MSG_INVALID_FOCAL_LENGTH
    ""  // ERR_MSG_INVALID_PIXEL_SIZE
};

const char* nextLangHTMLStrings[numberOfHTMLStrings] = {
    // HTML STRINGS
    "", // STR_TITLE
    "", // STR_TRACKER_CONTOLLER
    "", // STR_TRACKING
    "", // STR_HEMISPHERE
    "", // STR_NORTH
    "", // STR_SOUTH
    "", // STR_TRACKING_RATE
    "", // STR_SIDEREAL
    "", // STR_SOLAR
    "", // STR_LUNAR
    "", // STR_BTN_ON
    "", // STR_BTN_OFF
    "", // STR_SLEW_CONTROL
    "", // STR_SPEED_MULTIPLIER
    "", // STR_CUSTOM
    "", // STR_CUSTOM_SPEED
    "", // STR_SLEW_HINT
    "", // STR_SLEW_LEFT
    "", // STR_SLEW_RIGHT
    "", // STR_ABORT_SLEW
    "", // STR_INTERVALOMETER
    "", // STR_PRESET
    "", // STR_SAVE_PRESET
    "", // STR_LOAD_PRESET
    "", // STR_MODE
    "", // STR_LONG_EXPOSURE_STILL
    "", // STR_LONG_EXPOSURE_MOVIE
    "", // STR_DAY_TIME_LAPSE
    "", // STR_DAY_TIME_LAPSE_PAN
    "", // STR_EXPOSURE_LENGTH
    "", // STR_EXPOSURE_HINT
    "", // STR_NO_EXPOSURES
    "", // STR_NUM_EXPOSURES_HINT
    "", // STR_PRE_DELAY
    "", // STR_DELAY
    "", // STR_FRAMES
    "", // STR_NO_FRAMES
    "", // STR_PAN_ANGLE
    "", // STR_HOURS
    "", // STR_PAN_DIRECTION
    "", // STR_LEFT
    "", // STR_RIGHT
    "", // STR_GOTO_RA_CONTROL
    "", // STR_GOTO_RA_CURRENT
    "", // STR_GOTO_RA_TARGET
    "", // STR_GOTO_RA_HINT
    "", // STR_START_GOTO_RA
    "", // STR_STOP_GOTO_RA
    "", // STR_ENABLE_TRACKING
    "", // STR_NO
    "", // STR_YES
    "", // STR_DITHER
    "", // STR_DITHER_FREQ
    "", // STR_FOCAL_LENGTH
    "", // STR_FOCAL_LENGTH_HINT
    "", // STR_PIXEL_SIZE
    "", // STR_PIXEL_SIZE_HINT
    "", // STR_START_CAPTURE
    "", // STR_ABORT_CAPTURE
    "", // STR_CAPTURE_TIME
    "", // STR_STATUS
    "", // STR_STATUS_MSG
    ""  // STR_FIRMWARE_VERSION
};

#endif

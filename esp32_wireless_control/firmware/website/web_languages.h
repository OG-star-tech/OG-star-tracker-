#ifndef WEB_LANGUAGES_H
#define WEB_LANGUAGES_H

#define numberOfHTMLStrings 88

#include "error.h"

enum Languages
{
    EN,
    CN,
    DE,
    // Add next language
    LANG_COUNT
};

enum Messages
{
    MSG_TRACKING_ON,
    MSG_TRACKING_OFF,
    MSG_SLEWING,
    MSG_SLEW_CANCELLED,
    MSG_OK,
    MSG_SAVED_PRESET,
    MSG_TRACKING_NOT_ACTIVE,
    MSG_CAPTURE_ON,
    MSG_CAPTURE_OFF,
    MSG_CAPTURE_ALREADY_ON,
    MSG_CAPTURE_ALREADY_OFF,
    MSG_CAP_PREDELAY,
    MSG_CAP_EXPOSING,
    MSG_CAP_DITHER,
    MSG_CAP_PANNING,
    MSG_CAP_DELAY,
    MSG_CAP_REWIND,
    MSG_GOTO_RA_PANNING_ON,
    MSG_GOTO_RA_PANNING_OFF,
    MSG_POSITION_SET_SUCCESS,
    MSG_IDLE,
    NUMBER_OF_MESSAGES,
};

extern const char* const* languageNames[];
extern const char* const* languageMessageStrings[];
extern const char* const* languageErrorMessageStrings[];
extern const char* const* languageHTMLStrings[];
extern const char* const HTMLplaceHolders[numberOfHTMLStrings];

#endif

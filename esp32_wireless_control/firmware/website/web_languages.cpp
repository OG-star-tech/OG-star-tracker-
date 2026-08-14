#include "web_languages.h"
#include "languages/chineseLanguage.h"
#include "languages/englishLanguage.h"
#include "languages/germanLanguage.h"
// include Next language

/* clang-format off */
const char* const* languageNames[LANG_COUNT] = {
  englishLangLanguageNames,
  chineseLangLanguageNames,
  germanLangLanguageNames
  //nextLanguageLangNames  Array here
};

const char* const* languageMessageStrings[LANG_COUNT] = {
  englishLangMessageStrings,
  chineseLangMessageStrings,
  germanLangMessageStrings
  //nextLanguageMessageStrings Array here
};

const char* const* languageErrorMessageStrings[LANG_COUNT] = {
  englishLangErrorMessageStrings,
  chineseLangErrorMessageStrings,
  germanLangErrorMessageStrings
  //nextLanguageErrorMessageStrings Array here
};

const char* const* languageHTMLStrings[LANG_COUNT] = {
  englishLangHTMLStrings,
  chineseLangHTMLStrings,
  germanLangHTMLStrings
  //nextLanguageHTMLStrings Array here
};

const char* const HTMLplaceHolders[numberOfHTMLStrings] PROGMEM = {
  "%STR_TITLE%",
  "%STR_TRACKER_CONTROLLER%",
  "%STR_TRACKING%",
  "%STR_HEMISPHERE%",
  "%STR_NORTH%",
  "%STR_SOUTH%",
  "%STR_TRACKING_RATE%",
  "%STR_SIDEREAL%",
  "%STR_SOLAR%",
  "%STR_LUNAR%",
  "%STR_BTN_ON%",
  "%STR_BTN_OFF%",
  "%STR_SLEW_CONTROL%",
  "%STR_SPEED_MULTIPLIER%",
  "%STR_CUSTOM%",
  "%STR_CUSTOM_SPEED%",
  "%STR_SLEW_HINT%",
  "%STR_SLEW_LEFT%",
  "%STR_SLEW_RIGHT%",
  "%STR_ABORT_SLEW%",
  "%STR_INTERVALOMETER%",
  "%STR_PRESET%",
  "%STR_SAVE_PRESET%",
  "%STR_LOAD_PRESET%",
  "%STR_MODE%",
  "%STR_LONG_EXPOSURE_STILL%",
  "%STR_LONG_EXPOSURE_MOVIE%",
  "%STR_TIMELAPSE%",
  "%STR_TIMELAPSE_PAN%",
  "%STR_EXPOSURE_LENGTH%",
  "%STR_EXPOSURE_HINT%",
  "%STR_NO_EXPOSURES%",
  "%STR_NUM_EXPOSURES_HINT%",
  "%STR_PRE_DELAY%",
  "%STR_DELAY%",
  "%STR_FRAMES%",
  "%STR_NO_FRAMES%",
  "%STR_PAN_ANGLE%",
  "%STR_HOURS%",
  "%STR_PAN_DIRECTION%",
  "%STR_CONTINUOUS_PAN%",
  "%STR_LEFT%",
  "%STR_RIGHT%",
  "%STR_GOTO_RA_CONTROL%",
  "%STR_GOTO_RA_CURRENT%",
  "%STR_GOTO_RA_TARGET%",
  "%STR_GOTO_RA_HINT%",
  "%STR_START_GOTO_RA%",
  "%STR_STOP_GOTO_RA%",
  "%STR_ENABLE_TRACKING%",
  "%STR_NO%",
  "%STR_YES%",
  "%STR_DITHER%",
  "%STR_DITHER_FREQ%",
  "%STR_FOCAL_LENGTH%",
  "%STR_FOCAL_LENGTH_HINT%",
  "%STR_PIXEL_SIZE%",
  "%STR_PIXEL_SIZE_HINT%",
  "%STR_START_CAPTURE%",
  "%STR_ABORT_CAPTURE%",
  "%STR_CAPTURE_TIME%",
  "%STR_STATUS%",
  "%STR_STATUS_MSG%",
  "%STR_FIRMWARE_VERSION%",
  "%STR_STAR_CATALOG%",
  "%STR_STAR_OBJECT_NAME%",
  "%STR_STAR_SEARCH_PLACEHOLDER%",
  "%STR_STAR_SEARCH%",
  "%STR_STAR_SET_CURRENT%",
  "%STR_STAR_OBJECT_DETAILS%",
  "%STR_STAR_SEARCH_ERROR%",
  "%STR_STAR_OBJECT_NOT_FOUND%",
  "%STR_STAR_RA%",
  "%STR_STAR_DEC%",
  "%STR_STAR_TYPE%",
  "%STR_STAR_MAGNITUDE%",
  "%STR_STAR_CONSTELLATION%",
  "%STR_LOCAL_TIME%",
  "%STR_CURRENT_POSITION%",
  "%STR_CURRENT_RA_POSITION%",
  "%STR_DEC_DIFFERENCE%",
  "%STR_CURRENT_TIME%",
  "%STR_LONGITUDE%",
  "%STR_TUNE_RATE%",
  "%STR_RATE_PRESET%",
  "%STR_SAVE_RATE_PRESET%",
  "%STR_LOAD_RATE_PRESET%",
  "%STR_REVERSE_RA_DIRECTION%"
};
/* clang-format on */

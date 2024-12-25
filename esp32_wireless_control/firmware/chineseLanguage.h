#ifndef CHINESE_LANG_H
#define CHINESE_LANG_H
#include "web_languages.h"

const char* chineseLangLanguageNames[LANG_COUNT] = {
    // LANGUAGE NAMES
    "英语", // ENGLISH
    "中文", // CHINESE
};

const char* chineseLangMessageStrings[NUMBER_OF_MESSAGES] = {
    // MESSAGE STRINSS
    "追踪开启",                                   // MSG_TRACKING_ON
    "追踪关闭",                                   // MSG_TRACKING_OFF
    "快速调整中",                                 // STR_SLEWING
    "Slew Cancelled",                             // MSG_SLEW_CANCELLED
    "OK",                                         // MSG_OK
    "Saved Preset",                               // MSG_SAVED_PRESET
    "TRACKING NOT ACTIVE! Please Start Tracking", // MSG_TRACKING_NOT_ACTIVE
    "拍摄开始",                                   // STR_CAPTURE_ON
    "拍摄停止",                                   // STR_CAPTURE_OFF
    "拍摄已在进行中",                             // STR_CAPTURE_ALREADY_ON
    "拍摄已经停止",                               // STR_CAPTURE_ALREADY_OFF
    "Capture: Predelay",                          // MSG_CAP_PREDELAY
    "Capture: Exposing",                          // MSG_CAP_EXPOSING
    "Capture: Dither",                            // MSG_CAP_DITHER
    "Capture: Panning",                           // MSG_CAP_PANNING
    "Capture: Delay",                             // MSG_CAP_DELAY
    "Capture: Rewind",                            // MSG_CAP_REWIND
    "空闲"                                        // MSG_IDLE
};

const char* chineseLangHTMLStrings[numberOfHTMLStrings] = {
    // HTML STRINGS
    "OG 赤道仪控制面板",        // STR_TITLE
    "TRACKER CONTROLLER",       // STR_TRACKER_CONTOLLER
    "追踪",                     // STR_TRACKING
    "半球",                     // STR_HEMISPHERE
    "北半球",                   // STR_NORTH
    "南半球",                   // STR_SOUTH
    "追踪速率",                 // STR_TRACKING_RATE
    "恒星速率",                 // STR_SIDEREAL
    "太阳速率",                 // STR_SOLAR
    "月球速率",                 // STR_LUNAR
    "开启",                     // STR_BTN_ON
    "关闭",                     // STR_BTN_OFF
    "快速调",                   // STR_SLEW_CONTROL
    "速度倍率",                 // STR_SPEED_MULTIPLIER
    "自定义",                   // STR_CUSTOM
    "自定义速度",               // STR_CUSTOM_SPEED
    "按住方向按钮进行快速调整", // STR_SLEW_HINT
    "向左调整",                 // STR_SLEW_LEFT
    "向右调整",                 // STR_SLEW_RIGHT
    "停止调整",                 // STR_ABORT_SLEW
    "INTERVALOMETER",           // STR_INTERVALOMETER
    "Preset",                   // STR_PRESET
    "SAVE PRESET",              // STR_SAVE_PRESET
    "LOAD PRESET",              // STR_LOAD_PRESET
    "Mode",                     // STR_MODE
    "Long Exposure Still",      // STR_LONG_EXPOSURE_STILL
    "Long Exposure Movie",      // STR_LONG_EXPOSURE_MOVIE
    "Day Time Lapse",           // STR_DAY_TIME_LAPSE
    "Day Time Lapse Pan",       // STR_DAY_TIME_LAPSE_PAN
    "曝光时长",                 // STR_EXPOSURE_LENGTH
    "单位：秒 (例如: 30)",      // STR_EXPOSURE_HINT
    "Number of exposures",      // STR_NO_EXPOSURES
    "拍摄张数 (例如: 20)",      // STR_NUM_EXPOSURES_HINT
    "Pre Delay Time",           // STR_PRE_DELAY
    "Delay",                    // STR_DELAY
    "Number of Frames",         // STR_FRAMES
    "nº of frames",             // STR_NO_FRAMES
    "Pan Angle",                // STR_PAN_ANGLE
    "degrees (Ex. 3.45)",       // STR_DEGREES
    "Pan Direction",            // STR_PAN_DIRECTION
    "Left",                     // STR_LEFT
    "Right",                    // STR_RIGHT
    "Enable Tracking On End",   // STR_ENABLE_TRACKING
    "No",                       // STR_NO
    "Yes",                      // STR_YES
    "Dither",                   // STR_DITHER
    "Dither Frequency",         // STR_DITHER_FREQ
    "Lens focal length",        // STR_FOCAL_LENGTH
    "单位：毫米 (例如: 135)",   // STR_FOCAL_LENGTH_HINT
    "Camera pixel size",        // STR_PIXEL_SIZE
    "单位：微米 (例如: 4.1)",   // STR_PIXEL_SIZE_HINT
    "开始拍摄",                 // STR_START_CAPTURE
    "停止拍摄",                 // STR_ABORT_CAPTURE
    "Capture Time",             // STR_CAPTURE_TIME
    "状态",                     // STR_STATUS
    "就绪",                     // STR_STATUS_MSG
    "固件版本"                  // STR_FIRMWARE_VERSION
};

#endif
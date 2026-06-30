#ifndef CHINESE_LANG_H
#define CHINESE_LANG_H
#include "../website/web_languages.h"
#include <pgmspace.h>

const char* const chineseLangLanguageNames[LANG_COUNT] PROGMEM = {
    // LANGUAGE NAMES
    "英语", // ENGLISH
    "中文", // CHINESE
    "德语", // GERMAN
};

const char* const chineseLangMessageStrings[NUMBER_OF_MESSAGES] PROGMEM = {
    // MESSAGE STRINSS
    "电机开启",                                   // MSG_MOTOR_ON
    "电机关闭",                                   // MSG_MOTOR_OFF
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
    "Goto RA: Panning ON",                        // MSG_GOTO_RA_PANNING_ON
    "Goto RA: Panning OFF",                       // MSG_GOTO_RA_PANNING_OFF
    "Position set successfully",                  // MSG_POSITION_SET_SUCCESS
    "空闲"                                        // MSG_IDLE
};

const char* const chineseLangErrorMessageStrings[NUMBER_OF_ERROR_MESSAGES] PROGMEM = {
    // ERROR MESSAGE STRINGS
    "无效的拍摄模式",   // ERR_MSG_INVALID_CAPTURE_MODE
    "无效的曝光时长",   // ERR_MSG_INVALID_EXPOSURE_LENGTH
    "无效的曝光数量",   // ERR_MSG_INVALID_EXPOSURE_AMOUNT
    "无效的预延迟时间", // ERR_MSG_INVALID_PREDELAY_TIME
    "无效的延迟时间",   // ERR_MSG_INVALID_DELAY_TIME
    "无效的帧数量",     // ERR_MSG_INVALID_FRAME_AMOUNT
    "无效的平移角度",   // ERR_MSG_INVALID_PAN_ANGLE
    "无效的平移时长",   // ERR_MSG_INVALID_PAN_DURATION
    "无效的平移方向",   // ERR_MSG_INVALID_PAN_DIRECTION
    "无效的启用跟踪值", // ERR_MSG_INVALID_ENABLE_TRACKING_VALUE
    "无效的抖动选择",   // ERR_MSG_INVALID_DITHER_CHOICE
    "无效的抖动频率",   // ERR_MSG_INVALID_DITHER_FREQUENCY
    "无效的焦距",       // ERR_MSG_INVALID_FOCAL_LENGTH
    "无效的像素大小"    // ERR_MSG_INVALID_PIXEL_SIZE
};

const char* const chineseLangHTMLStrings[numberOfHTMLStrings] PROGMEM = {
    // HTML STRINGS
    "OG 赤道仪控制面板",                                                  // STR_TITLE
    "TRACKER CONTROLLER",                                                 // STR_TRACKER_CONTOLLER
    "追踪",                                                               // STR_TRACKING
    "半球",                                                               // STR_HEMISPHERE
    "北半球",                                                             // STR_NORTH
    "南半球",                                                             // STR_SOUTH
    "追踪速率",                                                           // STR_TRACKING_RATE
    "恒星速率",                                                           // STR_SIDEREAL
    "太阳速率",                                                           // STR_SOLAR
    "月球速率",                                                           // STR_LUNAR
    "电机开启",                                                           // STR_BTN_MOTOR_ON
    "电机关闭",                                                          // STR_BTN_MOTOR_OFF
    "开启",                                                               // STR_BTN_ON
    "关闭",                                                               // STR_BTN_OFF
    "快速调",                                                             // STR_SLEW_CONTROL
    "速度倍率",                                                           // STR_SPEED_MULTIPLIER
    "自定义",                                                             // STR_CUSTOM
    "自定义速度",                                                         // STR_CUSTOM_SPEED
    "按住方向按钮进行快速调整",                                           // STR_SLEW_HINT
    "向左调整",                                                           // STR_SLEW_LEFT
    "向右调整",                                                           // STR_SLEW_RIGHT
    "停止调整",                                                           // STR_ABORT_SLEW
    "间隔拍摄",                                                           // STR_INTERVALOMETER
    "电机控制",                                                           // STR_MOTOR_CONTROL
    "预设",                                                               // STR_PRESET
    "保存预设",                                                           // STR_SAVE_PRESET
    "加载预设",                                                           // STR_LOAD_PRESET
    "模式",                                                               // STR_MODE
    "长曝光静态",                                                         // STR_LONG_EXPOSURE_STILL
    "长曝光视频",                                                         // STR_LONG_EXPOSURE_MOVIE
    "延时摄影",                                                           // STR_TIMELAPSE
    "延时摄影平移",                                                       // STR_TIMELAPSE_PAN
    "曝光时长",                                                           // STR_EXPOSURE_LENGTH
    "单位：秒 (例如: 30)",                                                // STR_EXPOSURE_HINT
    "曝光次数",                                                           // STR_NO_EXPOSURES
    "拍摄张数 (例如: 20)",                                                // STR_NUM_EXPOSURES_HINT
    "预延迟时间 (秒)",                                                    // STR_PRE_DELAY
    "延迟 (秒)",                                                          // STR_DELAY
    "帧数",                                                               // STR_FRAMES
    "帧数",                                                               // STR_NO_FRAMES
    "平移角度",                                                           // STR_PAN_ANGLE
    "小时 (例如: 6)",                                                     // STR_HOURS
    "平移方向",                                                           // STR_PAN_DIRECTION
    "连续平移",                                                           // STR_CONTINUOUS_PAN
    "左",                                                                 // STR_LEFT
    "右",                                                                 // STR_RIGHT
    "定位赤经控制",                                                       // STR_GOTO_RA_CONTROL
    "当前",                                                               // STR_GOTO_RA_CURRENT
    "目标",                                                               // STR_GOTO_RA_TARGET
    "将相机对准一颗容易辨认的恒星（如天津四、牛郎星等）然后输入目标坐标", // STR_GOTO_RA_HINT
    "开始定位赤经",                                                       // STR_START_GOTO_RA
    "停止定位赤经",                                                       // STR_STOP_GOTO_RA
    "结束时禁用跟踪",                                                     // STR_ENABLE_TRACKING
    "否",                                                                 // STR_NO
    "是",                                                                 // STR_YES
    "微调",                                                               // STR_DITHER
    "微调频率 (例如: 1)",                                                 // STR_DITHER_FREQ
    "镜头焦距",                                                           // STR_FOCAL_LENGTH
    "单位：毫米 (例如: 135)",                                             // STR_FOCAL_LENGTH_HINT
    "相机像素大小 (µm)",                                                  // STR_PIXEL_SIZE
    "单位：微米 (例如: 4.1)",                                             // STR_PIXEL_SIZE_HINT
    "开始拍摄",                                                           // STR_START_CAPTURE
    "停止拍摄",                                                           // STR_ABORT_CAPTURE
    "拍摄时间",                                                           // STR_CAPTURE_TIME
    "状态",                                                               // STR_STATUS
    "就绪",                                                               // STR_STATUS_MSG
    "固件版本",                                                           // STR_FIRMWARE_VERSION
    "星表",                                                               // STR_STAR_CATALOG
    "天体名称",                                                           // STR_STAR_OBJECT_NAME
    "(例如: NGC224)", // STR_STAR_SEARCH_PLACEHOLDER
    "搜索",           // STR_STAR_SEARCH
    "设置当前",       // STR_STAR_SET_CURRENT
    "天体详情",       // STR_STAR_OBJECT_DETAILS
    "搜索错误",       // STR_STAR_SEARCH_ERROR
    "未找到天体",     // STR_STAR_OBJECT_NOT_FOUND
    "赤经",           // STR_STAR_RA
    "赤纬",           // STR_STAR_DEC
    "类型",           // STR_STAR_TYPE
    "星等",           // STR_STAR_MAGNITUDE
    "星座",           // STR_STAR_CONSTELLATION
    "当地时间",       // STR_LOCAL_TIME
    "当前位置",       // STR_CURRENT_POSITION
    "当前赤经位置",   // STR_CURRENT_RA_POSITION
    "赤纬差异",       // STR_DEC_DIFFERENCE
    "当前时间",       // STR_CURRENT_TIME
    "经度",           // STR_LONGITUDE
    "微调",           // STR_TUNE_RATE
    "预设速率",       // STR_RATE_PRESET
    "保存速率",       // STR_SAVE_RATE_PRESET
    "加载速率"        // STR_LOAD_RATE_PRESET
};

#endif

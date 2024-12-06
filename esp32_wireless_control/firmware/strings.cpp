#include "strings.h"
#include <WString.h>

// MIME type constants
const char* MIME_TYPE_TEXT = "text/plain";
const char* MIME_TYPE_HTML = "text/html";

// String lookup table structure
struct StringTable {
  const char* str;
  StringID id;
  Language lang;
};

// Parameter key lookup table structure
struct ParamKeyTable {
  const char* key;
  StringID id;
};

// Parameter key definitions
const ParamKeyTable paramKeys[] = {
    {.key = "trackingSpeed", .id = STR_TRACKING_SPEED},
    {.key = "speed", .id = STR_SPEED},
    {.key = "direction", .id = STR_DIRECTION},
    {.key = "exposure", .id = STR_EXPOSURE},
    {.key = "numExposures", .id = STR_NUM_EXPOSURES},
    {.key = "ditherEnabled", .id = STR_DITHER_ENABLED},
    {.key = "focalLength", .id = STR_FOCAL_LENGTH},
    {.key = "pixSize", .id = STR_PIXEL_SIZE},
    {.key = "disableTracking", .id = STR_DISABLE_TRACKING},
};

// All string definitions
const StringTable strings[] = {
    // English strings
    {.str = "Tracking ON", .id = STR_TRACKING_ON, .lang = EN},
    {.str = "Tracking OFF", .id = STR_TRACKING_OFF, .lang = EN},
    {.str = "Idle", .id = STR_IDLE, .lang = EN},
    {.str = "Slewing", .id = STR_SLEWING, .lang = EN},
    {.str = "Invalid Exposure Values!", .id = STR_INVALID_EXPOSURE, .lang = EN},
    {.str = "Invalid Dither Settings!", .id = STR_INVALID_DITHER, .lang = EN},
    {.str = "Capture Started", .id = STR_CAPTURE_ON, .lang = EN},
    {.str = "Capture Stopped", .id = STR_CAPTURE_OFF, .lang = EN},
    {.str = "Capture Already Running", .id = STR_CAPTURE_ALREADY_ON, .lang = EN},
    {.str = "Capture Already Stopped", .id = STR_CAPTURE_ALREADY_OFF, .lang = EN},
    {.str = "%d Captures Remaining...", .id = STR_CAPTURES_REMAINING, .lang = EN},

    // UI elements - English
    {.str = "OG Star Tracker Control Panel", .id = STR_TITLE, .lang = EN},
    {.str = "Tracking", .id = STR_TRACKING, .lang = EN},
    {.str = "Hemisphere", .id = STR_HEMISPHERE, .lang = EN},
    {.str = "North", .id = STR_NORTH, .lang = EN},
    {.str = "South", .id = STR_SOUTH, .lang = EN},
    {.str = "Tracking Rate", .id = STR_TRACKING_RATE, .lang = EN},
    {.str = "Sidereal", .id = STR_SIDEREAL, .lang = EN},
    {.str = "Solar", .id = STR_SOLAR, .lang = EN},
    {.str = "Lunar", .id = STR_LUNAR, .lang = EN},
    {.str = "Slew Control", .id = STR_SLEW_CONTROL, .lang = EN},
    {.str = "Speed Multiplier", .id = STR_SPEED_MULTIPLIER, .lang = EN},
    {.str = "Custom", .id = STR_CUSTOM, .lang = EN},
    {.str = "Custom Speed", .id = STR_CUSTOM_SPEED, .lang = EN},
    {.str = "Hold direction button to slew", .id = STR_SLEW_HINT, .lang = EN},
    {.str = "Slew Left", .id = STR_SLEW_LEFT, .lang = EN},
    {.str = "Slew Right", .id = STR_SLEW_RIGHT, .lang = EN},
    {.str = "Abort Slew", .id = STR_ABORT_SLEW, .lang = EN},
    {.str = "Photo Control", .id = STR_PHOTO_CONTROL, .lang = EN},
    {.str = "Exposure Length", .id = STR_EXPOSURE_LENGTH, .lang = EN},
    {.str = "in seconds (Ex. 30)", .id = STR_EXPOSURE_HINT, .lang = EN},
    {.str = "number of photos (Ex. 20)", .id = STR_NUM_EXPOSURES_HINT, .lang = EN},
    {.str = "Dither Settings", .id = STR_DITHER_SETTINGS, .lang = EN},
    {.str = "Enable Dithering", .id = STR_DITHER_ENABLE, .lang = EN},
    {.str = "in millimeters (Ex. 135)", .id = STR_FOCAL_LENGTH_HINT, .lang = EN},
    {.str = "in micrometers (Ex. 4.1)", .id = STR_PIXEL_SIZE_HINT, .lang = EN},
    {.str = "Start Capture", .id = STR_START_CAPTURE, .lang = EN},
    {.str = "Abort Capture", .id = STR_ABORT_CAPTURE, .lang = EN},
    {.str = "Status", .id = STR_STATUS, .lang = EN},
    {.str = "Ready", .id = STR_STATUS_MSG, .lang = EN},
    {.str = "Firmware Version", .id = STR_FIRMWARE_VERSION, .lang = EN},
    {.str = "ON", .id = STR_BTN_ON, .lang = EN},
    {.str = "OFF", .id = STR_BTN_OFF, .lang = EN},

    // Chinese strings
    {.str = "追踪开启", .id = STR_TRACKING_ON, .lang = CN},
    {.str = "追踪关闭", .id = STR_TRACKING_OFF, .lang = CN},
    {.str = "空闲", .id = STR_IDLE, .lang = CN},
    {.str = "快速调整中", .id = STR_SLEWING, .lang = CN},
    {.str = "无效的曝光参数!", .id = STR_INVALID_EXPOSURE, .lang = CN},
    {.str = "无效的抖动设置!", .id = STR_INVALID_DITHER, .lang = CN},
    {.str = "拍摄开始", .id = STR_CAPTURE_ON, .lang = CN},
    {.str = "拍摄停止", .id = STR_CAPTURE_OFF, .lang = CN},
    {.str = "拍摄已在进行中", .id = STR_CAPTURE_ALREADY_ON, .lang = CN},
    {.str = "拍摄已经停止", .id = STR_CAPTURE_ALREADY_OFF, .lang = CN},
    {.str = "剩余拍摄次数: %d", .id = STR_CAPTURES_REMAINING, .lang = CN},

    // UI elements - Chinese
    {.str = "OG 赤道仪控制面板", .id = STR_TITLE, .lang = CN},
    {.str = "追踪", .id = STR_TRACKING, .lang = CN},
    {.str = "半球", .id = STR_HEMISPHERE, .lang = CN},
    {.str = "北半球", .id = STR_NORTH, .lang = CN},
    {.str = "南半球", .id = STR_SOUTH, .lang = CN},
    {.str = "追踪速率", .id = STR_TRACKING_RATE, .lang = CN},
    {.str = "恒星速率", .id = STR_SIDEREAL, .lang = CN},
    {.str = "太阳速率", .id = STR_SOLAR, .lang = CN},
    {.str = "月球速率", .id = STR_LUNAR, .lang = CN},
    {.str = "快速调", .id = STR_SLEW_CONTROL, .lang = CN},
    {.str = "速度倍率", .id = STR_SPEED_MULTIPLIER, .lang = CN},
    {.str = "自定义", .id = STR_CUSTOM, .lang = CN},
    {.str = "自定义速度", .id = STR_CUSTOM_SPEED, .lang = CN},
    {.str = "按住方向按钮进行快速调整", .id = STR_SLEW_HINT, .lang = CN},
    {.str = "向左调整", .id = STR_SLEW_LEFT, .lang = CN},
    {.str = "向右调整", .id = STR_SLEW_RIGHT, .lang = CN},
    {.str = "停止调整", .id = STR_ABORT_SLEW, .lang = CN},
    {.str = "拍摄控制", .id = STR_PHOTO_CONTROL, .lang = CN},
    {.str = "曝光时长", .id = STR_EXPOSURE_LENGTH, .lang = CN},
    {.str = "单位：秒 (例如: 30)", .id = STR_EXPOSURE_HINT, .lang = CN},
    {.str = "拍摄张数 (例如: 20)", .id = STR_NUM_EXPOSURES_HINT, .lang = CN},
    {.str = "抖动设置", .id = STR_DITHER_SETTINGS, .lang = CN},
    {.str = "启用抖动", .id = STR_DITHER_ENABLE, .lang = CN},
    {.str = "单位：毫米 (例如: 135)", .id = STR_FOCAL_LENGTH_HINT, .lang = CN},
    {.str = "单位：微米 (例如: 4.1)", .id = STR_PIXEL_SIZE_HINT, .lang = CN},
    {.str = "开始拍摄", .id = STR_START_CAPTURE, .lang = CN},
    {.str = "停止拍摄", .id = STR_ABORT_CAPTURE, .lang = CN},
    {.str = "状态", .id = STR_STATUS, .lang = CN},
    {.str = "就绪", .id = STR_STATUS_MSG, .lang = CN},
    {.str = "固件版本", .id = STR_FIRMWARE_VERSION, .lang = CN},
    {.str = "开启", .id = STR_BTN_ON, .lang = CN},
    {.str = "关闭", .id = STR_BTN_OFF, .lang = CN},
    {.str = "中文", .id = STR_LANG_CN, .lang = EN},
    {.str = "中文", .id = STR_LANG_CN, .lang = CN},
};

// Function to get a string
const char* getString(StringID id, Language lang) {
  // Iterate through the lookup table to find the matching string
  for (const auto& entry : strings) {
    if (entry.id == id && entry.lang == lang) {
      return entry.str;
    }
  }
  // If the requested language version is not found, return the English version
  if (lang != EN) {
    return getString(id, EN);
  }
  return ""; // If nothing is found, return an empty string
}

// Function to get the parameter key name
const char* getParamKey(StringID id) {
  for (const auto& entry : paramKeys) {
    if (entry.id == id) {
      return entry.key;
    }
  }
  return "";
}

const char* LOGO_BASE64 = "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";

const char* html_template = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
    <meta charset='utf-8'>
    <meta name='viewport' content='width=device-width, initial-scale=1.0'>
    <title>%TITLE%</title>
    <style>
        body {
            background-color: #000000;
            text-align: center;
            font-family: system-ui;
            display: flex;
            padding: 10px;
            justify-content: center;
        }
        button {
            background-color: #FF0000;
            color: black;
            border: none;
            padding: 10px 10px;
            border-radius: 40px;
            text-align: center;
            text-decoration: none;
            display: inline-block;
            font-size: 14px;
            font-family: system-ui;
            margin-top: 10px;
            cursor: pointer;
            width: 50%;
            min-width: 120px;
            font-weight: bold;
        }
        select {
            font-size: 16px;
            padding: 5px;
            color: red;
            background-color: #000000;
            border: 2px solid #ff0000;
            border-radius: 8px;
            font-family: system-ui;
        }
        input[type='number'] {
            font-size: 16px;
            background-color: #000000;
            padding: 5px;
            font-family: system-ui;
            border: 2px solid #ff0000;
            color: red;
            border-radius: 8px;
            box-sizing: border-box;
        }
        label {
            display: inline-block;
            text-align: left;
            margin: 10px;
            font-size: 20px;
        }
        h1 {
            font-size: 26px;
            font-family: system-ui;
            color: red;
        }
        h2 {
            font-size: 24px;
            font-family: system-ui;
            color: red;
        }
        h3 {
            font-size: 16px;
            font-family: system-ui;
            color: #FF6B00;
        }
        h4 {
            font-size: 12px;
            font-family: system-ui;
            color: #FF6B00;
        }
        summary {
            font-weight: bold;
            font-family: system-ui;
            font-size: 16px;
            padding-top: 20px;
            color: #FF6B00;
        }
        .content {
            max-width: 450px;
            width: 100%;
            background-color: #000000;
        }
        .card {
            background-color: #000000;
            padding: 1px 16px 16px 16px;
            border-radius: 32px;
            margin-top: 20px;
            box-shadow: 0em 0em 1em rgb(255 0 0 / 100%);
        }
        .grid2 {
            display: flex;
            flex-direction: row;
            flex-wrap: wrap;
            color: #FF6B00;
            justify-content: center;
            align-items: center;
        }
        .grid2 > h3 {
            width: 50%;
            min-width: 120px;
        }
        .grid2 > select,
        .grid2 > input {
            width: 50%;
            min-width: 120px;
            height: 40px;
        }
        .grid2 > .checkbox {
            width: 50%;
            min-width: 120px;
            height: 20px;
            margin: 0px;
        }
        .header {
            display: flex;
            justify-content: center;
        }
        #logo {
            background-image: url('data:image/png;base64,%LOGO%');
            width: 150px;
            height: 120px;
            margin-top: 10px;
        }
        .button-group {
            padding-top: 10px;
            display: flex;
            flex-wrap: wrap;
            justify-content: center;
        }
        .right-separator {
            border-right: 2px solid #000000;
            -webkit-user-select: none;
            -moz-user-select: none;
            -ms-user-select: none;
            user-select: none;
            -webkit-touch-callout: none;
        }
        .left-separator {
            border-left: 2px solid #000000;
            -webkit-user-select: none;
            -moz-user-select: none;
            -ms-user-select: none;
            user-select: none;
            -webkit-touch-callout: none;
        }
        #status {
            font-size: 24px;
            margin: 0px;
            font-family: system-ui;
        }
    </style>
    <script>
        var lastClick = 0;
        var delay = 500;
        
        function sendRequest(url) {
            var xhr = new XMLHttpRequest();
            xhr.onreadystatechange = function() {
                if (this.readyState == 4 && this.status == 200) {
                    document.getElementById('status').innerHTML = this.responseText;
                }
            };
            xhr.open('GET', url, true);
            xhr.send();
        }

        setInterval(function() {
            sendRequest('/status');
        }, 5000);

        function requestFirmwareVersion() {
            var xhr = new XMLHttpRequest();
            xhr.onreadystatechange = function() {
                if (this.readyState == 4 && this.status == 200) {
                    document.getElementById('firmware').innerHTML = '%FIRMWARE_VERSION%: ' + this.responseText;
                }
            };
            xhr.open('GET', '/version', true);
            xhr.send();
        }

        function sendStartRequest(url) {
            var direction = document.getElementById('hemisphere-select').value;
            var trackingSpeed = document.getElementById('tracking-speed-select').value;
            var starturl = url + '?direction=' + direction + '&trackingSpeed=' + trackingSpeed;
            sendRequest(starturl);
        }

        function sendSlewStop() {
            lastClick = Date.now();
            sendRequest('/stopslew');
        }

        function sendSlewRequest(url) {
            if (lastClick >= (Date.now() - delay))
                return;
            lastClick = Date.now();
            var index = document.getElementById('slew-select').selectedIndex;
            if (index == 6) {
                var speed = document.getElementById('custom-speed').value;
            } else {
                var speed = document.getElementById('slew-select').value;
            }
            var slewurl = url + '?speed=' + speed;
            sendRequest(slewurl);
        }

        function checkCustom() {
            var index = document.getElementById('slew-select').selectedIndex;
            if (index == 6) {
                document.getElementById('custom-speed-text').style.display='block';
                document.getElementById('custom-speed').style.display='block';
            } else {
                document.getElementById('custom-speed-text').style.display='none';
                document.getElementById('custom-speed').style.display='none';
            }
        }

        function sendCaptureRequest() {
            var exposure = document.getElementById('exposure').value.trim();
            var numExposures = document.getElementById('num-exposures').value.trim();
            var focalLength = document.getElementById('focal_len').value.trim();
            var pixSize = Math.floor(parseFloat(document.getElementById('pixel_size').value.trim()) * 100);
            var disableTracking = document.getElementById('Disable_tracking_End').checked ? 1 : 0;
            var ditherEnabled = document.getElementById('dither_on').checked ? 1 : 0;
            var intervalometerUrl = '/start?exposure=' + exposure + 
                                  '&numExposures=' + numExposures + 
                                  '&focalLength=' + focalLength + 
                                  '&pixSize=' + pixSize + 
                                  '&ditherEnabled=' + ditherEnabled +
                                  '&disableTracking=' + disableTracking;
            sendRequest(intervalometerUrl);
        }

        function changeLanguage() {
            var lang = document.getElementById('language-select').value;
            var xhr = new XMLHttpRequest();
            xhr.onreadystatechange = function() {
                if (this.readyState == 4 && this.status == 200) {
                    window.location.reload(); // 强制刷新页面
                }
            };
            xhr.open('GET', '/setlang?lang=' + lang, true);
            xhr.send();
        }
    </script>
</head>
<body onload="requestFirmwareVersion()">
    <div class="content">
        <div class="language-selector">
            <select id="language-select" onchange="changeLanguage()">
                <option value="en" %lang_en%>English</option>
                <option value="cn" %lang_cn%>%LANG_CN%</option>
            </select>
        </div>
        <div class="header">
            <div id="logo"></div>
        </div>
        <h1>%TITLE%</h1>
        
        <div class="card">
            <h2>%TRACKING%</h2>
            <div class="grid2">
                <h3>%HEMISPHERE%</h3>
                <select id="hemisphere-select">
                    <option value="1" %north%>%NORTH%</option>
                    <option value="0" %south%>%SOUTH%</option>
                </select>
            </div>
            <div class="grid2">
                <h3>%TRACKING_RATE%</h3>
                <select id="tracking-speed-select">
                    <option value="0">%SIDEREAL%</option>
                    <option value="1">%SOLAR%</option>
                    <option value="2">%LUNAR%</option>
                </select>
            </div>
            <div class="button-group">
                <button class="right-separator" onclick="sendStartRequest('/on')">%BTN_ON%</button>
                <button class="left-separator" onclick="sendRequest('/off')">%BTN_OFF%</button>
            </div>
        </div>

        <div class="card">
            <h2>%SLEW_CONTROL%</h2>
            <div class="grid2">
                <h3>%SPEED_MULTIPLIER%</h3>
                <select id="slew-select" onchange="checkCustom()">
                    <option value="2">2 x %TRACKING_RATE%</option>
                    <option value="8">8</option>
                    <option value="50">50</option>
                    <option value="100">100</option>
                    <option value="200">200</option>
                    <option value="400">400</option>
                    <option value="custom">%CUSTOM%</option>
                </select>
            </div>
            <div class="grid2">
                <h3 id="custom-speed-text" style="display:none">%CUSTOM_SPEED%</h3>
                <input id="custom-speed" type="number" value="2" min="2" max="400" style="display:none"/>
            </div>
            <h4>%SLEW_HINT%</h4>
            <div class="button-group">
                <button class="right-separator" ontouchstart="sendSlewRequest('/left')" 
                        onmousedown="sendSlewRequest('/left')" 
                        ontouchend="sendSlewStop()" 
                        onmouseup="sendSlewStop()" 
                        onmouseleave="sendSlewStop()" 
                        ontouchcancel="sendSlewStop()">%SLEW_LEFT%</button>
                <button class="left-separator" ontouchstart="sendSlewRequest('/right')" 
                        onmousedown="sendSlewRequest('/right')" 
                        ontouchend="sendSlewStop()" 
                        onmouseup="sendSlewStop()" 
                        onmouseleave="sendSlewStop()" 
                        ontouchcancel="sendSlewStop()">%SLEW_RIGHT%</button>
            </div>
            <button onclick="sendSlewStop()">%ABORT_SLEW%</button>
        </div>

        <div class="card">
            <h2>%PHOTO_CONTROL%</h2>
            <div class="grid2">
                <h3>%EXPOSURE_LENGTH%</h3>
                <input type="number" id="exposure" placeholder="%EXPOSURE_HINT%">
            </div>
            <div class="grid2">
                <h3>%NUM_EXPOSURES%</h3>
                <input type="number" id="num-exposures" placeholder="%NUM_EXPOSURES_HINT%">
            </div>
            <div class="grid2">
                <h3>%DISABLE_TRACKING%</h3>
                <input class="checkbox" type="checkbox" id="Disable_tracking_End" %tracking%>
            </div>
            <details>
                <summary>%DITHER_SETTINGS%</summary>
                <div class="grid2">
                    <h3>%DITHER_ENABLE%</h3>
                    <input class="checkbox" type="checkbox" id="dither_on" %dither%>
                </div>
                <div class="grid2">
                    <h3>%FOCAL_LENGTH%</h3>
                    <input type="number" id="focal_len" placeholder="%FOCAL_LENGTH_HINT%" value="%focallen%">
                </div>
                <div class="grid2">
                    <h3>%PIXEL_SIZE%</h3>
                    <input type="number" id="pixel_size" placeholder="%PIXEL_SIZE_HINT%" value="%pixsize%" step="0.01">
                </div>
            </details>
            <div class="button-group">
                <button class="right-separator" onclick="sendCaptureRequest()">%START_CAPTURE%</button>
                <button class="left-separator" onclick="sendRequest('/abort')">%ABORT_CAPTURE%</button>
            </div>
        </div>

        <div class="card">
            <h2>%STATUS%</h2>
            <h3 id="status">%STATUS_MSG%</h3>
        </div>
        <h4 id="firmware">%FIRMWARE_VERSION%: </h4>
    </div>
</body>
</html>
)rawliteral";

String getHTML() {
  return String(html_template);
}

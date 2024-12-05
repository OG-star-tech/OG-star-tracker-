#include <WiFi.h>
#include <WebServer.h>
#include <DNSServer.h>
#include <string.h>
#include <esp_wifi.h>
#include <EEPROM.h>
#include "config.h"
#include "strings.h"
#include "template.h"

// try to update every time there is breaking change
const int firmware_version = 6;

// Set your Wi-Fi credentials
const byte DNS_PORT = 53;
const char* ssid = "OG Star Tracker";  //change to your SSID
const char* password = "password123";  //change to your password, must be 8+ characters
//If you are using AP mode, you can access the website using the below URL
const String website_name = "www.tracker.com";
const int dither_intensity = 5;
#define MIN_CUSTOM_SLEW_RATE 2

//Time b/w two rising edges should be 133.3333 ms
//66.666x2  ms
//sidereal rate = 0.00416 deg/s
//for 80Mhz APB (TIMER frequency)
#ifdef STEPPER_0_9
enum tracking_rate_state { TRACKING_SIDEREAL = 2659383,  //SIDEREAL (23h,56 min)
                           TRACKING_SOLAR = 2666666,     //SOLAR (24h)
                           TRACKING_LUNAR = 2723867 };   //LUNAR (24h, 31 min)
const int arcsec_per_step = 2;
#else  //stepper 1.8 deg
enum tracking_rate_state { TRACKING_SIDEREAL = 5318765,  //SIDEREAL (23h,56 min)
                           TRACKING_SOLAR = 5333333,     //SOLAR (24h)
                           TRACKING_LUNAR = 5447735 };   //LUNAR (24h, 31 min)
const int arcsec_per_step = 4;
#endif

volatile enum tracking_rate_state tracking_rate = TRACKING_SIDEREAL;  //default to sidereal rate

int slew_speed = 0, exposure_count = 0, exposure_duration = 0, dither_enabled = 0, focal_length = 0, pixel_size = 0, steps_per_10pixels = 0, direction = c_DIRECTION;
float arcsec_per_pixel = 0.0;
unsigned long blink_millis = 0;
uint64_t exposure_delay = 0;

//state variables
bool s_slew_active = false, s_tracking_active = true, s_capturing = false;  //change s_tracking_active state to false if you want tracker to be OFF on power-up
bool disable_tracking = false;
int exposures_taken = 0;
enum photo_control_state { ACTIVE,
                           DELAY,
                           DITHER,
                           INACTIVE };
volatile enum photo_control_state photo_control_status = INACTIVE;

float direction_left_bias = 0.5, direction_right_bias = 0.5;
int previous_direction = -1;

//2 bytes occupied by each int
//eeprom addresses
#define DITHER_ADDR 1
#define FOCAL_LEN_ADDR 3
#define PIXEL_SIZE_ADDR 5
#define DITHER_PIXELS 30  //how many pixels to dither

WebServer server(80);
DNSServer dnsServer;
hw_timer_t* timer_tracking = NULL;     //for tracking and slewing rate
hw_timer_t* timer_interval = NULL;     //for intervalometer control
hw_timer_t* timer_web_timeout = NULL;  //for webclient timeout control

// 添加当前语言变量
Language currentLanguage = EN;

// 处理语言切换请求
void handleSetLanguage() {
    String lang = server.arg("lang");
    if (lang == "cn") {
        currentLanguage = CN;
    } else if (lang == "en") {
        currentLanguage = EN;
    }
    
    // 保存语言选择到EEPROM
    EEPROM.write(LANG_EEPROM_ADDR, currentLanguage);
    EEPROM.commit();
    
    // 返回成功状态
    server.send(200, MIME_TYPE_TEXT, "OK");
}

void IRAM_ATTR timer_web_timeout_ISR() {
  handleSlewOff();
}

void IRAM_ATTR timer_tracking_ISR() {
  //tracking ISR
  digitalWrite(AXIS1_STEP, !digitalRead(AXIS1_STEP));  //toggle step pin at required frequency
}

void IRAM_ATTR timer_interval_ISR() {
  Serial.println("----");
  //intervalometer ISR
  switch (photo_control_status) {
    case ACTIVE:
      exposures_taken++;
      Serial.println("exposures_taken = " + String(exposures_taken) + "/" + String(exposure_count));
      if (exposure_count == exposures_taken) {
        Serial.println("State: Stopping intervalometer");
        // no more images to capture, stop
        disableIntervalometer();
        exposure_count = 0;
        s_capturing = false;
        photo_control_status = INACTIVE;

        if (disable_tracking) {
          s_tracking_active = false;
          timerAlarmDisable(timer_tracking);
        }
      } else if (exposures_taken % 3 == 0 && dither_enabled) {
        Serial.println("State: Dithering");
        // user has active dithering and this is %3 image, stop capturing and run dither routine
        photo_control_status = DITHER;
        stopCapture();
        timerStop(timer_interval);  //pause the timer, wait for dither to finish in main loop
      } else {
        Serial.println("State: Wait for next shot");
        // run normally
        timerWrite(timer_interval, exposure_delay);
        stopCapture();
        photo_control_status = DELAY;
      }
      break;
    case DELAY:
      Serial.println("State: Taking shot");
      timerWrite(timer_interval, 0);
      startCapture();
      photo_control_status = ACTIVE;
      break;
  }
}

// Handle requests to the root URL ("/")
void handleRoot() {
    Template tmpl(html_template);
    
    // 添加logo替换
    tmpl.replace("LOGO", LOGO_BASE64)
        // 在其他替换之前添加语言选择器的状态
        .replaceSelected("lang_en", currentLanguage == EN)
        .replaceSelected("lang_cn", currentLanguage == CN)
        .replace("TITLE", STR_TITLE, currentLanguage)
        .replace("LANG_CN", STR_LANG_CN, currentLanguage)
        .replace("TRACKING", STR_TRACKING, currentLanguage)
        .replace("HEMISPHERE", STR_HEMISPHERE, currentLanguage)
        .replace("NORTH", STR_NORTH, currentLanguage)
        .replace("SOUTH", STR_SOUTH, currentLanguage)
        .replace("TRACKING_RATE", STR_TRACKING_RATE, currentLanguage)
        .replace("SIDEREAL", STR_SIDEREAL, currentLanguage)
        .replace("SOLAR", STR_SOLAR, currentLanguage)
        .replace("LUNAR", STR_LUNAR, currentLanguage)
        .replace("SLEW_CONTROL", STR_SLEW_CONTROL, currentLanguage)
        .replace("SPEED_MULTIPLIER", STR_SPEED_MULTIPLIER, currentLanguage)
        .replace("CUSTOM", STR_CUSTOM, currentLanguage)
        .replace("CUSTOM_SPEED", STR_CUSTOM_SPEED, currentLanguage)
        .replace("SLEW_HINT", STR_SLEW_HINT, currentLanguage)
        .replace("SLEW_LEFT", STR_SLEW_LEFT, currentLanguage)
        .replace("SLEW_RIGHT", STR_SLEW_RIGHT, currentLanguage)
        .replace("ABORT_SLEW", STR_ABORT_SLEW, currentLanguage)
        .replace("PHOTO_CONTROL", STR_PHOTO_CONTROL, currentLanguage)
        .replace("EXPOSURE_LENGTH", STR_EXPOSURE_LENGTH, currentLanguage)
        .replace("EXPOSURE_HINT", STR_EXPOSURE_HINT, currentLanguage)
        .replace("NUM_EXPOSURES", STR_NUM_EXPOSURES, currentLanguage)
        .replace("NUM_EXPOSURES_HINT", STR_NUM_EXPOSURES_HINT, currentLanguage)
        .replace("DISABLE_TRACKING", STR_DISABLE_TRACKING, currentLanguage)
        .replace("DITHER_SETTINGS", STR_DITHER_SETTINGS, currentLanguage)
        .replace("DITHER_ENABLE", STR_DITHER_ENABLE, currentLanguage)
        .replace("FOCAL_LENGTH", STR_FOCAL_LENGTH, currentLanguage)
        .replace("FOCAL_LENGTH_HINT", STR_FOCAL_LENGTH_HINT, currentLanguage)
        .replace("PIXEL_SIZE", STR_PIXEL_SIZE, currentLanguage)
        .replace("PIXEL_SIZE_HINT", STR_PIXEL_SIZE_HINT, currentLanguage)
        .replace("START_CAPTURE", STR_START_CAPTURE, currentLanguage)
        .replace("ABORT_CAPTURE", STR_ABORT_CAPTURE, currentLanguage)
        .replace("STATUS", STR_STATUS, currentLanguage)
        .replace("STATUS_MSG", STR_STATUS_MSG, currentLanguage)
        .replace("FIRMWARE_VERSION", STR_FIRMWARE_VERSION, currentLanguage)
        .replace("BTN_ON", STR_BTN_ON, currentLanguage)
        .replace("BTN_OFF", STR_BTN_OFF, currentLanguage)
        
        // 替换状态相关的占位符
        .replaceSelected("north", direction == 1)
        .replaceSelected("south", direction == 0)
        .replaceChecked("dither", dither_enabled)
        .replaceChecked("tracking", disable_tracking)
        .replace("focallen", focal_length)
        .replace("pixsize", (float)pixel_size / 100.0, 2);
        
    server.send(200, MIME_TYPE_HTML, tmpl.toString());
}

void handleOn() {
    int tracking_speed = server.arg(getParamKey(STR_TRACKING_SPEED)).toInt();
    direction = server.arg(getParamKey(STR_DIRECTION)).toInt();
    s_tracking_active = true;
    initTracking();
}

void handleOff() {
    server.send(200, MIME_TYPE_TEXT, getString(STR_TRACKING_OFF, currentLanguage));
}

void handleLeft() {
    if (!s_slew_active) {  //if slew is not active - needed for ipad (passes multiple touchon events)
        slew_speed = server.arg(getParamKey(STR_SPEED)).toInt();
        //limit custom slew speed to 2-400
        slew_speed = slew_speed > MAX_CUSTOM_SLEW_RATE ? MAX_CUSTOM_SLEW_RATE : slew_speed < MIN_CUSTOM_SLEW_RATE ? MIN_CUSTOM_SLEW_RATE
                                                                                                              : slew_speed;
        initSlew(0);
    }
}

void handleRight() {
    if (!s_slew_active) {  //if slew is not active - needed for ipad (passes multiple touchon events)
        slew_speed = server.arg(getParamKey(STR_SPEED)).toInt();
        //limit custom slew speed to 2-400
        slew_speed = slew_speed > MAX_CUSTOM_SLEW_RATE ? MAX_CUSTOM_SLEW_RATE : slew_speed < MIN_CUSTOM_SLEW_RATE ? MIN_CUSTOM_SLEW_RATE
                                                                                                              : slew_speed;
        initSlew(1);  //reverse direction
    }
}

void handleSlewOff() {
    if (s_slew_active) {  //if slew is active needed for ipad (passes multiple touchoff events)
        s_slew_active = false;
        timerAlarmDisable(timer_web_timeout);
        timerDetachInterrupt(timer_web_timeout);
        timerEnd(timer_web_timeout);
        timerAlarmDisable(timer_tracking);
        initTracking();
    }
}

void handleStartCapture() {
    if (photo_control_status == INACTIVE) {
        exposure_duration = server.arg(getParamKey(STR_EXPOSURE)).toInt();
        exposure_count = server.arg(getParamKey(STR_NUM_EXPOSURES)).toInt();
        dither_enabled = server.arg(getParamKey(STR_DITHER_ENABLED)).toInt();
        focal_length = server.arg(getParamKey(STR_FOCAL_LENGTH)).toInt();
        pixel_size = server.arg(getParamKey(STR_PIXEL_SIZE)).toInt();
        disable_tracking = server.arg(getParamKey(STR_DISABLE_TRACKING)).toInt();

        exposures_taken = 0;

        if ((exposure_duration == 0 || exposure_count == 0)) {
            server.send(200, MIME_TYPE_TEXT, getString(STR_INVALID_EXPOSURE, currentLanguage));
            return;
        }

        if (dither_enabled && (focal_length == 0 || pixel_size == 0)) {
            server.send(200, MIME_TYPE_TEXT, getString(STR_INVALID_DITHER, currentLanguage));
            return;
        }

        updateEEPROM(dither_enabled, focal_length, pixel_size);
        arcsec_per_pixel = (((float)pixel_size / 100.0) / focal_length) * 206.265;        //div pixel size by 100 since we multiplied it by 100 in html page
        steps_per_10pixels = (int)(((arcsec_per_pixel * 10.0) / arcsec_per_step) + 0.5);  //add 0.5 to round up float to nearest int while casting
        Serial.println("steps per 10px: ");
        Serial.println(steps_per_10pixels);

        s_capturing = true;
        photo_control_status = ACTIVE;
        exposure_delay = ((exposure_duration - 3) * 2000);  // 3 sec delay
        initIntervalometer();
        server.send(200, MIME_TYPE_TEXT, getString(STR_CAPTURE_ON, currentLanguage));
    } else {
        server.send(200, MIME_TYPE_TEXT, getString(STR_CAPTURE_ALREADY_ON, currentLanguage));
    }
}

void handleAbortCapture() {
    if (photo_control_status == INACTIVE) {
        server.send(200, MIME_TYPE_TEXT, getString(STR_CAPTURE_ALREADY_OFF, currentLanguage));
        return;
    }

    disableIntervalometer();
    exposure_count = 0;
    exposure_duration = 0;
    photo_control_status = INACTIVE;
    server.send(200, MIME_TYPE_TEXT, getString(STR_CAPTURE_OFF, currentLanguage));
    s_capturing = false;
}

void handleStatusRequest() {
    if (s_slew_active) {
        server.send(200, MIME_TYPE_TEXT, getString(STR_SLEWING, currentLanguage));
        return;
    }
    if (!s_tracking_active) {
        server.send(200, MIME_TYPE_TEXT, getString(STR_IDLE, currentLanguage));
        return;
    }
    if (photo_control_status != INACTIVE) {
        char status[60];
        sprintf(status, getString(STR_CAPTURES_REMAINING, currentLanguage), exposure_count - exposures_taken);
        server.send(200, MIME_TYPE_TEXT, status);
        return;
    }

    server.send(204, MIME_TYPE_TEXT, "dummy");

    // TODO add detection for capturing
}

void handleVersion() {
    server.send(200, MIME_TYPE_TEXT, (String)firmware_version);
}

void writeEEPROM(int address, int value) {
    byte high = value >> 8;
    byte low = value & 0xFF;
    EEPROM.write(address, high);
    EEPROM.write(address + 1, low);
}

int readEEPROM(int address) {
    byte high = EEPROM.read(address);
    byte low = EEPROM.read(address + 1);
    return ((high << 8) + low);
}

void updateEEPROM(int dither, int focal_len, int pix_size) {
    if (readEEPROM(DITHER_ADDR) != dither) {
        writeEEPROM(DITHER_ADDR, dither);
        //Serial.println("dither updated");
    }
    if (readEEPROM(FOCAL_LEN_ADDR) != focal_len) {
        writeEEPROM(FOCAL_LEN_ADDR, focal_len);
        //Serial.println("focal length updated");
    }
    if (readEEPROM(PIXEL_SIZE_ADDR) != pix_size) {
        writeEEPROM(PIXEL_SIZE_ADDR, pix_size);
        //Serial.println("pix size updated");
    }
    EEPROM.commit();
}

void setMicrostep(int microstep) {
    switch (microstep) {
        case 8:
            digitalWrite(MS1, LOW);
            digitalWrite(MS2, LOW);
            break;
        case 16:
            digitalWrite(MS1, HIGH);
            digitalWrite(MS2, HIGH);
            break;
        case 32:
            digitalWrite(MS1, HIGH);
            digitalWrite(MS2, LOW);
            break;
        case 64:
            digitalWrite(MS1, LOW);
            digitalWrite(MS2, HIGH);
            break;
    }
}

void initSlew(int dir) {
    s_slew_active = true;
    timer_web_timeout = timerBegin(2, 40000, true);
    timerAttachInterrupt(timer_web_timeout, &timer_web_timeout_ISR, true);
    timerAlarmWrite(timer_web_timeout, 12000, true);  //12000 = 6 secs timeout, send status 5 sec poll (reset on poll)
    timerAlarmEnable(timer_web_timeout);
    server.send(200, MIME_TYPE_TEXT, getString(STR_SLEWING, currentLanguage));
    digitalWrite(AXIS1_DIR, dir);  //set slew direction
    timerAlarmDisable(timer_tracking);
    setMicrostep(8);
    timerAlarmWrite(timer_tracking, (2 * tracking_rate) / slew_speed, true);  //2*tracking rate (16 mstep vs 8) / slew_speed = multiple of tracking rate
    timerAlarmEnable(timer_tracking);
}

void initTracking() {
    digitalWrite(AXIS1_DIR, direction);
    setMicrostep(16);
    timerAlarmWrite(timer_tracking, tracking_rate, true);
    if (s_tracking_active) {
        timerAlarmEnable(timer_tracking);
        server.send(200, MIME_TYPE_TEXT, getString(STR_TRACKING_ON, currentLanguage));
    } else {
        timerAlarmDisable(timer_tracking);
        server.send(200, MIME_TYPE_TEXT, getString(STR_TRACKING_OFF, currentLanguage));
    }
}

void initIntervalometer() {
    timer_interval = timerBegin(1, 40000, true);
    timerAttachInterrupt(timer_interval, &timer_interval_ISR, true);
    timerAlarmWrite(timer_interval, (exposure_duration * 2000), true);  //2000 because prescaler cant be more than 16bit, = 1sec ISR freq
    timerAlarmEnable(timer_interval);
    startCapture();
}

void disableIntervalometer() {
    stopCapture();
    timerAlarmDisable(timer_interval);
    timerDetachInterrupt(timer_interval);
    timerEnd(timer_interval);
}

void stopCapture() {
    digitalWrite(INTERV_PIN, LOW);
}

void startCapture() {
    digitalWrite(INTERV_PIN, HIGH);
}

void ditherRoutine() {
    int i = 0, j = 0;
    timerAlarmDisable(timer_tracking);
    int random_direction = biased_random_direction(previous_direction);
    previous_direction = random_direction;
    digitalWrite(AXIS1_DIR, random_direction);  //dither in a random direction
    delay(500);

    for (i = 0; i < dither_intensity; i++) {
        for (j = 0; j < steps_per_10pixels; j++) {
            digitalWrite(AXIS1_STEP, !digitalRead(AXIS1_STEP));
            delay(10);
            digitalWrite(AXIS1_STEP, !digitalRead(AXIS1_STEP));
            delay(10);
        }
    }

    delay(1000);
    initTracking();
    delay(3000);  //settling time after dither
}

void setup() {
    Serial.begin(115200);
    EEPROM.begin(512);  //SIZE = 6 bytes, 2 bytes for each variable
    //fetch values from EEPROM
    dither_enabled = readEEPROM(DITHER_ADDR);
    focal_length = readEEPROM(FOCAL_LEN_ADDR);
    pixel_size = readEEPROM(PIXEL_SIZE_ADDR);

#ifdef AP
    WiFi.mode(WIFI_MODE_AP);
    WiFi.softAP(ssid, password);
    delay(500);
    Serial.println("Creating Wifi Network");

    //ANDROID 10 WORKAROUND==================================================
    //set new WiFi configurations
    WiFi.disconnect();
    Serial.println("reconfig WiFi...");
    /*Stop wifi to change config parameters*/
    esp_wifi_stop();
    esp_wifi_deinit();
    /*Disabling AMPDU RX is necessary for Android 10 support*/
    wifi_init_config_t my_config = WIFI_INIT_CONFIG_DEFAULT();  //We use the default config ...
    my_config.ampdu_rx_enable = 0;                              //... and modify only what we want.
    Serial.println("WiFi: Disabled AMPDU...");
    esp_wifi_init(&my_config);  //set the new config = "Disable AMPDU"
    esp_wifi_start();           //Restart WiFi
    delay(500);
    //ANDROID 10 WORKAROUND==================================================
#else
    WiFi.mode(WIFI_MODE_STA);  // Set ESP32 in station mode
    WiFi.begin(ssid, password);
    Serial.println("Connecting to Network in STA mode");
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.print(".");
    }
#endif
    dnsServer.setTTL(300);
    dnsServer.setErrorReplyCode(DNSReplyCode::ServerFailure);
    dnsServer.start(DNS_PORT, website_name, WiFi.softAPIP());

    server.on("/", HTTP_GET, handleRoot);
    server.on("/on", HTTP_GET, handleOn);
    server.on("/off", HTTP_GET, handleOff);
    server.on("/left", HTTP_GET, handleLeft);
    server.on("/right", HTTP_GET, handleRight);
    server.on("/stopslew", HTTP_GET, handleSlewOff);
    server.on("/start", HTTP_GET, handleStartCapture);
    server.on("/abort", HTTP_GET, handleAbortCapture);
    server.on("/status", HTTP_GET, handleStatusRequest);
    server.on("/version", HTTP_GET, handleVersion);

    // 从EEPROM读取语言设置
    currentLanguage = (Language)EEPROM.read(LANG_EEPROM_ADDR);
    if (currentLanguage >= LANG_COUNT) {
        currentLanguage = EN;  // 如果读取到无效值，默认使用英语
        EEPROM.write(LANG_EEPROM_ADDR, currentLanguage);
        EEPROM.commit();
    }
    
    // 添加语言切换路由
    server.on("/setlang", HTTP_GET, handleSetLanguage);

    // Start the server
    server.begin();

#ifdef AP
    Serial.println(WiFi.softAPIP());
#else
    Serial.println(WiFi.localIP());
#endif
    pinMode(INTERV_PIN, OUTPUT);
    pinMode(STATUS_LED, OUTPUT);
    pinMode(AXIS1_STEP, OUTPUT);
    pinMode(AXIS1_DIR, OUTPUT);
    pinMode(EN12_n, OUTPUT);
    pinMode(MS1, OUTPUT);
    pinMode(MS2, OUTPUT);
    digitalWrite(AXIS1_STEP, LOW);
    digitalWrite(EN12_n, LOW);

    timer_tracking = timerBegin(0, 2, true);
    timerAttachInterrupt(timer_tracking, &timer_tracking_ISR, true);
    initTracking();
}

void loop() {
    if (s_slew_active) {
        //blink status led if mount is in slew mode
        if (millis() - blink_millis >= 150) {
            digitalWrite(STATUS_LED, !digitalRead(STATUS_LED));
            blink_millis = millis();
        }
    } else {
        //turn on status led if sidereal tracking is ON
        digitalWrite(STATUS_LED, (s_tracking_active == true));
    }

    if (photo_control_status == DITHER) {
        disableIntervalometer();
        ditherRoutine();
        photo_control_status = ACTIVE;
        initIntervalometer();
    }
    server.handleClient();
    dnsServer.processNextRequest();
}

int biased_random_direction(int previous_direction) {
  // Adjust probabilities based on previous selection
  if (previous_direction == 0) {
    direction_left_bias = 0.45;   // Lower probability for 0
    direction_right_bias = 0.55;  // Higher probability for 1
  }
  if (previous_direction == 1) {
    direction_left_bias = 0.55;   // Higher probability for 0
    direction_right_bias = 0.45;  // Lower probability for 1
  }

  float rand_val = random(100) / 100.0;  // random number between 0.00 and 0.99

  if (rand_val < direction_left_bias) {
    return 0;
  } else {
    return 1;
  }
}
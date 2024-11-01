#include <WiFi.h>
#include <WebServer.h>
#include <DNSServer.h>
#include <string.h>
#include <esp_wifi.h>
#include <EEPROM.h>
#include "config.h"
#include "strings.h"

// try to update every time there is breaking change
const int firmware_version = 5;

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

void IRAM_ATTR timer_web_timeout_ISR() {
  handleSlewOff();
}

void IRAM_ATTR timer_tracking_ISR() {
  //tracking ISR
  digitalWrite(AXIS1_STEP, !digitalRead(AXIS1_STEP));  //toggle step pin at required frequency
}

void IRAM_ATTR timer_interval_ISR() {
  //intervalometer ISR
  switch (photo_control_status) {
    case ACTIVE:
      if (exposure_count - 1 == exposures_taken) {
        // no more images to capture, stop
        disableIntervalometer();
        exposure_count = 0;
        exposure_duration = 0;
        s_capturing = false;
        photo_control_status = INACTIVE;

        if (disable_tracking) {
          handleOff();
        }
      } else if (exposure_count % 3 == 0 && dither_enabled) {
        // user has active dithering and this is %3 image, stop capturing and run dither routine
        photo_control_status = DITHER;
        stopCapture();
        timerStop(timer_interval);  //pause the timer, wait for dither to finish in main loop
      } else {
        // run normally
        timerWrite(timer_interval, exposure_delay);
        stopCapture();
        photo_control_status = DELAY;
        exposures_taken++;
      }
      break;
    case DELAY:
      timerWrite(timer_interval, 0);
      startCapture();
      photo_control_status = ACTIVE;
      break;
  }
}

// Handle requests to the root URL ("/")
void handleRoot() {
  String formattedHtmlPage = String(html);
  formattedHtmlPage.replace("%north%", (direction ? "selected" : ""));
  formattedHtmlPage.replace("%south%", (!direction ? "selected" : ""));
  formattedHtmlPage.replace("%dither%", (dither_enabled ? "checked" : ""));
  formattedHtmlPage.replace("%focallen%", String(focal_length).c_str());
  formattedHtmlPage.replace("%pixsize%", String((float)pixel_size / 100, 2).c_str());
  server.send(200, MIME_TYPE_HTML, formattedHtmlPage);
}

void handleOn() {
  int tracking_speed = server.arg(TRACKING_SPEED).toInt();
  switch (tracking_speed) {
    case 0:  //sidereal rate
      tracking_rate = TRACKING_SIDEREAL;
      break;
    case 1:  //solar rate
      tracking_rate = TRACKING_SOLAR;
      break;
    case 2:  //lunar rate
      tracking_rate = TRACKING_LUNAR;
      break;
    default:
      tracking_rate = TRACKING_SIDEREAL;
      break;
  }
  direction = server.arg(DIRECTION).toInt();
  s_tracking_active = true;
  initTracking();
}

void handleOff() {
  s_tracking_active = false;
  timerAlarmDisable(timer_tracking);
  server.send(200, MIME_TYPE_TEXT, TRACKING_OFF);
}

void handleLeft() {
  if (!s_slew_active) {  //if slew is not active - needed for ipad (passes multiple touchon events)
    slew_speed = server.arg(SPEED).toInt();
    //limit custom slew speed to 2-400
    slew_speed = slew_speed > MAX_CUSTOM_SLEW_RATE ? MAX_CUSTOM_SLEW_RATE : slew_speed < MIN_CUSTOM_SLEW_RATE ? MIN_CUSTOM_SLEW_RATE : slew_speed;
    initSlew(0);
  }
}

void handleRight() {
  if (!s_slew_active) {  //if slew is not active - needed for ipad (passes multiple touchon events)
    slew_speed = server.arg(SPEED).toInt();
    //limit custom slew speed to 2-400
    slew_speed = slew_speed > MAX_CUSTOM_SLEW_RATE ? MAX_CUSTOM_SLEW_RATE : slew_speed < MIN_CUSTOM_SLEW_RATE ? MIN_CUSTOM_SLEW_RATE : slew_speed;
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
    exposure_duration = server.arg(EXPOSURE).toInt();
    exposure_count = server.arg(NUM_EXPOSURES).toInt();
    dither_enabled = server.arg(DITHER_ENABLED).toInt();
    focal_length = server.arg(FOCAL_LENGTH).toInt();
    pixel_size = server.arg(PIXEL_SIZE).toInt();
    disable_tracking = server.arg(DISABLE_TRACKING_ON_FINISH).toInt();
    
    exposures_taken = 0;

    if ((exposure_duration == 0 || exposure_count == 0)) {
      server.send(200, MIME_TYPE_TEXT, INVALID_EXPOSURE_VALUES);
      return;
    }

    if (dither_enabled && (focal_length == 0 || pixel_size == 0)) {
      server.send(200, MIME_TYPE_TEXT, INVALID_DITHER_VALUES);
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
    server.send(200, MIME_TYPE_TEXT, CAPTURE_ON);
  } else {
    server.send(200, MIME_TYPE_TEXT, CAPTURE_ALREADY_ON);
  }
}

void handleAbortCapture() {
  if (photo_control_status == INACTIVE) {
    server.send(200, MIME_TYPE_TEXT, CAPTURE_ALREADY_OFF);
    return;
  }

  disableIntervalometer();
  exposure_count = 0;
  exposure_duration = 0;
  photo_control_status = INACTIVE;
  server.send(200, MIME_TYPE_TEXT, CAPTURE_OFF);
  s_capturing = false;
}

void handleStatusRequest() {
  if (s_slew_active) {
    timerWrite(timer_web_timeout, 0);  //reset timer while slew on, prove still connected to web/app
    server.send(200, MIME_TYPE_TEXT, SLEWING);
  }
  if (photo_control_status != INACTIVE) {
    char status[60];
    sprintf(status, CAPTURES_REMAINING, exposure_count - exposures_taken);
    server.send(200, MIME_TYPE_TEXT, status);
    return;
  }

  if (!s_tracking_active && photo_control_status == INACTIVE) {
    server.send(200, MIME_TYPE_TEXT, IDLE);
    return;
  }

  if (s_tracking_active) {
    server.send(200, MIME_TYPE_TEXT, TRACKING_ON);
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
  server.send(200, MIME_TYPE_TEXT, SLEWING);
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
    server.send(200, MIME_TYPE_TEXT, TRACKING_ON);
  } else {
    timerAlarmDisable(timer_tracking);
    server.send(200, MIME_TYPE_TEXT, TRACKING_OFF);
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
  Serial.println("Dither rndm direction:");
  Serial.println(random_direction);

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

// when tracker moves left, next time its 5% higher chance tracked will move right
// with this tracker should keep in the middle in average
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

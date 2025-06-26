#include <ArduinoJson.h>
#include <ESPmDNS.h>
#include <ErriezSerialTerminal.h>
#include <WebServer.h>
#include <WiFi.h>
#include <esp_wifi.h>
#include <freertos/FreeRTOS.h>
#include <string.h>

#include "axis.h"
#include "commands.h"
#include "common_strings.h"
#include "config.h"
#include "hardwaretimer.h"
#include "index_html.h"
#include "intervalometer.h"
#include "uart.h"
#include "web_languages.h"
#include "website_strings.h"

SerialTerminal term(CLI_NEWLINE_CHAR, CLI_DELIMITER_CHAR);
WebServer server(WEBSERVER_PORT);
Languages language = EN;

void uartTask(void* pvParameters);
void consoleTask(void* pvParameters);
void webserverTask(void* pvParameters);
void intervalometerTask(void* pvParameters);

// Handle requests to the root URL ("/")
void handleRoot()
{
    String htmlString = html_content;
    for (int placeholder = 0; placeholder < numberOfHTMLStrings; placeholder++)
    {
        htmlString.replace(HTMLplaceHolders[placeholder],
                           languageHTMLStrings[language][placeholder]);
    }
    char buffer[100];
    String selectString = "";
    for (int lang = 0; lang < LANG_COUNT; lang++)
    {
        if (lang == language)
        {
            sprintf(buffer, "<option value=\"%u\" selected>%s</option>\n", lang,
                    languageNames[language][lang]);
        }
        else
        {
            sprintf(buffer, "<option value=\"%u\">%s</option>\n", lang,
                    languageNames[language][lang]);
        }
        // print_out(buffer);
        selectString.concat(buffer);
        buffer[0] = '\0';
    }
    htmlString.replace("%LANG_SELECT%", selectString);
    server.send(200, MIME_TYPE_HTML, htmlString);
}

void handleOn()
{
    int tracking_speed = server.arg(TRACKING_SPEED).toInt();
    trackingRateS rate;
    switch (tracking_speed)
    {
        case 0: // sidereal rate
            rate = TRACKING_SIDEREAL;
            break;
        case 1: // solar rate
            rate = TRACKING_SOLAR;
            break;
        case 2: // lunar rate
            rate = TRACKING_LUNAR;
            break;
        default:
            rate = TRACKING_SIDEREAL;
            break;
    }
    int direction = server.arg(DIRECTION).toInt();
    ra_axis.startTracking(rate, direction);

    if (intervalometer.currentErrorMessage == ErrorMessage::ERR_MSG_NONE)
        server.send(200, MIME_TYPE_TEXT, languageMessageStrings[language][MSG_TRACKING_ON]);
    else
        server.send(200, MIME_TYPE_TEXT,
                    languageErrorMessageStrings[language][intervalometer.currentErrorMessage]);
}

void handleOff()
{
    ra_axis.stopTracking();
    server.send(200, MIME_TYPE_TEXT, languageMessageStrings[language][MSG_TRACKING_OFF]);
}

void handleSlewRequest()
{
    if (!ra_axis.slewActive)
    { // if slew is not active - needed for ipad (passes multiple touchon events)
        int slew_speed = server.arg(SPEED).toInt();
        int direction = server.arg(DIRECTION).toInt();
        // limit custom slew speed to 2-400
        slew_speed = slew_speed > MAX_CUSTOM_SLEW_RATE   ? MAX_CUSTOM_SLEW_RATE
                     : slew_speed < MIN_CUSTOM_SLEW_RATE ? MIN_CUSTOM_SLEW_RATE
                                                         : slew_speed;
        ra_axis.startSlew((2 * ra_axis.trackingRate) / slew_speed, direction);
        server.send(200, MIME_TYPE_TEXT, languageMessageStrings[language][MSG_SLEWING]);
    }
}

void handleSlewOff()
{
    if (ra_axis.slewActive)
    { // if slew is active needed for ipad (passes multiple touchoff events)
        ra_axis.stopSlew();
    }
    server.send(200, MIME_TYPE_TEXT, languageMessageStrings[language][MSG_SLEW_CANCELLED]);
}

void handleSetLanguage()
{
    int lang = server.arg("lang").toInt();
    language = static_cast<Languages>(lang);
    EEPROM.write(LANG_EEPROM_ADDR, language);
    EEPROM.commit();
    server.send(200, MIME_TYPE_TEXT, languageMessageStrings[language][MSG_OK]);
}

void handleSetCurrent()
{
    if (!intervalometer.intervalometerActive)
    {
        // Reset the current error message
        intervalometer.currentErrorMessage = ERR_MSG_NONE;

        int modeInt = server.arg(CAPTURE_MODE).toInt();
        if (modeInt < 0 || modeInt >= Intervalometer::Mode::MAX_MODES)
        {
            intervalometer.currentErrorMessage = ERR_MSG_INVALID_CAPTURE_MODE;
            return;
        }
        Intervalometer::Mode captureMode = static_cast<Intervalometer::Mode>(modeInt);
        intervalometer.currentSettings.mode = captureMode;

        int exposureTime = server.arg(EXPOSURE_TIME).toInt();
        if (exposureTime <= 0)
        {
            intervalometer.currentErrorMessage = ERR_MSG_INVALID_EXPOSURE_LENGTH;
            return;
        }
        intervalometer.currentSettings.exposureTime = exposureTime;

        int exposures = server.arg(EXPOSURES).toInt();
        if (exposures <= 0)
        {
            intervalometer.currentErrorMessage = ERR_MSG_INVALID_EXPOSURE_AMOUNT;
            return;
        }
        intervalometer.currentSettings.exposures = exposures;

        int preDelay = server.arg(PREDELAY).toInt();
        if (preDelay < 0)
        {
            intervalometer.currentErrorMessage = ERR_MSG_INVALID_PREDELAY_TIME;
            return;
        }
        else if (preDelay == 0)
            preDelay = 5;
        intervalometer.currentSettings.preDelay = preDelay;

        int delayTime = server.arg(DELAY).toInt();
        if (delayTime < 0)
        {
            intervalometer.currentErrorMessage = ERR_MSG_INVALID_DELAY_TIME;
            return;
        }
        intervalometer.currentSettings.delayTime = delayTime;

        int frames = server.arg(FRAMES).toInt();
        if (frames <= 0)
        {
            intervalometer.currentErrorMessage = ERR_MSG_INVALID_FRAME_AMOUNT;
            return;
        }
        intervalometer.currentSettings.frames = frames;

        float panAngle = server.arg(PAN_ANGLE).toFloat() / 100;
        if (panAngle < 0.0)
        {
            intervalometer.currentErrorMessage = ERR_MSG_INVALID_PAN_ANGLE;
            return;
        }
        intervalometer.currentSettings.panAngle = panAngle;

        int panDirection = server.arg(PAN_DIRECTION).toInt();
        if (panDirection < 0 || panDirection > 1)
        {
            intervalometer.currentErrorMessage = ERR_MSG_INVALID_PAN_DIRECTION;
            return;
        }
        intervalometer.currentSettings.panDirection = panDirection;

        int enableTracking = server.arg(ENABLE_TRACKING).toInt();
        if (enableTracking < 0 || enableTracking > 1)
        {
            intervalometer.currentErrorMessage = ERR_MSG_INVALID_ENABLE_TRACKING_VALUE;
            return;
        }
        intervalometer.currentSettings.enableTracking = enableTracking;

        int dither = server.arg(DITHER_CHOICE).toInt();
        if (dither < 0 || dither > 1)
        {
            intervalometer.currentErrorMessage = ERR_MSG_INVALID_DITHER_CHOICE;
            return;
        }
        intervalometer.currentSettings.dither = dither;

        int ditherFrequency = server.arg(DITHER_FREQUENCY).toInt();
        if (ditherFrequency <= 0)
        {
            intervalometer.currentErrorMessage = ERR_MSG_INVALID_DITHER_FREQUENCY;
            return;
        }
        intervalometer.currentSettings.ditherFrequency = ditherFrequency;

        int focalLength = server.arg(FOCAL_LENGTH).toInt();
        if (focalLength <= 0)
        {
            intervalometer.currentErrorMessage = ERR_MSG_INVALID_FOCAL_LENGTH;
            return;
        }
        intervalometer.currentSettings.focalLength = focalLength;

        float pixelSize = server.arg(PIXEL_SIZE).toFloat() / 100;
        if (pixelSize <= 0.0)
        {
            intervalometer.currentErrorMessage = ERR_MSG_INVALID_PIXEL_SIZE;
            return;
        }
        intervalometer.currentSettings.pixelSize = pixelSize;

        String currentMode = server.arg(MODE);
        if (currentMode == "save")
        {
            int preset = server.arg(PRESET).toInt();
            intervalometer.saveSettingsToPreset(preset);
            server.send(200, MIME_TYPE_TEXT, languageMessageStrings[language][MSG_SAVED_PRESET]);
        }
        else if (currentMode == "start")
        {
            if ((intervalometer.currentSettings.mode == Intervalometer::LONG_EXPOSURE_MOVIE ||
                 intervalometer.currentSettings.mode == Intervalometer::LONG_EXPOSURE_STILL) &&
                !ra_axis.trackingActive)
            {
                server.send(200, MIME_TYPE_TEXT,
                            languageMessageStrings[language][MSG_TRACKING_NOT_ACTIVE]);
            }
            else
            {
                intervalometer.startCapture();
                server.send(200, MIME_TYPE_TEXT, languageMessageStrings[language][MSG_CAPTURE_ON]);
            }
        }
    }
    else
    {
        server.send(200, MIME_TYPE_TEXT, languageMessageStrings[language][MSG_CAPTURE_ALREADY_ON]);
    }
}

void handleGotoRA()
{
    Position currentPosition(0, 0, 0);
    Position targetPosition(0, 0, 0);
    int pan_speed = server.arg(SPEED).toInt();
    currentPosition.arcseconds = server.arg("currentRA").toInt();
    targetPosition.arcseconds = server.arg("targetRA").toInt();

    pan_speed = pan_speed > MAX_CUSTOM_SLEW_RATE   ? MAX_CUSTOM_SLEW_RATE
                : pan_speed < MIN_CUSTOM_SLEW_RATE ? MIN_CUSTOM_SLEW_RATE
                                                   : pan_speed;

    if (currentPosition.arcseconds == -1)
    {
        currentPosition.arcseconds = 0;
        print_out("Invalid Current RA input. Defaulting to 0.");
    }
    if (targetPosition.arcseconds == -1)
    {
        targetPosition.arcseconds = 0;
        print_out("Invalid Target RA input. Defaulting to 0.");
    }

    print_out("GotoRA called with:");
    print_out("  Current RA: %lld arcseconds", currentPosition.arcseconds);
    print_out("  Target RA: %lld arcseconds", targetPosition.arcseconds);
    print_out("  rate: %lld", (int) ((2 * ra_axis.trackingRate) / pan_speed));

    ra_axis.gotoTarget((2 * ra_axis.trackingRate) / pan_speed, currentPosition, targetPosition);
    server.send(200, MIME_TYPE_TEXT, languageMessageStrings[language][MSG_GOTO_RA_PANNING_ON]);
}

void handleGetPresetExposureSettings()
{
    int preset = server.arg(PRESET).toInt();
    intervalometer.readSettingsFromPreset(preset);
    JsonDocument settings;
    String json;
    settings[MODE] = intervalometer.currentSettings.mode;
    settings[EXPOSURES] = intervalometer.currentSettings.exposures;
    settings[DELAY] = intervalometer.currentSettings.delayTime;
    settings[PREDELAY] = intervalometer.currentSettings.preDelay;
    settings[EXPOSURE_TIME] = intervalometer.currentSettings.exposureTime;
    settings[PAN_ANGLE] = intervalometer.currentSettings.panAngle * 100;
    settings[PAN_DIRECTION] = intervalometer.currentSettings.panDirection;
    settings[DITHER_CHOICE] = intervalometer.currentSettings.dither;
    settings[DITHER_FREQUENCY] = intervalometer.currentSettings.ditherFrequency;
    settings[ENABLE_TRACKING] = intervalometer.currentSettings.enableTracking;
    settings[FRAMES] = intervalometer.currentSettings.frames;
    settings[PIXEL_SIZE] = intervalometer.currentSettings.pixelSize * 100;
    settings[FOCAL_LENGTH] = intervalometer.currentSettings.focalLength;
    serializeJson(settings, json);
    // print_out(json);
    server.send(200, "application/json", json);
}

void handleAbortCapture()
{
    if (intervalometer.intervalometerActive)
    {
        intervalometer.abortCapture();
        server.send(200, MIME_TYPE_TEXT, languageMessageStrings[language][MSG_CAPTURE_OFF]);
    }
    else
    {
        server.send(200, MIME_TYPE_TEXT, languageMessageStrings[language][MSG_CAPTURE_ALREADY_OFF]);
    }
}

void handleAbortGoToRA()
{
    if (ra_axis.slewActive)
    {
        ra_axis.stopGotoTarget();
        server.send(200, MIME_TYPE_TEXT, languageMessageStrings[language][MSG_GOTO_RA_PANNING_OFF]);
    }
}

void handleStatusRequest()
{
    if (intervalometer.intervalometerActive)
    {
        switch (intervalometer.currentState)
        {
            case Intervalometer::PRE_DELAY:
                server.send(200, MIME_TYPE_TEXT,
                            languageMessageStrings[language][MSG_CAP_PREDELAY]);
                break;
            case Intervalometer::CAPTURE:
                server.send(200, MIME_TYPE_TEXT,
                            languageMessageStrings[language][MSG_CAP_EXPOSING]);
                break;
            case Intervalometer::DITHER:
                server.send(200, MIME_TYPE_TEXT, languageMessageStrings[language][MSG_CAP_DITHER]);
                break;
            case Intervalometer::PAN:
                server.send(200, MIME_TYPE_TEXT, languageMessageStrings[language][MSG_CAP_PANNING]);
                break;
            case Intervalometer::DELAY:
                server.send(200, MIME_TYPE_TEXT, languageMessageStrings[language][MSG_CAP_DELAY]);
                break;
            case Intervalometer::REWIND:
                server.send(200, MIME_TYPE_TEXT, languageMessageStrings[language][MSG_CAP_REWIND]);
                break;
            case Intervalometer::INACTIVE:
            default:
                break;
        }
    }
    else if (ra_axis.slewActive && !ra_axis.goToTarget)
    {
        server.send(200, MIME_TYPE_TEXT, languageMessageStrings[language][MSG_SLEWING]);
    }
    else if (ra_axis.slewActive && ra_axis.goToTarget)
    {
        server.send(200, MIME_TYPE_TEXT, languageMessageStrings[language][MSG_GOTO_RA_PANNING_ON]);
    }
    else if (ra_axis.trackingActive)
    {
        server.send(200, MIME_TYPE_TEXT, languageMessageStrings[language][MSG_TRACKING_ON]);
    }
    else
    {
        if (intervalometer.currentErrorMessage == ErrorMessage::ERR_MSG_NONE)
            server.send(200, MIME_TYPE_TEXT, languageMessageStrings[language][MSG_IDLE]);
        else
            server.send(200, MIME_TYPE_TEXT,
                        languageErrorMessageStrings[language][intervalometer.currentErrorMessage]);
    }

    if (ra_axis.slewActive)
    {
        slewTimeOut.setCountValue(0); // reset timeout counter
    }

    server.send(204, MIME_TYPE_TEXT, "dummy");
}

void handleVersion()
{
    server.send(200, MIME_TYPE_TEXT, (String) INTERNAL_VERSION);
}

void setupWireless()
{
#ifdef AP_MODE
    WiFi.mode(WIFI_MODE_AP);
    WiFi.softAP(WIFI_SSID, WIFI_PASSWORD);
    vTaskDelay(500);
    print_out("Creating Wifi Network");

    // ANDROID 10 WORKAROUND==================================================
    // set new WiFi configurations
    WiFi.disconnect();
    print_out("reconfig WiFi...");
    /*Stop wifi to change config parameters*/
    esp_wifi_stop();
    esp_wifi_deinit();
    /*Disabling AMPDU RX is necessary for Android 10 support*/
    wifi_init_config_t my_config = WIFI_INIT_CONFIG_DEFAULT(); // We use the default config ...
    my_config.ampdu_rx_enable = 0;                             //... and modify only what we want.
    print_out("WiFi: Disabled AMPDU...");
    esp_wifi_init(&my_config); // set the new config = "Disable AMPDU"
    esp_wifi_start();          // Restart WiFi
    vTaskDelay(500);
    // ANDROID 10 WORKAROUND==================================================
#else
    WiFi.mode(WIFI_MODE_STA); // Set ESP32 in station mode
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    print_out("Connecting to Network in STA mode");
    while (WiFi.status() != WL_CONNECTED)
    {
        vTaskDelay(1000);
        print_out(".");
    }
#endif

    server.on("/", HTTP_GET, handleRoot);
    server.on("/on", HTTP_GET, handleOn);
    server.on("/off", HTTP_GET, handleOff);
    server.on("/startslew", HTTP_GET, handleSlewRequest);
    server.on("/stopslew", HTTP_GET, handleSlewOff);
    server.on("/setCurrent", HTTP_GET, handleSetCurrent);
    server.on("/readPreset", HTTP_GET, handleGetPresetExposureSettings);
    server.on("/abort", HTTP_GET, handleAbortCapture);
    server.on("/status", HTTP_GET, handleStatusRequest);
    server.on("/gotoRA", HTTP_GET, handleGotoRA);
    server.on("/abort-goto-ra", HTTP_GET, handleAbortGoToRA);
    server.on("/version", HTTP_GET, handleVersion);
    server.on("/setlang", HTTP_GET, handleSetLanguage);

    // Start the server
    server.begin();

#ifdef AP
    print_out("%s", WiFi.softAPIP().toString().c_str());
#else
    print_out("%s", WiFi.localIP().toString().c_str());
#endif

    print_out("Starting mDNS responder");
    if (!MDNS.begin(MDNS_NAME))
    {
		print_out("Error starting mDNS responder");
		return;
	}
    print_out("mDNS responder started");

    MDNS.addService("http", "tcp", WEBSERVER_PORT);
    MDNS.addServiceTxt("http", "tcp", "ogtracker", "1");
    MDNS.addServiceTxt("http", "tcp", "version", BUILD_VERSION);

    MDNS.addService("ogtracker", "tcp", WEBSERVER_PORT);
    MDNS.addServiceTxt("ogtracker", "tcp", "version", BUILD_VERSION);

}

void setup()
{
    // Start the debug serial connection
    setup_uart(&Serial, 115200);


    if (xTaskCreate(uartTask, "uart", 4096, NULL, 1, NULL))
    {
        // print_out_tbl(TSK_CLEAR_SCREEN);
        print_out_tbl(TSK_START_UART);
    }

    print_out_tbl(HEAD_LINE);
    print_out_tbl(HEAD_LINE_TRACKER);
    print_out("***         Running on %d MHz         ***", getCpuFrequencyMhz());
    print_out_tbl(HEAD_LINE_VERSION);

    EEPROM.begin(512); // SIZE = 5 x presets = 5 x 32 bytes = 160 bytes
    uint8_t langNum = EEPROM.read(LANG_EEPROM_ADDR);

    if (langNum >= LANG_COUNT)
        language = static_cast<Languages>(0);
    else
        language = static_cast<Languages>(langNum);

    // Initialize the pins
    pinMode(INTERV_PIN, OUTPUT);
    pinMode(STATUS_LED, OUTPUT);
    pinMode(AXIS1_STEP, OUTPUT);
    pinMode(AXIS1_DIR, OUTPUT);
    pinMode(EN12_n, OUTPUT);
    pinMode(MS1, OUTPUT);
    pinMode(MS2, OUTPUT);
    digitalWrite(AXIS1_STEP, LOW);
    digitalWrite(EN12_n, LOW);
    // handleExposureSettings();

    // Initialize Wifi and web server
    setupWireless();

    // Initialize the console serial
    setup_terminal(&term);

    if (xTaskCreate(consoleTask, "console", 4096, NULL, 1, NULL))
        print_out_tbl(TSK_START_CONSOLE);

    if (xTaskCreate(intervalometerTask, "intervalometer", 4096, NULL, 1, NULL))
        print_out_tbl(TSK_START_INTERVALOMETER);
    if (xTaskCreatePinnedToCore(webserverTask, "webserver", 4096, NULL, 1, NULL, 0))
        print_out_tbl(TSK_START_WEBSERVER);
}

void loop()
{
    int delay_ticks = 0;
    for (;;)
    {
        if (ra_axis.slewActive)
        {
            // Blink status LED if mount is in slew mode
            digitalWrite(STATUS_LED, !digitalRead(STATUS_LED));
            delay_ticks = 150; // Delay for 150 ms
        }
        else
        {
            // Turn on status LED if sidereal tracking is ON
            digitalWrite(STATUS_LED, ra_axis.trackingActive ? HIGH : LOW);
            delay_ticks = 1000; // Delay for 1 second
        }
        vTaskDelay(delay_ticks);
    }
}

void webserverTask(void* pvParameters)
{
    for (;;)
    {
        server.handleClient();
        vTaskDelay(1);
    }
}

void intervalometerTask(void* pvParameters)
{
    intervalometer.readPresetsFromEEPROM();

    for (;;)
    {
        if (intervalometer.intervalometerActive)
            intervalometer.run();
        vTaskDelay(1);
    }
}

void uartTask(void* pvParameters)
{
    for (;;)
    {
        uart_task();
        vTaskDelay(1);
    }
}

void consoleTask(void* pvParameters)
{
    for (;;)
    {
        term.readSerial();
        vTaskDelay(1);
    }
}

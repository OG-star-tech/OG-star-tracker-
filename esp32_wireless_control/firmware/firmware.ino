#include <ArduinoJson.h>
#include <ESPmDNS.h>
#include <ErriezSerialTerminal.h>
#include <WebServer.h>
#include <WiFi.h>
#include <esp_wifi.h>
#include <freertos/FreeRTOS.h>
#include <string.h>

#include "axis.h"
#include "catalogues/star_database.h"
#include "commands.h"
#include "common_strings.h"
#include "configs/config.h"
#include "eeprom_manager.h"
#include "functions/intervalometer/intervalometer.h"
#include "functions/ota/ota_handler.h"
#include "hardwaretimer.h"
#include "tracking_rates.h"
#include "uart.h"
#include "website/api_handler.h"
#include "website/web_languages.h"
#include "website/website_strings.h"

SerialTerminal term(CLI_NEWLINE_CHAR, CLI_DELIMITER_CHAR);
WebServer server(WEBSERVER_PORT);
Languages language = EN;
StarDatabase* starDatabase = nullptr;
Intervalometer* intervalometer = nullptr;

void uartTask(void* pvParameters);
void consoleTask(void* pvParameters);
void webserverTask(void* pvParameters);
void intervalometerTask(void* pvParameters);
void systemShutdown();

extern const uint8_t _interface_index_html_start[] asm("_binary_interface_index_html_start");
extern const uint8_t _interface_index_html_end[] asm("_binary_interface_index_html_end");

extern const uint8_t _catalogues_ngc_converted_ngc2000_bin_start[] asm(
    "_binary_catalogues_ngc_converted_ngc2000_bin_start");
extern const uint8_t _catalogues_ngc_converted_ngc2000_bin_end[] asm(
    "_binary_catalogues_ngc_converted_ngc2000_bin_end");

extern const uint8_t _catalogues_ngc_converted_ngc2000_compact_bin_start[] asm(
    "_binary_catalogues_ngc_converted_ngc2000_compact_bin_start");
extern const uint8_t _catalogues_ngc_converted_ngc2000_compact_bin_end[] asm(
    "_binary_catalogues_ngc_converted_ngc2000_compact_bin_end");

extern const uint8_t _catalogues_bsc5_converted_bsc5ra_bin_start[] asm(
    "_binary_catalogues_bsc5_converted_bsc5ra_bin_start");
extern const uint8_t _catalogues_bsc5_converted_bsc5ra_bin_end[] asm(
    "_binary_catalogues_bsc5_converted_bsc5ra_bin_end");

extern const uint8_t _catalogues_bsc5_converted_bsc5ra_compact_bin_start[] asm(
    "_binary_catalogues_bsc5_converted_bsc5ra_compact_bin_start");
extern const uint8_t _catalogues_bsc5_converted_bsc5ra_compact_bin_end[] asm(
    "_binary_catalogues_bsc5_converted_bsc5ra_compact_bin_end");

StarDatabase* handleStarDatabase(StarDatabaseType type)
{
    StarDatabase* db = nullptr;
    const uint8_t* bin_start = nullptr;
    const uint8_t* bin_end = nullptr;
    size_t len = 0;

    if (starDatabase != nullptr && starDatabase->getDatabaseType() == type)
        return starDatabase; // Return existing instance if already loaded

    if (starDatabase != nullptr)
    {
        starDatabase->unloadDatabase();
        delete starDatabase;
        starDatabase = nullptr;
    }

    switch (type)
    {
        case DB_NGC2000:
            bin_start = _catalogues_ngc_converted_ngc2000_bin_start;
            bin_end = _catalogues_ngc_converted_ngc2000_bin_end;
            len = bin_end - bin_start;
            break;
        case DB_NGC2000_COMPACT:
            bin_start = _catalogues_ngc_converted_ngc2000_compact_bin_start;
            bin_end = _catalogues_ngc_converted_ngc2000_compact_bin_end;
            len = bin_end - bin_start;
            break;
        case DB_BSC5:
            bin_start = _catalogues_bsc5_converted_bsc5ra_bin_start;
            bin_end = _catalogues_bsc5_converted_bsc5ra_bin_end;
            len = bin_end - bin_start;
            break;
        case DB_BSC5_COMPACT:
            bin_start = _catalogues_bsc5_converted_bsc5ra_compact_bin_start;
            bin_end = _catalogues_bsc5_converted_bsc5ra_compact_bin_end;
            len = bin_end - bin_start;
            break;
        default:
            print_out("Error: Unsupported database type %d", type);
            return nullptr;
    }

    db = new StarDatabase(type, bin_start, bin_end);
    if (!db->loadDatabase((const char*) bin_start, len))
    {
        delete db;
        return nullptr;
    }

    return db;
}

String getChipID()
{
    uint64_t chipid = ESP.getEfuseMac();
    uint8_t baseMac[6];

    // Extract each byte of the MAC address from the chipid
    baseMac[0] = (chipid >> 40) & 0xFF;
    baseMac[1] = (chipid >> 32) & 0xFF;
    baseMac[2] = (chipid >> 24) & 0xFF;
    baseMac[3] = (chipid >> 16) & 0xFF;
    baseMac[4] = (chipid >> 8) & 0xFF;
    baseMac[5] = chipid & 0xFF;

    // Create a String object to hold the formatted ID
    String id = String(baseMac[4], HEX) + String(baseMac[5], HEX);
    return id;
}

#if DEBUG == 1
void handleNotFound()
{
    print_out("HTTP 404: %s %s", server.method() == HTTP_GET ? "GET" : "POST",
              server.uri().c_str());
    print_out("  Client: %s", server.client().remoteIP().toString().c_str());
    print_out("  User-Agent: %s", server.header("User-Agent").c_str());
    server.send(404, MIME_TYPE_TEXT, "Not Found");
}
#endif

void setupWireless()
{
#if AP_MODE == 1
    // Create unique SSID for each device
    String chipID = getChipID();
    char ssid[32];
    sprintf(ssid, "%s#%s", WIFI_SSID, chipID.c_str());

    WiFi.mode(WIFI_MODE_AP);
    print_out("Creating Wifi Network: %s", ssid);

    // Configure AP with specific IP settings
    IPAddress local_IP(192, 168, 4, 1);
    IPAddress gateway(192, 168, 4, 1);
    IPAddress subnet(255, 255, 255, 0);

    if (!WiFi.softAPConfig(local_IP, gateway, subnet))
    {
        print_out("Failed to configure AP IP settings");
    }

    if (!WiFi.softAP(ssid, WIFI_PASSWORD))
    {
        print_out("Failed to start AP");
    }

    vTaskDelay(1000); // Give more time for AP to initialize

    // ANDROID 10 WORKAROUND==================================================
    // set new WiFi configurations
    print_out("Applying Android 10 compatibility workaround...");
    WiFi.disconnect();
    /*Stop wifi to change config parameters*/
    esp_wifi_stop();
    esp_wifi_deinit();
    /*Disabling AMPDU RX is necessary for Android 10 support*/
    wifi_init_config_t my_config = WIFI_INIT_CONFIG_DEFAULT(); // We use the default config ...
    my_config.ampdu_rx_enable = 0;                             //... and modify only what we want.
    print_out("WiFi: Disabled AMPDU...");
    esp_wifi_init(&my_config); // set the new config = "Disable AMPDU"
    esp_wifi_start();          // Restart WiFi

    // Restart AP after workaround
    WiFi.mode(WIFI_MODE_AP);
    if (!WiFi.softAPConfig(local_IP, gateway, subnet))
    {
        print_out("Failed to reconfigure AP IP settings after workaround");
    }
    if (!WiFi.softAP(ssid, WIFI_PASSWORD))
    {
        print_out("Failed to restart AP after workaround");
    }

    vTaskDelay(1000);
    // ANDROID 10 WORKAROUND==================================================

    print_out("AP setup complete. Checking status...");
    print_out("AP Status: %s", WiFi.softAPgetStationNum() >= 0 ? "Active" : "Inactive");
    print_out("WiFi Mode: %d", WiFi.getMode());
#else
    WiFi.mode(WIFI_MODE_STA); // Set ESP32 in station mode
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    print_out("Connecting to SSID: %s with password: %s", WIFI_SSID, WIFI_PASSWORD);
    print_out("Connecting to Network in STA mode");
    while (WiFi.status() != WL_CONNECTED)
    {
        vTaskDelay(1000);
        print_out(".");
    }
#endif

    // Initialize OTA Handler (singleton)
    OTAHandler::getInstance().init(&server);

    // Register all REST API endpoints using ApiHandler
    ApiHandler::getInstance().init(&server);
    ApiHandler::getInstance().registerEndpoints();

#if DEBUG == 1
    server.onNotFound(handleNotFound);
    print_out("Web server debug logging enabled");
#endif

    // Start the server
    server.begin();

#if AP_MODE == 1
    IPAddress apIP = WiFi.softAPIP();
    print_out("AP IP address: %s", apIP.toString().c_str());
    if (apIP == IPAddress(0, 0, 0, 0))
    {
        print_out("ERROR: AP IP is 0.0.0.0 - AP may not be configured correctly");
        print_out("WiFi Mode: %d", WiFi.getMode());
        print_out("AP Status: %d", WiFi.status());
    }
#else
    print_out("STA IP address: %s", WiFi.localIP().toString().c_str());
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

    // start UART task before any usage of print_out
    if (xTaskCreatePinnedToCore(uartTask, "uart", 4096, NULL, 1, NULL, 1))
    {
        print_out_tbl(TSK_CLEAR_SCREEN);
        print_out_tbl(TSK_START_UART);
    }

    print_out_tbl(HEAD_LINE);
    print_out_tbl(HEAD_LINE_TRACKER);
    print_out("***         Running on %d MHz         ***", getCpuFrequencyMhz());
    print_out("***     Dual Core Setup: ISR Core 0    ***");
    print_out("***     Application Tasks on Core 1    ***");
    print_out_tbl(HEAD_LINE_VERSION);

    // Initialize EEPROM manager
    EepromManager::begin(512); // SIZE = 5 x presets = 5 x 32 bytes = 160 bytes
    uint8_t langNum = 0;
    EepromManager::readObject(LANG_EEPROM_ADDR, langNum);

    if (langNum >= LANG_COUNT)
        language = static_cast<Languages>(0);
    else
        language = static_cast<Languages>(langNum);

    delay(1000);

    uint8_t raInvertNum = 0;
    EepromManager::readObject(RA_INVERT_DIR_EEPROM_ADDR, raInvertNum);
    if (raInvertNum > 1)
        ra_axis.setInvertDirectionPin(RA_INVERT_DIR_PIN); // never saved yet, use compile-time default
    else
        ra_axis.setInvertDirectionPin(raInvertNum == 1);

    // Initialize the pins
    bool result = ledcAttach(STATUS_LED, LEDC_FREQ, LEDC_RESOLUTION);
    if (!result)
    {
        print_out("Failed to attach LEDC to STATUS_LED pin");
    }
    pinMode(INTERV_PIN, OUTPUT);
    pinMode(AXIS1_STEP, OUTPUT);
    pinMode(AXIS1_DIR, OUTPUT);
    pinMode(EN12_n, OUTPUT);
    digitalWrite(AXIS1_STEP, LOW);
    digitalWrite(EN12_n, LOW);
    // handleExposureSettings();

    // Initialize Wifi and web server
    setupWireless();

    // Initialize the console serial
    setup_terminal(&term);

    if (xTaskCreatePinnedToCore(consoleTask, "console", 4096, NULL, 1, NULL, 1))
        print_out_tbl(TSK_START_CONSOLE);

    if (xTaskCreatePinnedToCore(intervalometerTask, "intervalometer", 4096, NULL, 1, NULL, 1))
        print_out_tbl(TSK_START_INTERVALOMETER);
    // Increase webserver task stack size to handle large HTML responses and concurrent connections
    if (xTaskCreatePinnedToCore(webserverTask, "webserver", 8192, NULL, 1, NULL, 1))
        print_out_tbl(TSK_START_WEBSERVER);

    // Give tasks time to fully initialize before starting axis
    vTaskDelay(100);
    print_out("Initializing axis with TMC driver...");

    ra_axis.begin();
}

void loop()
{
    static bool led_blink = false;
    int delay_ticks = 0;
    trackingRates.readTrackingRatePresetsFromEEPROM();

    if (DEFAULT_ENABLE_TRACKING == 1)
    {
        ra_axis.startTracking(ra_axis.rate.tracking, ra_axis.direction.tracking);
    }

    for (;;)
    {
        if (ra_axis.slewActive)
        {
            // Blink status LED if mount is in slew mode
            ledcWrite(STATUS_LED, led_blink ? 0 : STATUS_LED_BRIGHTNESS);
            led_blink = !led_blink;
            delay_ticks = 150; // Delay for 150 ms
        }
        else
        {
            // Turn on status LED if sidereal tracking is ON
            ledcWrite(STATUS_LED, ra_axis.trackingActive ? STATUS_LED_BRIGHTNESS : 0);
            delay_ticks = 1000; // Delay for 1 second
        }
        ra_axis.print_status();
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
    intervalometer = new Intervalometer(INTERV_PIN);
    intervalometer->readPresetsFromEEPROM();

    for (;;)
    {
        if (intervalometer->isActive())
            intervalometer->cleanup();
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

/**
 * @brief System shutdown and reset
 *
 * Stops motors, cleans up critical resources, and performs hard reset.
 * The reset() function jump provides more complete memory cleanup than ESP.restart()
 */
void (*resetFunc)(void) = 0; // declare reset function at address 0

void systemShutdown()
{
    print_out("[SHUTDOWN] System restart initiated...");

    // Stop motors for safety
    ra_axis.stopTracking();
    ra_axis.stopSlew();
    ra_axis.stopGotoTarget();
    intervalometer->abortCapture();
    intervalometer->cleanup();
    server.stop();
    server.close();
    WiFi.disconnect(true);
    WiFi.mode(WIFI_OFF);

    vTaskDelay(2000);
    print_out("[SHUTDOWN] Goodbye!");
    vTaskDelay(500);

    resetFunc();
}

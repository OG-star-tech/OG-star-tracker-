#include "api_handler.h"
#include "../axis.h"
#include "../catalogues/star_database.h"
#include "../commands.h"
#include "../configs/consts.h"
#include "../eeprom_manager.h"
#include "../error.h"
#include "../functions/intervalometer/intervalometer.h"
#include "../functions/ota/ota_handler.h"
#include "../tools/heap_monitor.h"
#include "../tracking_rates.h"
#include "../uart.h"
#include "web_languages.h"
#include "website_strings.h"

// Forward declarations of external objects
extern Axis ra_axis;
extern Intervalometer* intervalometer;
extern TrackingRates trackingRates;
extern Languages language;
extern StarDatabase* starDatabase;
extern StarDatabase* handleStarDatabase(StarDatabaseType catalogType);

// External HTML interface data
extern const uint8_t _interface_index_html_start[] asm("_binary_interface_index_html_start");
extern const uint8_t _interface_index_html_end[] asm("_binary_interface_index_html_end");

ApiHandler& ApiHandler::getInstance()
{
    static ApiHandler instance;
    return instance;
}

void ApiHandler::init(WebServer* server)
{
    _server = server;
}

#if DEBUG == 1
static void logRequest(WebServer* server, const char* endpoint)
{
    print_out("HTTP Request: %s %s", server->method() == HTTP_GET ? "GET" : "POST", endpoint);
    print_out("  Client: %s", server->client().remoteIP().toString().c_str());
    if (server->args() > 0)
    {
        print_out("  Arguments (%d):", server->args());
        for (int i = 0; i < server->args(); i++)
        {
            print_out("    %s = %s", server->argName(i).c_str(), server->arg(i).c_str());
        }
    }
}
#endif

static Position calculatePosition(String Arg)
{
    Position position(0, 0, 0);
    position.arcseconds = Arg.toInt();

    if (position.arcseconds == -1)
    {
        position.arcseconds = 0;
#if DEBUG == 1
        print_out("Invalid position input. Defaulting to 0.");
#endif
    }

    return position;
}

void ApiHandler::registerEndpoints()
{
    ApiHandler* api = this;

    // Web interface
    _server->on("/", HTTP_GET, [api]() { api->handleRoot(); });
    // Motor control
    _server->on("/motorOff", HTTP_GET, [api]() { api->handleMotorOff(); });
    _server->on("/motorOn", HTTP_GET, [api]() { api->handleMotorOn(); });
    // Tracking control
    _server->on("/on", HTTP_GET, [api]() { api->handleOn(); });
    _server->on("/off", HTTP_GET, [api]() { api->handleOff(); });
    // Slewing control
    _server->on("/startslew", HTTP_GET, [api]() { api->handleSlewRequest(); });
    _server->on("/stopslew", HTTP_GET, [api]() { api->handleSlewOff(); });

    // Goto control
    _server->on("/gotoRA", HTTP_GET, [api]() { api->handleGotoRA(); });
    _server->on("/abort-goto-ra", HTTP_GET, [api]() { api->handleAbortGoToRA(); });
    // Position management
    _server->on("/setPosition", HTTP_GET, [api]() { api->handleSetPosition(); });
    _server->on("/getCurrentPosition", HTTP_GET, [api]() { api->handleGetCurrentPosition(); });
    // Intervalometer control
    _server->on("/setCurrent", HTTP_GET, [api]() { api->handleSetCurrent(); });
    _server->on("/readPreset", HTTP_GET, [api]() { api->handleGetPresetExposureSettings(); });
    _server->on("/abort", HTTP_GET, [api]() { api->handleAbortCapture(); });
    // Tracking rates
    _server->on("/getTrackingRates", HTTP_GET, [api]() { api->handleGetTrackingRates(); });
    _server->on("/saveTrackingRatePreset", HTTP_GET,
                [api]() { api->handleSaveTrackingRatePreset(); });
    _server->on("/loadTrackingRatePreset", HTTP_GET,
                [api]() { api->handleLoadTrackingRatePreset(); });

    // Status & info
    _server->on("/status", HTTP_GET, [api]() { api->handleStatusRequest(); });
    _server->on("/version", HTTP_GET, [api]() { api->handleVersion(); });

    // Catalog search
    _server->on("/starSearch", HTTP_GET, [api]() { api->handleCatalogSearch(); });
    // Settings
    _server->on("/setlang", HTTP_GET, [api]() { api->handleSetLanguage(); });
    _server->on("/getlang", HTTP_GET, [api]() { api->handleGetLanguage(); });
    _server->on("/langstrings", HTTP_GET, [api]() { api->handleGetLanguageStrings(); });

    // OTA firmware update
    _server->on("/ota", HTTP_GET, []() { OTAHandler::getInstance().handleOTAPage(); });
    _server->on(
        "/update", HTTP_POST, []() { OTAHandler::getInstance().handleOTAComplete(); },
        []() { OTAHandler::getInstance().handleOTAUpload(); });
    _server->on("/checkversion", HTTP_GET,
                []() { OTAHandler::getInstance().handleCheckVersion(); });
    _server->on("/downloadupdate", HTTP_GET,
                []() { OTAHandler::getInstance().handleDownloadUpdate(); });
    _server->on("/otastatus", HTTP_GET, []() { OTAHandler::getInstance().handleOTAStatus(); });
}

// Handler implementations
void ApiHandler::handleRoot()
{
#if DEBUG == 1
    logRequest(_server, "/");
    print_out("  Client IP: %s", _server->client().remoteIP().toString().c_str());
    print_out("  User-Agent: %s", _server->header("User-Agent").c_str());
    HeapMonitor::log("handleRoot-start");
#endif

    // Simply serve the HTML as-is - no server-side processing
    // Language strings are loaded via JavaScript on the client side
    _server->send_P(200, MIME_TYPE_HTML, (const char*) _interface_index_html_start,
                    _interface_index_html_end - _interface_index_html_start);

#if DEBUG == 1
    print_out("  HTML served directly, size: %d bytes",
              _interface_index_html_end - _interface_index_html_start);
    HeapMonitor::log("handleRoot-end");
#endif
}

void ApiHandler::handleOn()
{
#if DEBUG == 1
    logRequest(_server, "/on");
#endif
    print_out("Handling tracking ON request with custom rate: %d",
              _server->arg(TRACKING_SPEED).toInt());

    uint64_t custom_rate = _server->arg(TRACKING_SPEED).toInt();
    int direction = _server->arg(DIRECTION).toInt();

    trackingRates.setCustomRate(custom_rate);
#if DEBUG == 1
    print_out("  Direction: %d, Final rate: %llu", direction, trackingRates.getRate());
#endif
    ra_axis.startTracking(trackingRates.getRate(), direction);

    if (intervalometer->getErrorMessage() == ErrorMessage::ERR_MSG_NONE)
        _server->send(200, MIME_TYPE_TEXT, languageMessageStrings[language][MSG_TRACKING_ON]);
    else
        _server->send(200, MIME_TYPE_TEXT,
                      languageErrorMessageStrings[language][intervalometer->getErrorMessage()]);
#if DEBUG == 1
    print_out("  Tracking ON response sent");
#endif
}

void ApiHandler::handleOff()
{
    ra_axis.stopTracking();
    _server->send(200, MIME_TYPE_TEXT, languageMessageStrings[language][MSG_TRACKING_OFF]);
}

void ApiHandler::handleMotorOff()
{
    // Stop all axis activity before disabling driver power.
    ra_axis.stopTracking();
    if (ra_axis.slewActive)
    {
        if (ra_axis.goToTarget)
            ra_axis.stopGotoTarget();
        else
            ra_axis.stopSlew();
    }

    ra_axis.disableMotorPower();
    _server->send(200, MIME_TYPE_TEXT, languageMessageStrings[language][MSG_MOTOR_OFF]);
}

void ApiHandler::handleMotorOn()
{
    ra_axis.enableMotorPower();
    _server->send(200, MIME_TYPE_TEXT, languageMessageStrings[language][MSG_MOTOR_ON]);
}

void ApiHandler::handleSlewRequest()
{
    if (!ra_axis.slewActive)
    { // if slew is not active - needed for ipad (passes multiple touchon events)
        int slew_speed = _server->arg(SPEED).toInt();
        int direction = _server->arg(DIRECTION).toInt();
        // limit custom slew speed to 2-400
        slew_speed = slew_speed > MAX_CUSTOM_SLEW_RATE   ? MAX_CUSTOM_SLEW_RATE
                     : slew_speed < MIN_CUSTOM_SLEW_RATE ? MIN_CUSTOM_SLEW_RATE
                                                         : slew_speed;
        ra_axis.startSlew((2 * ra_axis.rate.tracking) / slew_speed, direction);
        _server->send(200, MIME_TYPE_TEXT, languageMessageStrings[language][MSG_SLEWING]);
    }
}

void ApiHandler::handleSlewOff()
{
    if (ra_axis.slewActive)
    { // if slew is active needed for ipad (passes multiple touchoff events)
        ra_axis.stopSlew();
    }
    _server->send(200, MIME_TYPE_TEXT, languageMessageStrings[language][MSG_SLEW_CANCELLED]);
}

void ApiHandler::handleSetLanguage()
{
    int lang = _server->arg("lang").toInt();
    language = static_cast<Languages>(lang);
    EepromManager::writeObject(LANG_EEPROM_ADDR, language);
    _server->send(200, MIME_TYPE_TEXT, languageMessageStrings[language][MSG_OK]);
}

void ApiHandler::handleGetLanguage()
{
    char response[50];
    snprintf(response, sizeof(response), "{\"lang\":%d}", static_cast<int>(language));
    _server->send(200, MIME_APPLICATION_JSON, response);
}

void ApiHandler::handleGetLanguageStrings()
{
    // Helper to escape JSON string
    // TODO: make this more readable / reusable
    auto escapeJson = [](const char* str) -> String
    {
        String result = "";
        while (*str)
        {
            switch (*str)
            {
                case '"':
                    result += "\\\"";
                    break;
                case '\\':
                    result += "\\\\";
                    break;
                case '\n':
                    result += "\\n";
                    break;
                case '\r':
                    result += "\\r";
                    break;
                case '\t':
                    result += "\\t";
                    break;
                case '\b':
                    result += "\\b";
                    break;
                case '\f':
                    result += "\\f";
                    break;
                default:
                    if (*str < 0x20)
                    {
                        // Escape control characters
                        char buf[7];
                        snprintf(buf, sizeof(buf), "\\u%04x", (unsigned char) *str);
                        result += buf;
                    }
                    else
                    {
                        result += *str;
                    }
                    break;
            }
            str++;
        }
        return result;
    };

    // Build JSON response with all language strings
    // Use chunked encoding to avoid large buffer allocation
    _server->setContentLength(CONTENT_LENGTH_UNKNOWN);
    _server->send(200, MIME_APPLICATION_JSON, "");

    _server->sendContent("{");

    // Language names
    _server->sendContent("\"langNames\":[");
    for (int i = 0; i < LANG_COUNT; i++)
    {
        if (i > 0)
            _server->sendContent(",");
        _server->sendContent("\"");
        _server->sendContent(escapeJson(languageNames[language][i]).c_str());
        _server->sendContent("\"");
    }
    _server->sendContent("],");

    // HTML strings
    _server->sendContent("\"strings\":{");
    for (int i = 0; i < numberOfHTMLStrings; i++)
    {
        if (i > 0)
            _server->sendContent(",");
        _server->sendContent("\"");
        _server->sendContent(escapeJson(HTMLplaceHolders[i]).c_str());
        _server->sendContent("\":\"");
        _server->sendContent(escapeJson(languageHTMLStrings[language][i]).c_str());
        _server->sendContent("\"");
    }
    _server->sendContent("}");

    _server->sendContent("}");
    _server->sendContent("");
}

void ApiHandler::handleSetCurrent()
{
    if (!intervalometer->isActive())
    {
        // Reset the current error message
        intervalometer->setErrorMessage(ERR_MSG_NONE);

        Intervalometer::Settings settings = intervalometer->getSettings();

        int modeInt = _server->arg(CAPTURE_MODE).toInt();
        if (modeInt < 0 || modeInt >= static_cast<int>(Intervalometer::Mode::MaxModes))
        {
            intervalometer->setErrorMessage(ERR_MSG_INVALID_CAPTURE_MODE);
            return;
        }
        intervalometer->setMode(static_cast<Intervalometer::Mode>(modeInt));

        int exposureTime = _server->arg(EXPOSURE_TIME).toInt();
        if (exposureTime <= 0)
        {
            intervalometer->setErrorMessage(ERR_MSG_INVALID_EXPOSURE_LENGTH);
            return;
        }
        settings.exposureTime = exposureTime;

        int exposures = _server->arg(EXPOSURES).toInt();
        if (exposures <= 0)
        {
            intervalometer->setErrorMessage(ERR_MSG_INVALID_EXPOSURE_AMOUNT);
            return;
        }
        settings.exposures = exposures;

        int preDelay = _server->arg(PREDELAY).toInt();
        if (preDelay < 0)
        {
            intervalometer->setErrorMessage(ERR_MSG_INVALID_PREDELAY_TIME);
            return;
        }
        else if (preDelay == 0)
            preDelay = 5;
        settings.preDelay = preDelay;

        int delayTime = _server->arg(DELAY).toInt();
        if (delayTime < 0)
        {
            intervalometer->setErrorMessage(ERR_MSG_INVALID_DELAY_TIME);
            return;
        }
        settings.delayTime = delayTime;

        int frames = _server->arg(FRAMES).toInt();
        if (frames <= 0)
        {
            intervalometer->setErrorMessage(ERR_MSG_INVALID_FRAME_AMOUNT);
            return;
        }
        settings.frames = frames;

        float panAngle = _server->arg(PAN_ANGLE).toFloat() / 100;
        if (panAngle < 0.0)
        {
            intervalometer->setErrorMessage(ERR_MSG_INVALID_PAN_ANGLE);
            return;
        }
        settings.panAngle = panAngle;

        int panDirection = _server->arg(PAN_DIRECTION).toInt();
        if (panDirection < 0 || panDirection > 1)
        {
            intervalometer->setErrorMessage(ERR_MSG_INVALID_PAN_DIRECTION);
            return;
        }
        settings.panDirection = panDirection;

        int continuousPan = _server->arg(CONTINUOUS_PAN).toInt();
        settings.continuousPan = continuousPan;

        int enableTracking = _server->arg(ENABLE_TRACKING).toInt();
        if (enableTracking < 0 || enableTracking > 1)
        {
            intervalometer->setErrorMessage(ERR_MSG_INVALID_ENABLE_TRACKING_VALUE);
            return;
        }
        settings.enableTracking = enableTracking;

        int dither = _server->arg(DITHER_CHOICE).toInt();
        if (dither < 0 || dither > 1)
        {
            intervalometer->setErrorMessage(ERR_MSG_INVALID_DITHER_CHOICE);
            return;
        }
        settings.dither = dither;

        int ditherFrequency = _server->arg(DITHER_FREQUENCY).toInt();
        if (ditherFrequency <= 0)
        {
            intervalometer->setErrorMessage(ERR_MSG_INVALID_DITHER_FREQUENCY);
            return;
        }
        settings.ditherFrequency = ditherFrequency;

        int focalLength = _server->arg(FOCAL_LENGTH).toInt();
        if (focalLength <= 0)
        {
            intervalometer->setErrorMessage(ERR_MSG_INVALID_FOCAL_LENGTH);
            return;
        }
        settings.focalLength = focalLength;

        float pixelSize = _server->arg(PIXEL_SIZE).toFloat() / 100;
        if (pixelSize <= 0.0)
        {
            intervalometer->setErrorMessage(ERR_MSG_INVALID_PIXEL_SIZE);
            return;
        }
        settings.pixelSize = pixelSize;
        settings.mode = static_cast<uint8_t>(intervalometer->getMode());
        intervalometer->setSettings(settings);

        String currentMode = _server->arg(MODE);
        if (currentMode == "save")
        {
            int preset = _server->arg(PRESET).toInt();
            intervalometer->saveSettingsToPreset(preset);
            _server->send(200, MIME_TYPE_TEXT, languageMessageStrings[language][MSG_SAVED_PRESET]);
        }
        else if (currentMode == "start")
        {
            Intervalometer::Mode mode = intervalometer->getMode();
            if ((mode == Intervalometer::Mode::LongExposureMovie ||
                 mode == Intervalometer::Mode::LongExposureStill) &&
                !ra_axis.trackingActive)
            {
                _server->send(200, MIME_TYPE_TEXT,
                              languageMessageStrings[language][MSG_TRACKING_NOT_ACTIVE]);
            }
            else
            {
                intervalometer->startCapture();
                _server->send(200, MIME_TYPE_TEXT,
                              languageMessageStrings[language][MSG_CAPTURE_ON]);
            }
        }
    }
    else
    {
        _server->send(200, MIME_TYPE_TEXT,
                      languageMessageStrings[language][MSG_CAPTURE_ALREADY_ON]);
    }
}

void ApiHandler::handleGotoRA()
{
    Position currentPosition = calculatePosition(_server->arg("currentRA"));
    Position targetPosition = calculatePosition(_server->arg("targetRA"));
    int pan_speed = _server->arg(SPEED).toInt();
    bool hemisphereDirection = ra_axis.direction.tracking;

    pan_speed = pan_speed > MAX_CUSTOM_SLEW_RATE   ? MAX_CUSTOM_SLEW_RATE
                : pan_speed < MIN_CUSTOM_SLEW_RATE ? MIN_CUSTOM_SLEW_RATE
                                                   : pan_speed;

    print_out("GotoRA called with:");
    print_out("  Current RA: %lld arcseconds", currentPosition.arcseconds);
    print_out("  Target RA: %lld arcseconds", targetPosition.arcseconds);
    print_out("  Hemisphere direction: %d", hemisphereDirection);
    print_out("  rate: %lld", (int) ((2 * ra_axis.rate.tracking) / pan_speed));

    ra_axis.gotoTarget(TRACKER_MOTOR_MICROSTEPPING / 2, (2 * ra_axis.rate.tracking) / pan_speed,
                       currentPosition, targetPosition, hemisphereDirection);
    _server->send(200, MIME_TYPE_TEXT, languageMessageStrings[language][MSG_GOTO_RA_PANNING_ON]);
}

void ApiHandler::handleSetPosition()
{
    Position currentPosition = calculatePosition(_server->arg("currentRA"));
    int64_t stepPosition = currentPosition.arcseconds * trackingRates.getStepsPerSecondSolar();

    ra_axis.setPosition(stepPosition);
    _server->send(200, MIME_TYPE_TEXT, languageMessageStrings[language][MSG_POSITION_SET_SUCCESS]);
}

void ApiHandler::handleGetPresetExposureSettings()
{
    int preset = _server->arg(PRESET).toInt();
    intervalometer->readSettingsFromPreset(preset);
    ArduinoJson::JsonDocument settings;
    String json;
    const Intervalometer::Settings& s = intervalometer->getSettings();
    settings[MODE] = static_cast<uint8_t>(intervalometer->getMode());
    settings[EXPOSURES] = s.exposures;
    settings[DELAY] = s.delayTime;
    settings[PREDELAY] = s.preDelay;
    settings[EXPOSURE_TIME] = s.exposureTime;
    settings[PAN_ANGLE] = s.panAngle * 100;
    settings[PAN_DIRECTION] = s.panDirection;
    settings[CONTINUOUS_PAN] = s.continuousPan;
    settings[DITHER_CHOICE] = s.dither;
    settings[DITHER_FREQUENCY] = s.ditherFrequency;
    settings[ENABLE_TRACKING] = s.enableTracking;
    settings[FRAMES] = s.frames;
    settings[PIXEL_SIZE] = s.pixelSize * 100;
    settings[FOCAL_LENGTH] = s.focalLength;
    serializeJson(settings, json);
    _server->send(200, MIME_APPLICATION_JSON, json);
}

void ApiHandler::handleAbortCapture()
{
    if (intervalometer->isActive())
    {
        intervalometer->abortCapture();
        _server->send(200, MIME_TYPE_TEXT, languageMessageStrings[language][MSG_CAPTURE_OFF]);
    }
    else
    {
        _server->send(200, MIME_TYPE_TEXT,
                      languageMessageStrings[language][MSG_CAPTURE_ALREADY_OFF]);
    }
}

void ApiHandler::handleAbortGoToRA()
{
    if (ra_axis.slewActive)
    {
        ra_axis.stopGotoTarget();
        _server->send(200, MIME_TYPE_TEXT,
                      languageMessageStrings[language][MSG_GOTO_RA_PANNING_OFF]);
    }
}

void ApiHandler::handleStatusRequest()
{
    if (intervalometer->isActive())
    {
        // Build status string with progress info
        String statusMsg;
        uint16_t currentExp = intervalometer->getExposuresTaken();
        uint16_t totalExp = intervalometer->getSettings().exposures;

        switch (intervalometer->getState())
        {
            case Intervalometer::State::PreDelay:
                statusMsg = languageMessageStrings[language][MSG_CAP_PREDELAY];
                break;
            case Intervalometer::State::Capture:
                statusMsg = languageMessageStrings[language][MSG_CAP_EXPOSING];
                statusMsg += " (" + String(currentExp + 1) + "/" + String(totalExp) + ")";
                break;
            case Intervalometer::State::Dither:
                statusMsg = languageMessageStrings[language][MSG_CAP_DITHER];
                statusMsg += " (" + String(currentExp) + "/" + String(totalExp) + ")";
                break;
            case Intervalometer::State::Pan:
                statusMsg = languageMessageStrings[language][MSG_CAP_PANNING];
                statusMsg += " (" + String(currentExp) + "/" + String(totalExp) + ")";
                break;
            case Intervalometer::State::Delay:
                statusMsg = languageMessageStrings[language][MSG_CAP_DELAY];
                statusMsg += " (" + String(currentExp) + "/" + String(totalExp) + ")";
                break;
            case Intervalometer::State::Rewind:
                statusMsg = languageMessageStrings[language][MSG_CAP_REWIND];
                break;
            case Intervalometer::State::Inactive:
            case Intervalometer::State::Complete:
            default:
                break;
        }

        if (statusMsg.length() > 0)
        {
            _server->send(200, MIME_TYPE_TEXT, statusMsg);
        }
    }
    else if (ra_axis.slewActive && !ra_axis.goToTarget)
    {
        _server->send(200, MIME_TYPE_TEXT, languageMessageStrings[language][MSG_SLEWING]);
    }
    else if (ra_axis.slewActive && ra_axis.goToTarget)
    {
        _server->send(200, MIME_TYPE_TEXT,
                      languageMessageStrings[language][MSG_GOTO_RA_PANNING_ON]);
    }
    else if (ra_axis.trackingActive)
    {
        _server->send(200, MIME_TYPE_TEXT, languageMessageStrings[language][MSG_TRACKING_ON]);
    }
    else
    {
        if (intervalometer->getErrorMessage() == ErrorMessage::ERR_MSG_NONE)
            if (ra_axis.motorActive)
            {
                String status = String(languageMessageStrings[language][MSG_IDLE]) + " - " + String(languageMessageStrings[language][MSG_MOTOR_ON]);
                _server->send(200, MIME_TYPE_TEXT, status);
            }else{
                String status = String(languageMessageStrings[language][MSG_IDLE]) + " - " + String(languageMessageStrings[language][MSG_MOTOR_OFF]);
                _server->send(200, MIME_TYPE_TEXT, status);
            }
        else
            _server->send(200, MIME_TYPE_TEXT,
                          languageErrorMessageStrings[language][intervalometer->getErrorMessage()]);
    }

    _server->send(204, MIME_TYPE_TEXT, "dummy");
}

void ApiHandler::handleVersion()
{
    String json = "{";
    json += "\"version\":\"" + String(BUILD_VERSION) + "\",";
    json += "\"buildDate\":\"" + String(__DATE__) + " " + String(__TIME__) + "\",";
    json += "\"internalVersion\":" + String(INTERNAL_VERSION);
    json += "}";

    _server->send(200, MIME_APPLICATION_JSON, json);
}

void ApiHandler::handleGetTrackingRates()
{
#if DEBUG == 1
    logRequest(_server, "/getTrackingRates");
    print_out("  Client IP: %s", _server->client().remoteIP().toString().c_str());
#endif

    int rateType = _server->arg("type").toInt(); // 0=current, 1=sidereal, 2=solar, 3=lunar
    print_out("Received tracking rate request with type: %d", rateType);
    uint64_t rate = 0;

    switch (rateType)
    {
        case 0:
            rate = trackingRates.getRate(); // current rate
            break;
        case 1:
            rate = trackingRates.getSiderealRate();
            break;
        case 2:
            rate = trackingRates.getSolarRate();
            break;
        case 3:
            rate = trackingRates.getLunarRate();
            break;
        default:
            rate = trackingRates.getRate(); // default to current
            break;
    }

    _server->send(200, MIME_TYPE_TEXT, String(rate));
#if DEBUG == 1
    print_out("  Response sent: %s", String(rate).c_str());
#endif
}

void ApiHandler::handleGetCurrentPosition()
{
    String utcTimeStr = _server->arg("utcTime");
    String timezoneStr = _server->arg("timezone");
    float longitude = _server->arg("longitude").toFloat();
    int64_t currentStepPosition = ra_axis.getPosition();

    // Calculate current RA position in arcseconds (0-86399)
    // Normalize position to handle negative values and multiple revolutions
    int64_t normalizedSteps =
        ((currentStepPosition % STEPS_PER_TRACKER_FULL_REV_INT) + STEPS_PER_TRACKER_FULL_REV_INT) %
        STEPS_PER_TRACKER_FULL_REV_INT;
    long raSeconds =
        (long) ((normalizedSteps * RA_SECONDS_PER_FULL_REV) / STEPS_PER_TRACKER_FULL_REV_INT);

    String response = "{\"ra\":" + String(raSeconds) + ",\"utcTime\":\"" + utcTimeStr + "\"" +
                      ",\"longitude\":" + String(longitude) + "}";

    _server->send(200, MIME_APPLICATION_JSON, response);
}

void ApiHandler::handleSaveTrackingRatePreset()
{
    int preset = _server->arg(PRESET).toInt();
    uint8_t trackingType = _server->arg(TRACKING_TYPE).toInt();
    uint64_t customRate = _server->arg(CUSTOM_RATE).toInt();

    // Save directly to tracking rate preset system
    trackingRates.saveTrackingRatePreset(preset, trackingType, customRate);

    _server->send(200, MIME_TYPE_TEXT, "Tracking rate preset saved");
}

void ApiHandler::handleLoadTrackingRatePreset()
{
    int preset = _server->arg(PRESET).toInt();

    if (preset < 5)
    {
        ArduinoJson::JsonDocument response;
        response["trackingRateType"] = trackingRates.trackingRatePresets[preset].trackingRateType;
        response["customTrackingRate"] =
            (long long) trackingRates.trackingRatePresets[preset].customTrackingRate;

        trackingRates.loadTrackingRatePreset(preset);

        String json;
        serializeJson(response, json);
        _server->send(200, MIME_APPLICATION_JSON, json);
    }
    else
        _server->send(400, MIME_TYPE_TEXT, "Invalid preset number");
}

void ApiHandler::handleCatalogSearch()
{
    StarDatabaseType catalogType = (StarDatabaseType) _server->arg(STAR_CATALOG).toInt();
    String objectName = _server->arg(STAR_NAME);

    starDatabase = handleStarDatabase(catalogType);

#if DEBUG == 1
    print_out("Received catalog=%d, name=%s", catalogType, objectName.c_str());
#endif

    if (objectName.length() == 0)
    {
        _server->send(400, MIME_TYPE_TEXT, "Object name required");
        return;
    }

    StarUnifiedEntry foundObject;
    bool found = starDatabase->findByName(objectName.c_str(), foundObject);

    if (found)
    {
        ArduinoJson::JsonDocument objectData;
        String json;
        objectData["name"] = foundObject.name;
        objectData["ra"] = (long long) (foundObject.ra_hours * 3600.0);
        objectData["dec"] = (long long) (foundObject.dec_deg * 3600.0);
        objectData["type"] = foundObject.type_str;
        objectData["magnitude"] = foundObject.magnitude;
        objectData["constellation"] = foundObject.constellation;
        serializeJson(objectData, json);

#if DEBUG == 1
        print_out("Found object: %s at RA=%.2fh, Dec=%.2f°", foundObject.name.c_str(),
                  foundObject.ra_hours, foundObject.dec_deg);
#endif
        _server->send(200, MIME_APPLICATION_JSON, json);
    }
    else
    {
#if DEBUG == 1
        print_out("Object not found: %s", objectName.c_str());
#endif
        _server->send(404, "text/plain", "Object not found");
    }
}

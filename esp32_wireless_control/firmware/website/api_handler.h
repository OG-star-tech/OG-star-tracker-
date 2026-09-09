#ifndef API_HANDLER_H
#define API_HANDLER_H

#include <WebServer.h>

/**
 * @class ApiHandler
 * @brief REST API handler for OG Star Tracker
 *
 * This class manages all HTTP REST API endpoints for controlling and monitoring
 * the star tracker device. Each endpoint is documented with its purpose, parameters,
 * and response format.
 */
class ApiHandler
{
  public:
    static ApiHandler& getInstance();
    void init(WebServer* server);

    void registerEndpoints();

    // ==================== TRACKING CONTROL ====================

    /**
     * @endpoint GET /on
     * @brief Enable sidereal tracking at specified rate and direction
     * @param direction - Hemisphere/direction (0=northern/clockwise, 1=southern/counter-clockwise)
     * @param trackingSpeed - Tracking rate value (depends on tracking rate type selected)
     * @response 200 OK with message "Tracking ON"
     */
    void handleOn();

    /**
     * @endpoint GET /off
     * @brief Disable sidereal tracking
     * @response 200 OK with message "Tracking OFF"
     */
    void handleOff();

    /**
     * @endpoint GET /motorOff
     * @brief Disable motor driver output power
     * @response 200 OK with message "Motor OFF"
     */
    void handleMotorOff();

    /**
     * @endpoint GET /motorOn
     * @brief Enable motor driver output power
     * @response 200 OK with message "Motor ON"
     */
    void handleMotorOn();

    // ==================== SLEWING CONTROL ====================

    /**
     * @endpoint GET /startslew
     * @brief Start manual slewing at specified speed
     * @param speed - Slew speed multiplier (2-400, lower=faster)
     * @param direction - Slew direction (0=left, 1=right)
     * @response 200 OK with message "Slewing"
     */
    void handleSlewRequest();

    /**
     * @endpoint GET /stopslew
     * @brief Stop current slewing operation
     * @response 200 OK with message "Slew Cancelled"
     */
    void handleSlewOff();

    // ==================== GOTO CONTROL ====================

    /**
     * @endpoint GET /gotoRA
     * @brief Move mount to target RA position
     * @param currentRA - Current RA position (HH:MM:SS format)
     * @param targetRA - Target RA position (HH:MM:SS format)
     * @param speed - Goto speed multiplier (2-400, lower=faster)
     * @response 200 OK with message "Goto RA - Panning ON"
     */
    void handleGotoRA();

    /**
     * @endpoint GET /abort-goto-ra
     * @brief Abort current goto RA operation
     * @response 200 OK with message "Goto RA aborted"
     */
    void handleAbortGoToRA();

    // ==================== POSITION MANAGEMENT ====================

    /**
     * @endpoint GET /setPosition
     * @brief Set current mount position
     * @param currentRA - Current RA position (HH:MM:SS format)
     * @response 200 OK with message "Position Set Success"
     */
    void handleSetPosition();

    /**
     * @endpoint GET /getCurrentPosition
     * @brief Get current mount position
     * @response 200 OK with JSON: {"position": <steps>}
     */
    void handleGetCurrentPosition();

    // ==================== INTERVALOMETER CONTROL ====================

    /**
     * @endpoint GET /setCurrent
     * @brief Configure and start intervalometer capture
     * @param mode - Capture mode (0=start, 1=save to preset)
     * @param preset - Preset number (0-4)
     * @param captureMode - Capture type (0-3: STILL/MOVIE/TIMELAPSE/TIMELAPSE_PAN)
     * @param exposureTime - Exposure duration in seconds
     * @param exposures - Number of exposures
     * @param preDelay - Pre-delay in seconds
     * @param delay - Delay between exposures in seconds
     * @param frames - Number of frames (for MOVIE mode)
     * @param panAngle - Pan angle in degrees * 100
     * @param panDirection - Pan direction (0=left, 1=right)
     * @param enableTracking - Enable tracking (0=off, 1=on)
     * @param ditherChoice - Enable dithering (0=off, 1=on)
     * @param ditherFrequency - Dither every N exposures
     * @param focalLength - Focal length in mm
     * @param pixelSize - Pixel size in microns * 100
     * @response 200 OK with message
     */
    void handleSetCurrent();

    /**
     * @endpoint GET /readPreset
     * @brief Load intervalometer preset settings
     * @param preset - Preset number (0-4)
     * @response 200 OK with JSON object containing preset settings
     */
    void handleGetPresetExposureSettings();

    /**
     * @endpoint GET /abort
     * @brief Abort current intervalometer capture
     * @response 200 OK with message "Capture OFF"
     */
    void handleAbortCapture();

    // ==================== TRACKING RATES ====================

    /**
     * @endpoint GET /getTrackingRates
     * @brief Get current tracking rate configuration
     * @response 200 OK with JSON: {"type": <type>, "customRate": <rate>}
     */
    void handleGetTrackingRates();

    /**
     * @endpoint GET /saveTrackingRatePreset
     * @brief Save tracking rate to preset
     * @param preset - Preset number (0-4)
     * @param type - Tracking type (0-4: SIDEREAL/LUNAR/SOLAR/KING/CUSTOM)
     * @param customRate - Custom rate value (if type=CUSTOM)
     * @response 200 OK with message
     */
    void handleSaveTrackingRatePreset();

    /**
     * @endpoint GET /loadTrackingRatePreset
     * @brief Load tracking rate from preset
     * @param preset - Preset number (0-4)
     * @response 200 OK with JSON: {"type": <type>, "customRate": <rate>}
     */
    void handleLoadTrackingRatePreset();

    // ==================== STATUS & INFO ====================

    /**
     * @endpoint GET /status
     * @brief Get device status information
     * @response 200 OK with JSON containing:
     *   - slewActive: boolean
     *   - trackingActive: boolean
     *   - intervalometerActive: boolean
     *   - goToTarget: boolean
     *   - exposuresTaken: number
     *   - currentExposure: number
     */
    void handleStatusRequest();

    /**
     * @endpoint GET /version
     * @brief Get firmware version
     * @response 200 OK with JSON: {"version": "<version>", "buildDate": "<date>"}
     */
    void handleVersion();

    // ==================== CATALOG SEARCH ====================

    /**
     * @endpoint GET /starSearch
     * @brief Search star/object catalog
     * @param catalog - Catalog type (0-3: NGC2000/NGC2000_COMPACT/BSC5/BSC5_COMPACT)
     * @param query - Search query string
     * @response 200 OK with JSON object containing search results
     */
    void handleCatalogSearch();

    // ==================== SETTINGS ====================

    /**
     * @endpoint GET /setlang
     * @brief Set web interface language
     * @param lang - Language code (0=EN, 1=DE, 2=CN)
     * @response 200 OK with message
     */
    void handleSetLanguage();

    /**
     * @endpoint GET /getlang
     * @brief Get current language
     * @response 200 OK with JSON {lang: number}
     */
    void handleGetLanguage();

    /**
     * @endpoint GET /langstrings
     * @brief Get all language strings for current language as JSON
     * @response 200 OK with JSON object containing all translated strings
     */
    void handleGetLanguageStrings();

    /**
     * @endpoint GET /setMotorSettings
     * @brief Set motor direction options (persisted to EEPROM), e.g. to correct reversed
     * stepper wiring without needing to recompile
     * @param raInvert - Reverse RA motor direction (0=normal, 1=inverted)
     * @response 200 OK with message
     */
    void handleSetMotorSettings();

    /**
     * @endpoint GET /getMotorSettings
     * @brief Get current motor direction options
     * @response 200 OK with JSON {"raInvert": 0|1}
     */
    void handleGetMotorSettings();

    /**
     * @endpoint GET /
     * @brief Serve main web interface
     * @response 200 OK with HTML content
     */
    void handleRoot();

    // ==================== OTA UPDATE ====================

    // check the ota_handler.h for OTA endpoint handlers

    /**
     * @endpoint GET /checkversion
     * @brief Check for firmware updates on GitHub
     * @response 200 OK with JSON containing version info and update availability
     * @response JSON format:
     * {
     *   "currentVersion": "v2.1",
     *   "buildDate": "Nov 16 2025 10:30:00",
     *   "updateAvailable": true/false,
     *   "latestVersion": "v2.2",  // if update available
     *   "releaseUrl": "https://...",  // if update available
     *   "downloadUrl": "https://...",  // if update available
     *   "releaseNotes": "..."  // if update available
     * }
     */
    void handleCheckVersion();

    /**
     * @endpoint GET /downloadupdate
     * @brief Download and install firmware update directly from GitHub
     * @param url - URL of the firmware.bin file to download
     * @response 200 OK with status updates
     * @note Device will reboot automatically after successful update
     */
    void handleDownloadUpdate();

    /**
     * @endpoint GET /otastatus
     * @brief Get current OTA update progress
     * @response JSON with {active, percent, bytesWritten, totalBytes, complete}
     */
    void handleOTAStatus();

  private:
    ApiHandler() : _server(nullptr)
    {
    }

    ApiHandler(const ApiHandler&) = delete;
    ApiHandler& operator=(const ApiHandler&) = delete;

    WebServer* _server;
};

#endif // API_HANDLER_H

# OG Star Tracker REST API Documentation

## Overview
The OG Star Tracker provides a REST API over HTTP for remote control and monitoring. All endpoints use GET requests and return either plain text or JSON responses.

**Base URL:** `http://<device-ip>/`  
**Default Port:** 80

---

## Table of Contents
1. [Tracking Control](#tracking-control)
2. [Slewing Control](#slewing-control)
3. [Goto Control](#goto-control)
4. [Position Management](#position-management)
5. [Intervalometer Control](#intervalometer-control)
6. [Tracking Rates](#tracking-rates)
7. [Status & Info](#status--info)
8. [Catalog Search](#catalog-search)
9. [Settings](#settings)
10. [OTA Firmware Update](#ota-firmware-update)

---

## Tracking Control

### Enable Tracking
**Endpoint:** `GET /on`  
**Description:** Enable sidereal tracking at specified rate and direction
This endpoint automatically enables motor driver power before movement.

**Parameters:**
| Parameter | Type | Required | Description |
|-----------|------|----------|-------------|
| `direction` | integer | Yes | Hemisphere/direction: 0=northern/clockwise, 1=southern/counter-clockwise |
| `trackingSpeed` | integer | Yes | Tracking rate value (depends on tracking rate type selected) |

**Response:** `200 OK` - "Tracking ON"

**Example:**
```
GET http://192.168.4.1/on?direction=0&trackingSpeed=15956
```

### Disable Tracking
**Endpoint:** `GET /off`  
**Description:** Disable sidereal tracking  
**Response:** `200 OK` - "Tracking OFF"

**Example:**
```
GET http://192.168.4.1/off
```

### Motor Off
**Endpoint:** `GET /motorOff`  
**Description:** Cut power to the stepper motor driver output stage (de-energize motor coils)  
**Response:** `200 OK` - "Motor OFF"

**Example:**
```
GET http://192.168.4.1/motorOff
```

### Motor On
**Endpoint:** `GET /motorOn`  
**Description:** Re-enable power to the stepper motor driver output stage  
**Response:** `200 OK` - "Motor ON"

**Example:**
```
GET http://192.168.4.1/motorOn
```

---

## Slewing Control

### Start Slewing
**Endpoint:** `GET /startslew`  
**Description:** Start manual slewing at specified speed  
This endpoint automatically enables motor driver power before movement.

**Parameters:**
| Parameter | Type | Required | Description |
|-----------|------|----------|-------------|
| `speed` | integer | Yes | Slew speed multiplier (2-400, lower=faster) |
| `direction` | integer | Yes | Direction: 0=left/west, 1=right/east |

**Response:** `200 OK` - "Slewing"

**Example:**
```
GET http://192.168.4.1/startslew?speed=16&direction=1
```

### Stop Slewing
**Endpoint:** `GET /stopslew`  
**Description:** Stop current slewing operation  
**Response:** `200 OK` - "Slew Cancelled"

**Example:**
```
GET http://192.168.4.1/stopslew
```

---

## Goto Control

### Goto RA Position
**Endpoint:** `GET /gotoRA`  
**Description:** Move mount to target RA position  

**Parameters:**
| Parameter | Type | Required | Description |
|-----------|------|----------|-------------|
| `currentRA` | string | Yes | Current RA position (HH:MM:SS format) |
| `targetRA` | string | Yes | Target RA position (HH:MM:SS format) |
| `speed` | integer | Yes | Goto speed multiplier (2-400, lower=faster) |

**Response:** `200 OK` - "Goto RA - Panning ON"

**Example:**
```
GET http://192.168.4.1/gotoRA?currentRA=12:30:45&targetRA=14:20:30&speed=8
```

### Abort Goto
**Endpoint:** `GET /abort-goto-ra`  
**Description:** Abort current goto RA operation  
**Response:** `200 OK` - "Goto RA aborted"

**Example:**
```
GET http://192.168.4.1/abort-goto-ra
```

---

## Position Management

### Set Position
**Endpoint:** `GET /setPosition`  
**Description:** Set current mount position  

**Parameters:**
| Parameter | Type | Required | Description |
|-----------|------|----------|-------------|
| `currentRA` | string | Yes | Current RA position (HH:MM:SS format) |

**Response:** `200 OK` - "Position Set Success"

**Example:**
```
GET http://192.168.4.1/setPosition?currentRA=12:30:45
```

### Get Current Position
**Endpoint:** `GET /getCurrentPosition`  
**Description:** Get current mount position in steps  

**Response:** `200 OK` - JSON object
```json
{
  "position": 123456
}
```

**Example:**
```
GET http://192.168.4.1/getCurrentPosition
```

---

## Intervalometer Control

### Configure and Start Capture
**Endpoint:** `GET /setCurrent`  
**Description:** Configure and start intervalometer capture sequence  

**Parameters:**
| Parameter | Type | Required | Description |
|-----------|------|----------|-------------|
| `mode` | integer | Yes | 0=start capture, 1=save to preset |
| `preset` | integer | Yes | Preset number (0-4) |
| `captureMode` | integer | Yes | 0=LONG_EXPOSURE_STILL, 1=LONG_EXPOSURE_MOVIE, 2=TIMELAPSE, 3=TIMELAPSE_PAN, |
| `exposureTime` | integer | Yes | Exposure duration in seconds |
| `exposures` | integer | Yes | Number of exposures |
| `preDelay` | integer | Yes | Pre-delay in seconds |
| `delay` | integer | Yes | Delay between exposures in seconds |
| `frames` | integer | Yes | Number of frames (for MOVIE mode) |
| `panAngle` | integer | Yes | Pan angle in degrees × 100 |
| `panDirection` | integer | Yes | 0=left/west, 1=right/east |
| `enableTracking` | integer | Yes | 0=off, 1=on |
| `ditherChoice` | integer | Yes | 0=off, 1=on |
| `ditherFrequency` | integer | Yes | Dither every N exposures |
| `focalLength` | integer | Yes | Focal length in mm |
| `pixelSize` | integer | Yes | Pixel size in microns × 100 |

**Response:** `200 OK` - Success message or error

**Example:**
```
GET http://192.168.4.1/setCurrent?mode=0&preset=0&captureMode=0&exposureTime=30&exposures=10&preDelay=5&delay=2&frames=1&panAngle=0&panDirection=1&enableTracking=1&ditherChoice=1&ditherFrequency=3&focalLength=200&pixelSize=350
```

### Read Preset
**Endpoint:** `GET /readPreset`  
**Description:** Load intervalometer preset settings  

**Parameters:**
| Parameter | Type | Required | Description |
|-----------|------|----------|-------------|
| `preset` | integer | Yes | Preset number (0-4) |

**Response:** `200 OK` - JSON object with preset settings
```json
{
  "mode": 0,
  "exposures": 10,
  "delay": 2,
  "preDelay": 5,
  "exposureTime": 30,
  "panAngle": 0,
  "panDirection": 1,
  "ditherChoice": 1,
  "ditherFrequency": 3,
  "enableTracking": 1,
  "frames": 1,
  "pixelSize": 350,
  "focalLength": 200
}
```

**Example:**
```
GET http://192.168.4.1/readPreset?preset=0
```

### Abort Capture
**Endpoint:** `GET /abort`  
**Description:** Abort current intervalometer capture sequence  
**Response:** `200 OK` - "Capture OFF"

**Example:**
```
GET http://192.168.4.1/abort
```

---

## Tracking Rates

### Get Tracking Rates
**Endpoint:** `GET /getTrackingRates`  
**Description:** Get current tracking rate configuration  

**Response:** `200 OK` - JSON object
```json
{
  "type": 0,
  "customRate": 0
}
```
**Types:** 0=SIDEREAL, 1=LUNAR, 2=SOLAR, 3=CUSTOM

**Example:**
```
GET http://192.168.4.1/getTrackingRates
```

### Save Tracking Rate Preset
**Endpoint:** `GET /saveTrackingRatePreset`  
**Description:** Save tracking rate to preset  

**Parameters:**
| Parameter | Type | Required | Description |
|-----------|------|----------|-------------|
| `preset` | integer | Yes | Preset number (0-4) |
| `type` | integer | Yes | 0=SIDEREAL, 1=LUNAR, 2=SOLAR, 3=CUSTOM |
| `customRate` | integer | No | Custom rate value (required if type=3) |

**Response:** `200 OK` - Success message

**Example:**
```
GET http://192.168.4.1/saveTrackingRatePreset?preset=0&type=0&customRate=0
```

### Load Tracking Rate Preset
**Endpoint:** `GET /loadTrackingRatePreset`  
**Description:** Load tracking rate from preset  

**Parameters:**
| Parameter | Type | Required | Description |
|-----------|------|----------|-------------|
| `preset` | integer | Yes | Preset number (0-4) |

**Response:** `200 OK` - JSON object with tracking rate settings

**Example:**
```
GET http://192.168.4.1/loadTrackingRatePreset?preset=0
```

---

## Status & Info

### Get Status
**Endpoint:** `GET /status`  
**Description:** Get device status information  

**Response:** `200 OK` - JSON object
```json
{
  "slewActive": false,
  "trackingActive": true,
  "intervalometerActive": false,
  "goToTarget": false,
  "exposuresTaken": 0,
  "currentExposure": 0
}
```

**Example:**
```
GET http://192.168.4.1/status
```

### Get Version
**Endpoint:** `GET /version`  
**Description:** Get firmware version and build information  

**Response:** `200 OK` - JSON object
```json
{
  "version": "v2.1",
  "buildDate": "Nov 22 2025 10:30:00",
  "internalVersion": 210
}
```

**Response Fields:**
| Field | Type | Description |
|-------|------|-------------|
| `version` | string | Firmware version string (from BUILD_VERSION define) |
| `buildDate` | string | Compilation date and time |
| `internalVersion` | integer | Internal version number for comparison |

**Example:**
```
GET http://192.168.4.1/version
```

**Notes:**
- `buildDate` format: "MMM DD YYYY HH:MM:SS" (e.g., "Nov 22 2025 10:30:00")
- `internalVersion` is a numeric value for programmatic version comparison
- Same data returned by `/checkversion` endpoint in OTA section

---

## Catalog Search

### Search Star/Object Catalog
**Endpoint:** `GET /starSearch`  
**Description:** Search star/object catalog by name  

**Parameters:**
| Parameter | Type | Required | Description |
|-----------|------|----------|-------------|
| `catalog` | integer | Yes | 0=NGC2000, 1=NGC2000_COMPACT, 2=BSC5, 3=BSC5_COMPACT |
| `query` | string | Yes | Search query string (star/object name) |

**Response:** `200 OK` - JSON object with search results

**Example:**
```
GET http://192.168.4.1/starSearch?catalog=0&query=M31
```

---

## Settings

### Set Language
**Endpoint:** `GET /setlang`  
**Description:** Set web interface language  

**Parameters:**
| Parameter | Type | Required | Description |
|-----------|------|----------|-------------|
| `lang` | integer | Yes | 0=English, 1=German, 2=Chinese |

**Response:** `200 OK` - Success message

**Example:**
```
GET http://192.168.4.1/setlang?lang=0
```

### Get Current Language
**Endpoint:** `GET /getlang`
**Description:** Get currently selected language index

**Response:** `200 OK` - JSON object
```json
{
  "lang": 0
}
```

**Example:**
```
GET http://192.168.4.1/getlang
```

### Get Language Strings
**Endpoint:** `GET /langstrings`
**Description:** Get all translated strings for current language as JSON

**Response:** `200 OK` - JSON object with language strings and language names
```json
{
  "strings": {
    "%STR_TRACKING%": "Tracking",
    "%STR_SLEWING%": "Slewing",
    ...
  },
  "langNames": ["English", "Deutsch", "中文"]
}
```

**Example:**
```
GET http://192.168.4.1/langstrings
```

**Notes:**
- Response is streamed using chunked encoding to reduce memory usage
- Contains all UI string placeholders and their translations
- Language names array used for building language selector dropdown

### Get Web Interface
**Endpoint:** `GET /`  
**Description:** Serve main web interface HTML  

**Response:** `200 OK` - HTML content

**Example:**
```
GET http://192.168.4.1/
```

---

## OTA Firmware Update

### Get OTA Update Page
**Endpoint:** `GET /ota`  
**Description:** Serve OTA firmware update web interface  

**Response:** `200 OK` - HTML content with firmware update interface

**Example:**
```
GET http://192.168.4.1/ota
```

---

### Check for Updates
**Endpoint:** `GET /checkversion`  
**Description:** Check GitHub for latest firmware release and compare with current version  

**Response:** `200 OK` - JSON object
```json
{
  "currentVersion": "v2.1",
  "buildDate": "Nov 22 2025 10:30:00",
  "latestVersion": "v2.1-beta03",
  "releaseUrl": "https://github.com/OG-star-tech/OG-star-tracker-/releases/tag/v2.1-beta03",
  "downloadUrl": "https://github.com/OG-star-tech/OG-star-tracker-/releases/download/v2.1-beta03/firmware.bin",
  "releaseNotes": "Bug fixes|Performance improvements|New features"
}
```

**Error Response:**
```json
{
  "currentVersion": "v2.1",
  "buildDate": "Nov 22 2025 10:30:00",
  "error": "No internet connection. If in AP mode, connect device to WiFi network with internet access."
}
```

**Example:**
```
GET http://192.168.4.1/checkversion
```

**Notes:**
- **Requires internet access:** Device must be connected to a WiFi network with internet access (not AP mode)
- **AP Mode Limitation:** When device is in Access Point mode, it cannot reach GitHub API. Connect the device to a WiFi network with internet access first
- Release notes are pipe-separated (|) for multi-line display
- Returns latest release even if same or older than current version

---

### Download and Install Update
**Endpoint:** `GET /downloadupdate`  
**Description:** Download firmware from URL and install automatically (GitHub release or custom URL)  

**Parameters:**
| Parameter | Type | Required | Description |
|-----------|------|----------|-------------|
| `url` | string | Yes | Direct URL to firmware.bin file (URL-encoded) |

**Response:** `200 OK` - "Starting download..."

**Example:**
```
GET http://192.168.4.1/downloadupdate?url=https%3A%2F%2Fgithub.com%2FOG-star-tech%2FOG-star-tracker-%2Freleases%2Fdownload%2Fv2.1-beta03%2Ffirmware.bin
```

**Process Flow:**
1. Server validates URL parameter and WiFi connection
2. Returns immediate response "Starting download..."
3. Downloads firmware in background (512-byte chunks)
4. Writes to flash partition with verification
5. Device reboots automatically after 6 seconds if successful

**Error Responses:**
- `400 Bad Request` - "Missing URL parameter"
- `500 Internal Server Error` - "WiFi not connected"

**Notes:**
- **Requires internet access:** Device must be connected to a WiFi network with internet access (not AP mode)
- Use `/otastatus` endpoint to monitor download progress
- Device stays responsive during download (polls every 500ms)
- Typical download time: 30-60 seconds for ~1.2MB firmware
- URL must point directly to `.bin` file

---

### Manual Firmware Upload
**Endpoint:** `POST /update`  
**Description:** Upload firmware binary file directly via HTTP POST (multipart/form-data)  

**Content-Type:** `multipart/form-data`

**Form Field:**
| Field | Type | Description |
|-------|------|-------------|
| `firmware` | file | Binary firmware file (.bin format) |

**Response:** `200 OK` - "Update Success! Rebooting..." or "Update Failed"

**Example (curl):**
```bash
curl -X POST -F "firmware=@firmware.bin" http://192.168.4.1/update
```

**Process Flow:**
1. Receives firmware chunks via multipart upload
2. Writes directly to flash partition during upload
3. Verifies integrity after upload completes
4. Device reboots automatically after 6 seconds if successful

**Notes:**
- Maximum file size: Limited by available flash partition (~2MB typical)
- Upload progress reported via `/otastatus` endpoint
- Use `/complete` endpoint to trigger reboot after upload verification

---

### Get OTA Status
**Endpoint:** `GET /otastatus`  
**Description:** Get real-time status of ongoing OTA update (upload or download)  

**Response:** `200 OK` - JSON object
```json
{
  "active": true,
  "complete": false,
  "error": false,
  "bytesWritten": 524288,
  "totalBytes": 1244160,
  "percent": 42
}
```

**Status Fields:**
| Field | Type | Description |
|-------|------|-------------|
| `active` | boolean | Update is actively downloading/uploading |
| `complete` | boolean | Update completed successfully, ready to reboot |
| `error` | boolean | Update failed (check serial output for details) |
| `bytesWritten` | integer | Bytes written to flash so far |
| `totalBytes` | integer | Total firmware size in bytes |
| `percent` | integer | Progress percentage (0-100) |

**Example:**
```
GET http://192.168.4.1/otastatus
```

**Polling Pattern:**
```javascript
// Poll every 500ms during update
const interval = setInterval(async () => {
  const response = await fetch('/otastatus');
  const data = await response.json();
  
  if (data.active) {
    console.log(`Progress: ${data.percent}%`);
  } else if (data.complete) {
    console.log('Update complete, device rebooting...');
    clearInterval(interval);
  } else if (data.error) {
    console.log('Update failed');
    clearInterval(interval);
  }
}, 500);
```

**Notes:**
- Poll during `/downloadupdate` or `/update` operations
- `active=false, complete=false, error=false` at 100% indicates verification phase
- Verification timeout: 5 seconds (frontend should show error after timeout)

---

### Complete OTA Update
**Endpoint:** `GET /complete`  
**Description:** Finalize OTA update and trigger device reboot (called automatically by frontend)  

**Response:** `200 OK` - "Update Success! Rebooting..." or "Update Failed"

**Example:**
```
GET http://192.168.4.1/complete
```

**Notes:**
- Checks for errors before rebooting
- 6-second delay before reboot (server stays responsive)
- Typically called automatically by OTA web interface
- Returns error status if update verification failed

---

### OTA Update Workflow

**Automatic Update (GitHub):**
```
1. GET /checkversion                    -> Get latest release info
2. User confirms update
3. GET /downloadupdate?url=<firmware>   -> Start download
4. Poll GET /otastatus every 500ms      -> Monitor progress (0-100%)
5. Verification phase (2-5 seconds)     -> active=false, complete waiting
6. GET /complete called automatically   -> Trigger reboot
7. Device reboots after 6 seconds
```

**Manual Upload:**
```
1. User selects .bin file
2. POST /update with multipart form     -> Upload begins
3. Poll GET /otastatus every 500ms      -> Monitor progress (0-100%)
4. Verification phase (2-5 seconds)     -> active=false, complete waiting
5. GET /complete called automatically   -> Trigger reboot
6. Device reboots after 6 seconds
```

**Error Handling:**
- Flash read failures detected during verification
- Timeout if verification exceeds 5 seconds
- Write errors reported immediately via `/otastatus` (error=true)
- Serial output provides detailed error messages

**Safety Features:**
- Server remains responsive during entire process (non-blocking delays)
- Progress tracking prevents UI from hanging
- Automatic rollback on write failures
- 6-second countdown gives user time to see status

---

## Error Responses

All endpoints may return error messages in plain text format with appropriate HTTP status codes:

- `400 Bad Request` - Invalid parameters
- `404 Not Found` - Endpoint not found
- `500 Internal Server Error` - Server error

---

## Notes

1. **RA Format:** All RA positions use HH:MM:SS format (e.g., "12:30:45")
2. **Scaled Values:** Some values are scaled by 100 (panAngle, pixelSize) to avoid floating point in URL parameters
3. **Direction Convention:** 0=left/west, 1=right/east for all directional parameters
4. **Speed Convention:** Lower speed values = faster movement (range: 2-400)
5. **Preset Numbering:** All presets are numbered 0-4 (5 presets total)

---

## Example Usage

### Python Example
```python
import requests

# Base URL of your star tracker
base_url = "http://192.168.4.1"

# Enable tracking
response = requests.get(f"{base_url}/on")
print(response.text)

# Start a 30-second exposure timelapse with 10 exposures
params = {
    "mode": 0,
    "preset": 0,
    "captureMode": 0,
    "exposureTime": 30,
    "exposures": 10,
    "preDelay": 5,
    "delay": 2,
    "frames": 1,
    "panAngle": 0,
    "panDirection": 1,
    "enableTracking": 1,
    "ditherChoice": 0,
    "ditherFrequency": 1,
    "focalLength": 200,
    "pixelSize": 350
}
response = requests.get(f"{base_url}/setCurrent", params=params)
print(response.text)

# Check status
response = requests.get(f"{base_url}/status")
print(response.json())
```

### JavaScript Example
```javascript
const baseUrl = "http://192.168.4.1";

// Enable tracking
fetch(`${baseUrl}/on`)
  .then(response => response.text())
  .then(data => console.log(data));

// Get status
fetch(`${baseUrl}/status`)
  .then(response => response.json())
  .then(data => console.log(data));

// Start slewing
fetch(`${baseUrl}/startslew?speed=16&direction=1`)
  .then(response => response.text())
  .then(data => console.log(data));
```

---

## Version History

- **v2.1.0-beta03** - Initial REST API documentation
  - Added TIMELAPSE_PAN mode
  - Improved position tracking
  - Enhanced error handling
  - Added OTA firmware update endpoints
  - Real-time update progress tracking
  - Automatic and manual update support

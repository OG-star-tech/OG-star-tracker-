#include "esp32-hal-gpio.h"
// #include <cstdlib>
#include "intervalometer.h"
#include "uart.h"

void intervalometer_ISA()
{
    intervalometer.nextState = true;
}

Intervalometer::Intervalometer(uint8_t triggerPinArg)
    : intervalometerTimer(2000, intervalometer_ISA)
{ // 2kHz resolution of 0.5 ms

    currentState = INACTIVE;
    currentErrorMessage = ERR_MSG_NONE;
    intervalometerTimer.stop();
    nextState = false;
    intervalometerActive = false;
    // readPresetsFromEEPROM();
    timerStarted = false;
    axisMoving = false;
    total = 0;
    triggerPin = triggerPinArg;
    currentSettings.mode = LONG_EXPOSURE_STILL;
    currentSettings.exposures = 1;
    currentSettings.delayTime = 1;
    currentSettings.preDelay = 1;
    currentSettings.exposureTime = 1;
    currentSettings.panAngle = 0.0;
    currentSettings.panDirection = true;
    currentSettings.dither = false;
    currentSettings.ditherFrequency = 1;
    currentSettings.enableTracking = false;
    currentSettings.frames = 1;
    currentSettings.pixelSize = 1.0;
    currentSettings.focalLength = 1;
    previousDitherDirection = 0;
}

void Intervalometer::abortCapture()
{
    currentState = INACTIVE;
}

void Intervalometer::startCapture()
{
    currentState = PRE_DELAY;
    intervalometerActive = true;
}

/* MODES:
LONG_EXPOSURE_STILL: Predelay>(Internal Timed Capture>Dither?>Delay (x exposures)) (with tracking)
LONG_EXPOSURE_MOVIE: (Predelay>(Internal Timed Capture>Dither?>Delay (x exposures))>rewind axis (x
frames)) (with tracking) DAY_TIME_LAPSE: Predelay>(Camera Capture>delay (x exposures)) (no tracking)
DAY_TIME_LAPSE_PAN: Predelay>(Camera Capture>pan deg>delay (x exposures)) (no tracking)
*/

void Intervalometer::run()
{
    switch (currentState)
    {
        case INACTIVE:
            print_out("Intervalometer: INACTIVE");
            ra_axis.stopSlew();
            intervalometerTimer.stop();
            digitalWrite(triggerPin, LOW);
            intervalometerActive = false;
            exposures_taken = 0;
            frames_taken = 0;
            timerStarted = false;
            axisMoving = false;
            nextState = false;
            ra_axis.counterActive = false;
            if (!currentSettings.enableTracking)
            {
                ra_axis.stopTracking();
            }
            break;
        case PRE_DELAY:

            if (!timerStarted)
            {
                print_out("Intervalometer: PREDELAY_START");
                if ((currentSettings.mode == DAY_TIME_LAPSE ||
                     currentSettings.mode == DAY_TIME_LAPSE_PAN) &&
                    ra_axis.trackingActive)
                {
                    ra_axis.stopTracking();
                }
                ra_axis.counterActive = currentSettings.mode == LONG_EXPOSURE_MOVIE ? true : false;
                intervalometerTimer.start(2000 * currentSettings.preDelay, false);
                timerStarted = true;
                intervalometerActive = true;
            }
            if (nextState)
            {
                intervalometerTimer.stop();
                print_out("Intervalometer: PREDELAY_STOP");
                nextState = false;
                timerStarted = false;
                currentState = CAPTURE;
            }
            break;
        case CAPTURE:
            if (!timerStarted)
            { // nightime modes
                print_out("Intervalometer: capture_start");
                if (currentSettings.mode == LONG_EXPOSURE_MOVIE && !ra_axis.counterActive)
                {
                    ra_axis.resetAxisCount();
                    ra_axis.counterActive = true;
                }
                digitalWrite(triggerPin, HIGH);
                if (currentSettings.mode > 1)
                {
                    intervalometerTimer.start(2000, false); // 1 sec should cover day time
                                                            // exposures.
                    vTaskDelay(10);
                    digitalWrite(triggerPin, LOW);
                }
                else
                {
                    intervalometerTimer.start(2000 * currentSettings.exposureTime, false);
                }
                timerStarted = true;
            }
            if (nextState)
            {
                digitalWrite(triggerPin, LOW);
                intervalometerTimer.stop();
                print_out("Intervalometer: capture_end");
                nextState = false;
                timerStarted = false;
                exposures_taken++;
                currentState = currentSettings.dither                       ? DITHER
                               : currentSettings.mode == DAY_TIME_LAPSE_PAN ? PAN
                                                                            : DELAY;
            }
            break;
        case DITHER:
            if (exposures_taken % currentSettings.ditherFrequency == 0)
            {
                if (!axisMoving)
                {
                    print_out("Intervalometer: dither_start");
                    axisMoving = true;
                    uint8_t randomDirection = biasedRandomDirection(previousDitherDirection);
                    previousDitherDirection = randomDirection;
                    int16_t stepsToDither =
                        ((random(100 * DITHER_DISTANCE_X10_PIXELS) + 1) / 100.0) *
                        getStepsPerTenPixels();
                    stepsToDither = randomDirection ? stepsToDither : stepsToDither * -1;
                    ra_axis.setAxisTargetCount(stepsToDither + ra_axis.axisCountValue);
                    if (ra_axis.targetCount != ra_axis.axisCountValue)
                    {
                        ra_axis.goToTarget = true;
                        ra_axis.counterActive = true;
                        ra_axis.startSlew(ra_axis.trackingRate / 3,
                                          randomDirection); // dither at 6 x tracking rate.
                    }
                }
                if (!ra_axis.slewActive)
                {
                    print_out("Intervalometer: dither_end");
                    if (currentSettings.mode != LONG_EXPOSURE_MOVIE)
                    {
                        ra_axis.counterActive = false;
                    }
                    axisMoving = false;
                    currentState = DELAY;
                }
            }
            else
            {
                currentState = DELAY;
            }
            break;
        case PAN:
            if (!axisMoving)
            {
                print_out("Intervalometer: pan_start");
                axisMoving = true;
                uint64_t arcSecsToMove = uint64_t(3600.0 * currentSettings.panAngle);
                int64_t stepsToMove = currentSettings.panDirection
                                          ? arcSecsToMove / ARCSEC_PER_STEP
                                          : (arcSecsToMove / ARCSEC_PER_STEP) * -1;
                ra_axis.resetAxisCount();
                ra_axis.setAxisTargetCount(stepsToMove);
                if (ra_axis.targetCount != ra_axis.axisCountValue)
                {
                    ra_axis.counterActive = true;
                    ra_axis.goToTarget = true;
                    ra_axis.startSlew(ra_axis.trackingRate / 10,
                                      currentSettings.panDirection); // pan at 20x tracking rate
                }
            }
            if (!ra_axis.slewActive)
            {
                print_out("Intervalometer: pan_end");
                axisMoving = false;
                ra_axis.counterActive = false;
                ra_axis.goToTarget = false;
                currentState = DELAY;
            }
            break;
        case DELAY:
            if (exposures_taken >= currentSettings.exposures)
            {
                currentState = currentSettings.mode != LONG_EXPOSURE_MOVIE ? INACTIVE
                               : frames_taken < currentSettings.frames     ? REWIND
                                                                           : INACTIVE;
            }
            else
            {
                if (!timerStarted)
                {
                    print_out("Intervalometer: delay_start");
                    intervalometerTimer.start(2000 * currentSettings.delayTime, false);
                    timerStarted = true;
                }
                if (nextState)
                {
                    intervalometerTimer.stop();
                    print_out("Intervalometer: delay_end");
                    nextState = false;
                    timerStarted = false;
                    currentState = CAPTURE;
                }
            }

            break;
        case REWIND:
            if (!axisMoving)
            {
                print_out("Intervalometer: rewind_start");
                axisMoving = true;
                exposures_taken = 0;
                frames_taken++;
                ra_axis.setAxisTargetCount(0);
                if (ra_axis.targetCount != ra_axis.axisCountValue)
                {
                    ra_axis.goToTarget = true;
                    ra_axis.startSlew(ra_axis.trackingRate / 10,
                                      !ra_axis.trackingDirection); // rewind at 20 x tracking rate.
                }
            }
            if (!ra_axis.slewActive)
            {
                print_out("Intervalometer: rewind_end");
                axisMoving = false;
                currentState = CAPTURE;
            }
            break;
    }
}

void Intervalometer::saveSettingsToPreset(uint8_t preset)
{
    presets[preset] = currentSettings;
    savePresetsToEEPPROM();
}

void Intervalometer::readSettingsFromPreset(uint8_t preset)
{
    currentSettings = presets[preset];
}

void Intervalometer::savePresetsToEEPPROM()
{
    print_out("writtenBytes: ");
    print_out("%d", writeObjectToEEPROM(PRESETS_EEPROM_START_LOCATION, presets));
}

void Intervalometer::readPresetsFromEEPROM()
{
    print_out("readBytes: ");
    print_out("%d", readObjectFromEEPROM(PRESETS_EEPROM_START_LOCATION, presets));
}

uint16_t Intervalometer::getStepsPerTenPixels()
{
    // add 0.5 to round up float to nearest int while casting
    return (int) (((getArcsec_per_pixel() * 10.0) / ARCSEC_PER_STEP) + 0.5);
}

float Intervalometer::getArcsec_per_pixel()
{
    return ((float) currentSettings.pixelSize / currentSettings.focalLength) * 206.265;
}

// when tracker moves left, next time its 5% higher chance tracked will move right
// with this tracker should keep in the middle in average
uint8_t Intervalometer::biasedRandomDirection(uint8_t previous_direction)
{
    // Adjust probabilities based on previous selection
    uint8_t direction_left_bias = previous_direction == 0 ? 45 : 55;
    uint8_t rand_val = random(100); // random number between 0 and 99
    uint8_t randomDirection = rand_val < direction_left_bias ? 0 : 1;
    return randomDirection;
}

template <class T> int Intervalometer::writeObjectToEEPROM(int address, const T& object)
{
    const byte* p = (const byte*) (const void*) &object;
    unsigned int i;
    for (i = 0; i < sizeof(object); i++)
    {
        // print_out("Address = ");
        // print_out(address);
        // print_out(", Data = ");
        // print_out(*p);
        EEPROM.write(address++, *p++);
        EEPROM.commit();
    }

    return i;
}

template <class T> int Intervalometer::readObjectFromEEPROM(int address, T& object)
{
    byte* p = (byte*) (void*) &object;
    unsigned int i;
    for (i = 0; i < sizeof(object); i++)
    {
        // print_out("Address = ");
        // print_out(address);
        // print_out(", Data = ");
        // print_out(EEPROM.read(address), HEX);
        *p++ = EEPROM.read(address++);
    }
    return i;
}
Intervalometer intervalometer(INTERV_PIN);

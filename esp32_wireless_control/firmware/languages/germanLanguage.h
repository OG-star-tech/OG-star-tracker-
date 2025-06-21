#ifndef GERMAN_LANG_H
#define GERMAN_LANG_H
#include "web_languages.h"

const char* germanLangLanguageNames[LANG_COUNT] = {
    // LANGUAGE NAMES
    "Englisch",   // ENGLISH
    "Chinesisch", // CHINESE
    "Deutsch",    // GERMAN
};

const char* germanLangMessageStrings[NUMBER_OF_MESSAGES] = {
    // MESSAGE STRINGS
    "Verfolgung Ein",                                   // MSG_TRACKING_ON
    "Verfolgung Aus",                                   // MSG_TRACKING_OFF
    "Schwenken",                                        // MSG_SLEWING
    "Schwenken Abgebrochen",                            // MSG_SLEW_CANCELLED
    "OK",                                               // MSG_OK
    "Voreinstellung Gespeichert",                       // MSG_SAVED_PRESET
    "VERFOLGUNG NICHT AKTIV! Bitte Verfolgung Starten", // MSG_TRACKING_NOT_ACTIVE
    "Aufnahme Ein",                                     // MSG_CAPTURE_ON
    "Aufnahme Aus",                                     // MSG_CAPTURE_OFF
    "Aufnahme Bereits Ein",                             // MSG_CAPTURE_ALREADY_ON
    "Aufnahme Bereits Aus",                             // MSG_CAPTURE_ALREADY_OFF
    "Aufnahme: Vorlaufzeit",                            // MSG_CAP_PREDELAY
    "Aufnahme: Belichten",                              // MSG_CAP_EXPOSING
    "Aufnahme: Zittern",                                // MSG_CAP_DITHER
    "Aufnahme: Schwenken",                              // MSG_CAP_PANNING
    "Aufnahme: Verzögern",                              // MSG_CAP_DELAY
    "Aufnahme: Zurückspulen",                           // MSG_CAP_REWIND
    "Goto RA: Panning ON",                              // MSG_GOTO_RA_PANNING_ON
    "Goto RA: Panning OFF",                             // MSG_GOTO_RA_PANNING_OFF
    "Leerlauf",                                         // MSG_IDLE
};

const char* germanLangErrorMessageStrings[NUMBER_OF_ERROR_MESSAGES] = {
    // ERROR MESSAGE STRINGS
    "Ungültiger Aufnahmemodus",         // ERR_MSG_INVALID_CAPTURE_MODE
    "Ungültige Belichtungszeit",        // ERR_MSG_INVALID_EXPOSURE_LENGTH
    "Ungültige Anzahl an Belichtungen", // ERR_MSG_INVALID_EXPOSURE_AMOUNT
    "Ungültige Vorlaufzeit",            // ERR_MSG_INVALID_PREDELAY_TIME
    "Ungültige Verzögerungszeit",       // ERR_MSG_INVALID_DELAY_TIME
    "Ungültige Bildmenge",              // ERR_MSG_INVALID_FRAME_AMOUNT
    "Ungültiger Schwenkwinkel",         // ERR_MSG_INVALID_PAN_ANGLE
    "Ungültige Schwenkrichtung",        // ERR_MSG_INVALID_PAN_DIRECTION
    "Ungültiger Aktivierungswert",      // ERR_MSG_INVALID_ENABLE_TRACKING_VALUE
    "Ungültige Zitterwahl",             // ERR_MSG_INVALID_DITHER_CHOICE
    "Ungültige Zitterfrequenz",         // ERR_MSG_INVALID_DITHER_FREQUENCY
    "Ungültige Brennweite",             // ERR_MSG_INVALID_FOCAL_LENGTH
    "Ungültige Pixelgröße"              // ERR_MSG_INVALID_PIXEL_SIZE
};

const char* germanLangHTMLStrings[numberOfHTMLStrings] = {
    // HTML STRINGS
    "OG Star Tracker Steuerungspanel",              // STR_TITLE
    "TRACKER STEUERUNG",                            // STR_TRACKER_CONTOLLER
    "Verfolgung",                                   // STR_TRACKING
    "Hemisphäre",                                   // STR_HEMISPHERE
    "Nord",                                         // STR_NORTH
    "Süd",                                          // STR_SOUTH
    "Verfolgungsrate",                              // STR_TRACKING_RATE
    "Sidereal",                                     // STR_SIDEREAL
    "Solar",                                        // STR_SOLAR
    "Lunar",                                        // STR_LUNAR
    "EIN",                                          // STR_BTN_ON
    "AUS",                                          // STR_BTN_OFF
    "Schwenksteuerung",                             // STR_SLEW_CONTROL
    "Geschwindigkeitsmultiplikator",                // STR_SPEED_MULTIPLIER
    "Benutzerdefiniert",                            // STR_CUSTOM
    "Benutzerdefinierte Geschwindigkeit",           // STR_CUSTOM_SPEED
    "Richtungstaste gedrückt halten zum Schwenken", // STR_SLEW_HINT
    "nach Links schwenken",                         // STR_SLEW_LEFT
    "nach Rechts schwenken",                        // STR_SLEW_RIGHT
    "Abbrechen",                                    // STR_ABORT_SLEW
    "INTERVALLAUSLÖSER",                            // STR_INTERVALOMETER
    "Voreinstellung",                               // STR_PRESET
    "VOREINSTELLUNG SPEICHERN",                     // STR_SAVE_PRESET
    "VOREINSTELLUNG LADEN",                         // STR_LOAD_PRESET
    "Modus",                                        // STR_MODE
    "Langzeitbelichtung Still",                     // STR_LONG_EXPOSURE_STILL
    "Langzeitbelichtung Film",                      // STR_LONG_EXPOSURE_MOVIE
    "Tageszeitraffer",                              // STR_DAY_TIME_LAPSE
    "Tageszeitraffer Schwenken",                    // STR_DAY_TIME_LAPSE_PAN
    "Belichtungsdauer",                             // STR_EXPOSURE_LENGTH
    "in Sekunden (z.B. 30)",                        // STR_EXPOSURE_HINT
    "Anzahl der Belichtungen",                      // STR_NO_EXPOSURES
    "Anzahl der Fotos (z.B. 20)",                   // STR_NUM_EXPOSURES_HINT
    "Vorlaufzeit (5s)",                             // STR_PRE_DELAY
    "Verzögerung",                                  // STR_DELAY
    "Anzahl der Bilder",                            // STR_FRAMES
    "nº der Bilder",                                // STR_NO_FRAMES
    "Schwenkwinkel",                                // STR_PAN_ANGLE
    "in Stunden (z.B. 6)",                          // STR_HOURS
    "Schwenkrichtung",                              // STR_PAN_DIRECTION
    "Links",                                        // STR_LEFT
    "Rechts",                                       // STR_RIGHT
    "Goto RA Control",                              // STR_GOTO_RA_CONTROL
    "Goto RA Aktuell",                              // STR_GOTO_RA_CURRENT
    "Goto RA Ziel",                                 // STR_GOTO_RA_TARGET
    "Richten Sie Ihre Kamera auf\n"
    "einen markanten Stern wie Deneb, Altair...\n"
    "und geben Sie dann die Koordinaten des Ziels ein\n", // STR_GOTO_RA_HINT
    "Start Goto RA",                                      // STR_START_GOTO_RA
    "Stop Goto RA",                                       // STR_STOP_GOTO_RA
    "Verfolgung am Ende Deaktivieren",                    // STR_ENABLE_TRACKING
    "Nein",                                               // STR_NO
    "Ja",                                                 // STR_YES
    "Zittern",                                            // STR_DITHER
    "Zitterfrequenz",                                     // STR_DITHER_FREQ
    "Brennweite des Objektivs",                           // STR_FOCAL_LENGTH
    "in Millimetern (z.B. 135)",                          // STR_FOCAL_LENGTH_HINT
    "Pixelgröße des Bildsensors",                         // STR_PIXEL_SIZE
    "in Mikrometern (z.B. 4.1)",                          // STR_PIXEL_SIZE_HINT
    "Aufnahme Starten",                                   // STR_START_CAPTURE
    "Aufnahme Abbrechen",                                 // STR_ABORT_CAPTURE
    "Aufnahmezeit",                                       // STR_CAPTURE_TIME
    "Status",                                             // STR_STATUS
    "Bereit",                                             // STR_STATUS_MSG
    "Firmware Version"                                    // STR_FIRMWARE_VERSION
};

#endif

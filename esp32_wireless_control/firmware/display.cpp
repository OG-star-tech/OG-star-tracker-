#include "config.h"

#if HAVE_DISPLAY

#include "display.h"
#include "uart.h"
#include "axis.h"
#include "intervalometer.h"
#include "web_languages.h"
#include <inttypes.h>

#include "bsc5/bsc5ra.h"

extern Languages language;

//#include "Adafruit_USB_RGB_Backpack.h"
//Adafruit_USB_RGB_Backpack lcd(Serial1);

Display display(SDA_PIN, SCL_PIN);

#include <LiquidCrystal_I2C.h> // Use an up to date library version, which has the init method
LiquidCrystal_I2C lcd(0x27, LCD_COLUMNS, LCD_ROWS);  // set the LCD address to 0x27 for a 16 chars and 2 line display

char line[LCD_COLUMNS+1];

Display::Display(uint8_t sda_pin, uint8_t scl_pin) : _sda_pin(sda_pin), _scl_pin(scl_pin)
{

}

void Display::begin()
{
//	Serial1.end();
//	Serial1.begin(19200, SERIAL_8N1, D5, D4);
//	while(!Serial1);
//    lcd.begin(LCD_COLUMNS, LCD_ROWS);
//    lcd.clear();
//    lcd.setBacklightRGBColor(255, 0, 0);


	Wire.end();
	Wire.begin(_sda_pin, _scl_pin);	// define SDA and SCL pins
	Wire.setClock(400000UL);
    lcd.init();
    lcd.clear();
	lcd.setBacklight(1);// Switch backlight LED on

    if (xTaskCreate(displayTask, "display", 4096, this, 1, NULL))
        print_out("started displayTask");
}

void Display::updateDisplay()
{
	lcd.setCursor(0, 0);
	for(int i = 0; i < LCD_COLUMNS; i++)
	{
		line[i] = ' ';
	}
	line[LCD_COLUMNS] = '\0';
	lcd.print(line);
	lcd.setCursor(0, 0);
	snprintf(line, LCD_COLUMNS+1, "%s", getCurrentStatusMessage());
	lcd.print(line);

	int64_t position = ra_axis.getPosition();
	lcd.setCursor(0, 1);
	int seconds = position/STEPS_PER_SECOND_256MICROSTEP;
	int milisec = (1000/STEPS_PER_SECOND_256MICROSTEP) * (position - STEPS_PER_SECOND_256MICROSTEP*seconds);
	int sec = seconds % 60;
	int min = (seconds / 60) % 60;
	int hour = seconds / 3600;
//	snprintf(line, LCD_COLUMNS+1, "RA:%s%02d %02d' %02d.%03d\"", seconds < 0 ? "-":" " , abs(hour), abs(min), abs(sec), abs(milisec));
	snprintf(line, LCD_COLUMNS+1, "%s%02d %02d' %02d.%03d\"", seconds < 0 ? "-":" " , abs(hour), abs(min), abs(sec), abs(milisec));
	lcd.print(line);

#if LCD_ROWS > 2
	static bool line2Cleared = false;
	if(intervalometer.intervalometerActive)
	{
		lcd.setCursor(0, 2);
		uint16_t exposures = intervalometer.currentSettings.exposures;
		uint16_t currentExposure = intervalometer.getCurrentExposure();
		uint16_t exposuresTaken = intervalometer.getExposuresTaken();
		snprintf(line, LCD_COLUMNS+1, "Exposures: %" PRIu16 "/%" PRIu16 "/%" PRIu16, exposuresTaken, currentExposure, exposures);
		lcd.print(line);

		line2Cleared = false;
	} else if(!line2Cleared){
		lcd.setCursor(0, 2);
		lcd.print("                    ");
		line2Cleared = true;
	}
#endif
#if LCD_ROWS > 3
	static bool line3Cleared = false;
	if(intervalometer.intervalometerActive)
	{
		lcd.setCursor(0, 3);
		TickType_t currentTicks = xTaskGetTickCount();
		TickType_t startCaptureTickCount = intervalometer.getStartCaptureTickCount();
		TickType_t captureDurationTickCount = intervalometer.getCaptureDurationTickCount();

		snprintf(line, LCD_COLUMNS+1, "Time: %.1f/%.1f",
				pdTICKS_TO_MS(currentTicks - startCaptureTickCount)/1000.0,
				pdTICKS_TO_MS(captureDurationTickCount)/1000.0);
		lcd.print(line);

		line3Cleared = false;
	} else if(!line3Cleared){
		lcd.setCursor(0, 3);
		lcd.print("                    ");
		line3Cleared = true;
	}
#endif
}

const char* Display::getCurrentStatusMessage()
{
    if (intervalometer.intervalometerActive)
    {
        switch (intervalometer.currentState)
        {
            case Intervalometer::PRE_DELAY:
            	return languageMessageStrings[language][MSG_CAP_PREDELAY];
                break;
            case Intervalometer::CAPTURE:
                return languageMessageStrings[language][MSG_CAP_EXPOSING];
                break;
            case Intervalometer::DITHER:
                return languageMessageStrings[language][MSG_CAP_DITHER];
                break;
            case Intervalometer::PAN:
                return languageMessageStrings[language][MSG_CAP_PANNING];
                break;
            case Intervalometer::DELAY:
                return languageMessageStrings[language][MSG_CAP_DELAY];
                break;
            case Intervalometer::REWIND:
                return languageMessageStrings[language][MSG_CAP_REWIND];
                break;
            case Intervalometer::INACTIVE:
            default:
                break;
        }
    }
    else if (ra_axis.slewActive && !ra_axis.goToTarget)
    {
        return languageMessageStrings[language][MSG_SLEWING];
    }
    else if (ra_axis.slewActive && ra_axis.goToTarget)
    {
        return languageMessageStrings[language][MSG_GOTO_RA_PANNING_ON];
    }
    else if (ra_axis.trackingActive)
    {
        return languageMessageStrings[language][MSG_TRACKING_ON];
    }
    else
    {
        if (intervalometer.currentErrorMessage == ErrorMessage::ERR_MSG_NONE)
            return languageMessageStrings[language][MSG_IDLE];
        else
            return languageErrorMessageStrings[language][intervalometer.currentErrorMessage];
    }
    return languageMessageStrings[language][MSG_IDLE];
}

void Display::displayTask(void* pvParameters)
{
	Display *disp = (Display *) pvParameters;
    for (;;)
    {
        disp->updateDisplay();
        vTaskDelay(500 * portTICK_PERIOD_MS);
    }
}

#endif /* HAVE_DISPLAY */

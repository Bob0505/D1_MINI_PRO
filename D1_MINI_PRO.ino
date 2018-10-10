/*
  ESP8266 BlinkWithoutDelay by Simon Peter
  Blink the blue LED on the ESP-01 module
  Based on the Arduino Blink without Delay example
  This example code is in the public domain

  The blue LED on the ESP-01 module is connected to GPIO1
  (which is also the TXD pin; so we cannot use Serial.print() at the same time)

  Note that this sketch uses LED_BUILTIN to find the pin with the internal LED
*/

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

int ledState = LOW;

uint32 previousMillis = 0;
uint16 interval = 300;

#if (SSD1306_LCDHEIGHT != 48)
#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif

// SCL GPIO5
// SDA GPIO4
#define OLED_RESET 0  // GPIO0
#define MAX_PIXEL_X 64	/* 10 word */
#define MAX_PIXEL_Y 48	/*  8 word*/

Adafruit_SSD1306 OLED(OLED_RESET);

void draw_Pixels()
{
	for(uint16 idx_x=0 ; idx_x<MAX_PIXEL_X ; idx_x+=5)
	{
		for(uint16 idx_y=0 ; idx_y<MAX_PIXEL_Y ; idx_y+=5)
		{
			OLED.drawPixel(idx_x, idx_y, WHITE);
		}
	}
}

void setup()
{
	pinMode(LED_BUILTIN, OUTPUT);

	Serial.begin(74880);
	while (!Serial) {
		; // wait for serial port to connect. Needed for Leonardo only
	}
	Serial.println("Hello World!!");

	OLED.begin(SSD1306_SWITCHCAPVCC, 0x3C);

	OLED.clearDisplay();
	OLED.setTextColor(WHITE);
	//draw_Pixels();

	OLED.setCursor(0,0);	OLED.setTextSize(1);/* 6x8 */
	OLED.println("[Bob&Kara]");
	
	OLED.display();
}

void loop()
{
	uint32 currentMillis = millis();

	if (currentMillis - previousMillis >= interval)
	{
		previousMillis = currentMillis;
		if (ledState == LOW)
		{
			ledState = HIGH;  // Note that this switches the LED *off*
		} else {
			ledState = LOW;  // Note that this switches the LED *on*
		}
		digitalWrite(LED_BUILTIN, ledState);
	}
}
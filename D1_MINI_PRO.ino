/*
  ESP8266 BlinkWithoutDelay by Simon Peter
  Blink the blue LED on the ESP-01 module
  Based on the Arduino Blink without Delay example
  This example code is in the public domain

  The blue LED on the ESP-01 module is connected to GPIO1
  (which is also the TXD pin; so we cannot use Serial.print() at the same time)

  Note that this sketch uses LED_BUILTIN to find the pin with the internal LED
*/
//#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_SHT31.h>

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

Adafruit_SHT31 sht31 = Adafruit_SHT31();
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

void DisHeader()
{
	OLED.setCursor(0,0);	OLED.setTextSize(1);/* 6x8 */
	OLED.println("[Bob&Kara]");
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

	OLED.clearDisplay();	OLED.setTextColor(WHITE);
	//draw_Pixels();
	DisHeader();
	OLED.display();
	
	if (! sht31.begin(0x45)) {   // Set to 0x44 for alternate i2c addr
		Serial.println("Couldn't find SHT31");
		//while (1) delay(1);
	}
}

void GetTempHumi(float* temp, float* humi)
{
	*temp = sht31.readTemperature();
	*humi = sht31.readHumidity();
}

void DisTempHumi(float temp, float humi)
{
	char tempstr[11];
	
	if (! isnan(temp))
	{  // check if 'is not a number'
		Serial.print("Temp *C = "); Serial.println(temp);
		sprintf(tempstr, "Temp: %.1f", temp);
	} else { 
		Serial.println("Failed to read temperature");
		sprintf(tempstr, "Temp: Error");
	}
	OLED.setCursor(0,1*8);	OLED.setTextSize(1);/* 6x8 */
	OLED.println(tempstr);

	if (! isnan(humi))
	{  // check if 'is not a number'
		Serial.print("Hum. % = "); Serial.println(humi);
		sprintf(tempstr, "Humi: %.1f", humi);
	} else { 
		Serial.println("Failed to read humidity");
		sprintf(tempstr, "Humi: Error");
	}
	OLED.setCursor(0,2*8);	OLED.setTextSize(1);/* 6x8 */
	OLED.println(tempstr);
}

void DisUpdate()
{
	float temp, humi;

	GetTempHumi(&temp, &humi);

	OLED.clearDisplay();	OLED.setTextColor(WHITE);
	DisHeader();
	DisTempHumi(temp, humi);
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

		DisUpdate();
	}
}
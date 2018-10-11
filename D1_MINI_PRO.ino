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
#include <Adafruit_BMP085.h>

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

Adafruit_SHT31  sht31 = Adafruit_SHT31();
Adafruit_BMP085 bmp;

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

	if (!bmp.begin()) {
		Serial.println("Could not find a valid BMP085 sensor, check wiring!");
		//while (1) {}
	}
}

void GetTempHumiPres(float* SHT_temp, float* SHT_humi, float* BMP_temp, int32_t* BMP_pres)
{	
	*SHT_temp = sht31.readTemperature();
	*SHT_humi = sht31.readHumidity();

	*BMP_temp = bmp.readTemperature();	//			C
	*BMP_pres = bmp.readPressure();		//Pressure	Pa
}

void ShowData(int16_t x_idx, int16_t y_idx, char* str, bool IsInt, float data, bool IsNextLine)
{
	char tempstr[11];

	if (! isnan(data))
	{  // check if 'is not a number'
		if(IsInt)
			sprintf(tempstr, "%s: %d", str, (int32_t)data);
		else
			sprintf(tempstr, "%s: %.1f", str, data);
	} else {
		sprintf(tempstr, "%s: Error", str);
	}

	Serial.print(tempstr);
	if(IsNextLine)
		Serial.print("\n");
	else
		Serial.print("\t");

	OLED.setCursor(6*x_idx, 8*y_idx);	OLED.setTextSize(1);/* 6x8 */
	OLED.println(tempstr);
}

void DisTempHumiPres(float SHT_temp, float SHT_humi, float BMP_temp, int32_t BMP_pres)
{
	char tempstr[11];
	char str[5];	// 4 char

	// === === SHT === ===
	ShowData( 0, 1, "STmp", false, SHT_temp, false);
	ShowData( 0, 2, "SHum", false, SHT_humi, true);
	// === === BMP === ===
	ShowData( 0, 3, "BTmp", false, BMP_temp, false);
	ShowData( 0, 4, "BPre", true,  ((float)BMP_pres/100), true);
}

void DisUpdate()
{
	float SHT_temp, SHT_humi, BMP_temp;
	int32_t BMP_pres;

	GetTempHumiPres(&SHT_temp, &SHT_humi, &BMP_temp, &BMP_pres);

	OLED.clearDisplay();	OLED.setTextColor(WHITE);
	DisHeader();
	DisTempHumiPres(SHT_temp, SHT_humi, BMP_temp, BMP_pres);
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
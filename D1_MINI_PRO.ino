/*
  ESP8266 BlinkWithoutDelay by Simon Peter
  Blink the blue LED on the ESP-01 module
  Based on the Arduino Blink without Delay example
  This example code is in the public domain

  The blue LED on the ESP-01 module is connected to GPIO1
  (which is also the TXD pin; so we cannot use Serial.print() at the same time)

  Note that this sketch uses LED_BUILTIN to find the pin with the internal LED
*/

int ledState = LOW;

uint32 previousMillis = 0;
uint16 interval = 300;

void setup()
{
	pinMode(LED_BUILTIN, OUTPUT);

	Serial.begin(74880);
	while (!Serial) {
		; // wait for serial port to connect. Needed for Leonardo only
	}
	Serial.println("Hello World!!");
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
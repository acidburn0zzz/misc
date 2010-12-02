#include "WProgram.h"

short ledPin = 13;      // LED connected to digital pin 13
short interval = 0;
long previousMillis = 0;
int ledState = LOW;

void setup() {                 // run once, when the sketch starts
    pinMode(ledPin, OUTPUT);    // sets the digital pin as output
    Serial.begin(57600);        // start serial port at 9600 bps
}

void loop() {
    if (Serial.available() > 0) {
        interval = Serial.read() * 4;
    }

    if (interval == 0)
        digitalWrite(ledPin, LOW);
    else if (millis() - previousMillis > interval) {
        // save the last time you blinked the LED 
        previousMillis = millis();   

        // if the LED is off turn it on and vice-versa:
        if (ledState == LOW)
            ledState = HIGH;
        else
            ledState = LOW;

        digitalWrite(ledPin, ledState);
    }
}

int main() {
	init();
	setup();
    
	for (;;)
		loop();
    
	return 0;
}

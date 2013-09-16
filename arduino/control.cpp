unsigned char ledPin = 9; //Change to a pin supporting PWM ie. 3, 5, 6, 9-11
unsigned char brightness = 0;

void setup() {                 // run once, when the sketch starts
    pinMode(ledPin, OUTPUT);    // sets the digital pin as output
    Serial.begin(57600);        // start serial port at 9600 bps
}

void loop() {
    if (Serial.available() > 0) {
        brightness = Serial.read();
    }

    analogWrite(ledPin, brightness);
}

int main() {
	init();
	setup();
    
	for (;;)
		loop();
    
	return 0;
}

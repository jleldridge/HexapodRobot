/***************************************************
This is an example for our Adafruit 16-channel PWM & Servo driver
Servo test - this will drive 16 servos, one after the other

Pick one up today in the adafruit shop!
------> http://www.adafruit.com/products/815

These displays use I2C to communicate, 2 pins are required to
interface. For Arduino UNOs, thats SCL -> Analog 5, SDA -> Analog 4

Adafruit invests time and resources providing this open source code,
please support Adafruit and open-source hardware by purchasing
products from Adafruit!

Written by Limor Fried/Ladyada for Adafruit Industries.
BSD license, all text above must be included in any redistribution
****************************************************/

#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

// called this way, it uses the default address 0x40
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();
// you can also call it with a different address you want

#define SERVOMIN  275 // this is the 'minimum' pulse length count (out of 4096)
#define SERVOMAX  525 // this is the 'maximum' pulse length count (out of 4096)
#define SERVOMID 400

int servoNum = 0;
int servos[] = { 0, 1, 2, 3, 4, 5, 8, 9, 10, 11, 12, 13 };
int servoCount = 12;

void setup() {
	Serial.begin(9600);
	Serial.println("16 channel Servo test!");

	pwm.begin();

	pwm.setPWMFreq(60);  // Analog servos run at ~60 Hz updates
}

// you can use this function if you'd like to set the pulse length in seconds
// e.g. setServoPulse(0, 0.001) is a ~1 millisecond pulse width. its not precise!
void setServoPulse(uint8_t n, double pulse) {
	double pulselength;

	pulselength = 1000000;   // 1,000,000 us per second
	pulselength /= 60;   // 60 Hz
	Serial.print(pulselength); Serial.println(" us per period");
	pulselength /= 4096;  // 12 bits of resolution
	Serial.print(pulselength); Serial.println(" us per bit");
	pulse *= 1000;
	pulse /= pulselength;
	Serial.println(pulse);
	pwm.setPWM(n, 0, pulse);
}

void loop() {
	centerServos();
	delay(2000);
	maxServos();
	delay(2000);
	minServos();
	delay(2000);
}

void centerServos() {
	for (int i = 0; i < servoCount; i++) {
		pwm.setPWM(servos[i], 0, SERVOMID);
	}
}

void maxServos() {
	for (int i = 0; i < servoCount; i++) {
		pwm.setPWM(servos[i], 0, SERVOMAX);
	}
}

void minServos() {
	for (int i = 0; i < servoCount; i++) {
		pwm.setPWM(servos[i], 0, SERVOMIN);
	}
}







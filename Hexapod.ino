#include <Wire.h>
#include <IRLib.h>
#include <Adafruit_PWMServoDriver.h>

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

#define SELECT_BUTTON 3344
#define LEFT_ARROW    720 
#define RIGHT_ARROW   3280
#define UP_ARROW      752 
#define DOWN_ARROW    2800

#define SERVOMIN  275
#define SERVOMAX  525
#define SERVOMID 400
#define KNEEMID 425
#define STRIDEMAX 100

int servoNum = 0;
int servos[] = { 
	2, 3,
	4, 5,
	14, 15,
	8, 9,
	6, 7,
	0, 1};
int servoCount = 12;

int angle;                                                 // determines the direction/angle (0°-360°) that the robot will walk in 
int rotate;                                                // rotate mode: -1 = anticlockwise, +1 = clockwise, 0 = no rotation
int speed;                                                 // walking speed: -15 to +15 
int stride;                                                // size of step: exceeding 400 may cause the legs to hit each other
int step = 0;

IRrecv My_Receiver(2);//Receive on pin 2
IRdecode My_Decoder;

void setup() 
{	
	Serial.begin(9600);	

	My_Receiver.No_Output();//Turn off any unused IR LED output circuit
	My_Receiver.enableIRIn(); // Start the receiver

	pwm.begin();
	pwm.setPWMFreq(60);

	speed = 0;
	angle = 30;
	rotate = 0;

	Serial.println("Initialized...");
	Serial.println("Ready to decode IR!");
}

void loop() 
{
	readIR();
	walk();
}

void readIR() 
{
	if (My_Receiver.GetResults(&My_Decoder)) {
		My_Decoder.decode();
		Serial.println(My_Decoder.value);
		switch (My_Decoder.value) {
			Serial.println(My_Decoder.value);
			case SELECT_BUTTON:
				if (speed == 0)
				{
					speed = 1;
				}
				else 
				{
					speed = 0;
				}
				break;
			case LEFT_ARROW:    
				speed = 1;
				angle = 300;
				rotate = 0;
				break;
			case RIGHT_ARROW:   
				speed = 1;
				angle = 120;
				rotate = 0;
				break;
			case UP_ARROW:
				speed = 1;
				angle = 30;
				rotate = 0;
				break;
			case DOWN_ARROW:
				speed = 1;
				angle = 210;
				rotate = 0;
				break;
		}
		My_Receiver.resume();
	}
}

void centerServos() 
{
	for (int i = 0; i < servoCount; i++) 
	{
		pwm.setPWM(servos[i], 0, SERVOMID);
	}
}

void maxServos() 
{
	for (int i = 0; i < servoCount; i++) 
	{
		pwm.setPWM(servos[i], 0, SERVOMAX);
	}
}

void minServos() 
{
	for (int i = 0; i < servoCount; i++) 
	{
		pwm.setPWM(servos[i], 0, SERVOMIN);
	}
}


void walk() 
{
	if (speed == 0)
	{
		stride -= 25;
		if (stride<0) stride = 0;
	}
	else
	{
		stride += 25;
		if (stride > STRIDEMAX) stride = STRIDEMAX;
	}

	walkSingleLeg(0);
	//walkSingleOddLeg(2);
	walkSingleLeg(4);
	//walkSingleOddLeg(6);
	walkSingleLeg(8);
	//walkSingleOddLeg(10);

	step += speed;
	if (step>359)
		step -= 360;
	if (step<0)
		step += 360;
}

void walkSingleLeg(int kneeIndex)
{
	float A;
	double Xa, Knee, Hip;

	// angle of leg on the body + angle of travel
	A = float(60 * kneeIndex + angle);
	// keep value within 0°-360°
	if (A > 359) 
		A -= 360;

	// convert degrees to radians for SIN function
	A = A*PI / 180;

	// Xa value for rotation
	Xa = stride*rotate;
    // hip movement affected by walking angle
	if (rotate == 0)
	{
		// Xa hip position multiplier for walking at an angle
		Xa = sin(A)*-stride;
	}

	// angle of leg
	A = float(step);
	// convert degrees to radians for SIN function
	A = A*PI / 180;
	Knee = sin(A)*stride;
	Hip = cos(A)*Xa;

	pwm.setPWM(servos[kneeIndex], 0, KNEEMID + int(Knee));
	pwm.setPWM(servos[kneeIndex + 1], 0, SERVOMID + int(Hip));
}

void walkSingleOddLeg(int kneeIndex)
{
	float A;
	double Xa, Knee, Hip;

	// angle of leg on the body + angle of travel
	A = float(60 * kneeIndex + angle);
	// keep value within 0°-360°
	if (A > 359)
		A -= 360;

	// convert degrees to radians for SIN function
	A = A*PI / 180;

	// Xa value for rotation
	Xa = stride*rotate;
	// hip movement affected by walking angle
	if (rotate == 0)
	{
		// Xa hip position multiplier for walking at an angle
		Xa = sin(A)*-stride;
	}

	A = float(step + 180);
	if (A>359) A -= 360;                                      
	// convert degrees to radians for SIN function
	A = A*PI / 180;
	Knee = sin(A)*stride;
	Hip = cos(A)*Xa;

	pwm.setPWM(servos[kneeIndex], 0, KNEEMID + int(Knee));
	pwm.setPWM(servos[kneeIndex + 1], 0, SERVOMID + int(Hip));
}




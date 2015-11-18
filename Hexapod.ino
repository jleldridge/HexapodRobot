#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

#define SERVOMIN  275 // this is the 'minimum' pulse length count (out of 4096)
#define SERVOMAX  525 // this is the 'maximum' pulse length count (out of 4096)
#define SERVOMID 400

int servoNum = 0;
int servos[] = { 0, 1, 2, 3, 12, 13, 8, 9, 4, 5, 10, 11};
int servoCount = 12;

int angle;                                                 // determines the direction/angle (0°-360°) that the robot will walk in 
int rotate;                                                // rotate mode: -1 = anticlockwise, +1 = clockwise, 0 = no rotation
int Speed;                                                 // walking speed: -15 to +15 
int Stride;                                                // size of step: exceeding 400 may cause the legs to hit each other

void setup() 
{
	Serial.begin(9600);	
	pwm.begin();
	pwm.setPWMFreq(60);
	Serial.println("Initialized...");
}

void loop() 
{
	Speed = 3;
	angle = 0;
	rotate = 0;
	delay(50);
	
	walkSingleLeg(0);
	walkSingleOddLeg(2);
	walkSingleLeg(4);
	walkSingleOddLeg(6);
	walkSingleLeg(8);
	walkSingleOddLeg(10);
	//centerServos();
	//delay(2000);
	//maxServos();
	//delay(2000);
	//minServos();
	//delay(2000);
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

void walkSingleLeg(int kneeIndex)
{
	if (Speed == 0)
	{
		Stride -= 25;
		if (Stride<0) Stride = 0;
	}
	else
	{
		Stride += 25;
		if (Stride>100) Stride = 100;
	}

	float A;
	double Xa, Knee, Hip;
	static int Step;

	// angle of leg on the body + angle of travel
	A = float(60 * kneeIndex + angle);
	// keep value within 0°-360°
	if (A > 359) 
		A -= 360;

	// convert degrees to radians for SIN function
	A = A*PI / 180;

	// Xa value for rotation
	Xa = Stride*rotate;
    // hip movement affected by walking angle
	if (rotate == 0)
	{
		// Xa hip position multiplier for walking at an angle
		Xa = sin(A)*-Stride;
	}

	// angle of leg
	A = float(Step);
	// convert degrees to radians for SIN function
	A = A*PI / 180;
	Knee = sin(A)*Stride;
	Hip = cos(A)*Xa;

	pwm.setPWM(servos[kneeIndex], 0, SERVOMID + int(Knee));
	pwm.setPWM(servos[kneeIndex + 1], 0, SERVOMID + int(Hip));
	Serial.println(Knee);
	Serial.println(Hip);

	Step += Speed;
	if (Step>359) 
		Step -= 360;
	if (Step<0)
		Step += 360;
}

void walkSingleOddLeg(int kneeIndex)
{
	if (Speed == 0)
	{
		Stride -= 25;
		if (Stride<0) Stride = 0;
	}
	else
	{
		Stride += 25;
		if (Stride>100) Stride = 100;
	}

	float A;
	double Xa, Knee, Hip;
	static int Step;

	// angle of leg on the body + angle of travel
	A = float(60 * kneeIndex + angle);
	// keep value within 0°-360°
	if (A > 359)
		A -= 360;

	// convert degrees to radians for SIN function
	A = A*PI / 180;

	// Xa value for rotation
	Xa = Stride*rotate;
	// hip movement affected by walking angle
	if (rotate == 0)
	{
		// Xa hip position multiplier for walking at an angle
		Xa = sin(A)*-Stride;
	}

	A = float(Step + 180);
	if (A>359) A -= 360;                                      
	// convert degrees to radians for SIN function
	A = A*PI / 180;
	Knee = sin(A)*Stride;
	Hip = cos(A)*Xa;

	pwm.setPWM(servos[kneeIndex], 0, SERVOMID + int(Knee));
	pwm.setPWM(servos[kneeIndex + 1], 0, SERVOMID + int(Hip));
	Serial.println(Knee);
	Serial.println(Hip);

	Step += Speed;
	if (Step>359)
		Step -= 360;
	if (Step<0)
		Step += 360;
}

//void Walk()
//{
//	if (Speed == 0)                                             // return all legs to default position when stopped
//	{
//		Stride -= 25;                                            // as Stride aproaches 0, all servos return to center position
//		if (Stride<0) Stride = 0;                                 // do not allow negative values, this would reverse movements
//	}
//	else                                                     // this only affects the robot if it was stopped
//	{
//		Stride += 25;                                            // slowly increase Stride value so that servos start up smoothly
//		if (Stride>450) Stride = 450;                             // maximum value reached, prevents legs from colliding.
//	}
//
//	float A;                                                 // temporary value for angle calculations
//	double Xa, Knee, Hip;                                      // results of trigometric functions
//	static int Step;                                         // position of legs in circular motion from 0° to 360°                               
//
//	for (int i = 0; i<6; i += 2)                                    // calculate positions for odd numbered legs 1,3,5
//	{
//		A = float(60 * i + angle);                                   // angle of leg on the body + angle of travel
//		if (A>359) A -= 360;                                      // keep value within 0°-360°
//
//		A = A*PI / 180;                                            // convert degrees to radians for SIN function
//
//		Xa = Stride*rotate;                                      // Xa value for rotation
//		if (rotate == 0)                                          // hip movement affected by walking angle
//		{
//			Xa = sin(A)*-Stride;                                   // Xa hip position multiplier for walking at an angle
//		}
//
//		A = float(Step);                                         // angle of leg
//		A = A*PI / 180;                                            // convert degrees to radians for SIN function
//		Knee = sin(A)*Stride;
//		Hip = cos(A)*Xa;
//
//		sv[i * 2].writeMicroseconds(svc[i * 2] + int(Knee));         // update knee  servos 1,3,5
//		sv[i * 2 + 1].writeMicroseconds(svc[i * 2 + 1] + int(Hip));      // update hip servos 1,3,5
//	}
//
//	for (int i = 1; i<6; i += 2)                                    // calculate positions for even numbered legs 2,4,6
//	{
//		A = float(60 * i + angle);                                   // angle of leg on the body + angle of travel
//		if (A>359) A -= 360;                                      // keep value within 0°-360°
//
//		A = A*PI / 180;                                            // convert degrees to radians for SIN function
//		Xa = Stride*rotate;                                      // Xa value for rotation
//		if (rotate == 0)                                          // hip movement affected by walking angle
//		{
//			Xa = sin(A)*-Stride;                                   // Xa hip position multiplier for walking at an angle
//		}
//
//		A = float(Step + 180);                                     // angle of leg
//		if (A>359) A -= 360;                                      // keep value within 0°-360°
//		A = A*PI / 180;                                            // convert degrees to radians for SIN function
//		Knee = sin(A)*Stride;
//		Hip = cos(A)*Xa;
//
//		sv[i * 2].writeMicroseconds(svc[i * 2] + int(Knee));         // update knee  servos 2,4,6
//		sv[i * 2 + 1].writeMicroseconds(svc[i * 2 + 1] + int(Hip));      // update hip servos 2,4,6
//	}
//
//	Step += Speed;                                             // cycle through circular motion of gait
//	if (Step>359) Step -= 360;                                 // keep value within 0°-360°
//	if (Step<0) Step += 360;                                   // keep value within 0°-360°
//}







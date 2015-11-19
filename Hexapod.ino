#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

#define SERVOMIN  275
#define SERVOMAX  525
#define SERVOMID 400
#define KNEEMID 425
#define STRIDEMAX 100

int servoNum = 0;
int servos[] = { 0, 1, 2, 3, 12, 13, 8, 9, 4, 5, 10, 11};
int servoCount = 12;

int angle;                                                 // determines the direction/angle (0°-360°) that the robot will walk in 
int rotate;                                                // rotate mode: -1 = anticlockwise, +1 = clockwise, 0 = no rotation
int speed;                                                 // walking speed: -15 to +15 
int stride;                                                // size of step: exceeding 400 may cause the legs to hit each other
int step = 0;

void setup() 
{
	Serial.begin(9600);	
	pwm.begin();
	pwm.setPWMFreq(60);
	Serial.println("Initialized...");
}

void loop() 
{
	speed = 10;
	angle = 0;
	rotate = 0;

	walk();
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
	walkSingleOddLeg(2);
	walkSingleLeg(4);
	walkSingleOddLeg(6);
	walkSingleLeg(8);
	walkSingleOddLeg(10);
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
	Serial.println(Knee);
	Serial.println(Hip);

	step += speed;
	if (step>359) 
		step -= 360;
	if (step<0)
		step += 360;
}

void walkSingleOddLeg(int kneeIndex)
{
	float A;
	double Xa, Knee, Hip;
	static int step;

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
	Serial.println(Knee);
	Serial.println(Hip);

	step += speed;
	if (step>359)
		step -= 360;
	if (step<0)
		step += 360;
}

//void Walk()
//{
//	if (speed == 0)                                             // return all legs to default position when stopped
//	{
//		stride -= 25;                                            // as stride aproaches 0, all servos return to center position
//		if (stride<0) stride = 0;                                 // do not allow negative values, this would reverse movements
//	}
//	else                                                     // this only affects the robot if it was stopped
//	{
//		stride += 25;                                            // slowly increase stride value so that servos start up smoothly
//		if (stride>450) stride = 450;                             // maximum value reached, prevents legs from colliding.
//	}
//
//	float A;                                                 // temporary value for angle calculations
//	double Xa, Knee, Hip;                                      // results of trigometric functions
//	static int step;                                         // position of legs in circular motion from 0° to 360°                               
//
//	for (int i = 0; i<6; i += 2)                                    // calculate positions for odd numbered legs 1,3,5
//	{
//		A = float(60 * i + angle);                                   // angle of leg on the body + angle of travel
//		if (A>359) A -= 360;                                      // keep value within 0°-360°
//
//		A = A*PI / 180;                                            // convert degrees to radians for SIN function
//
//		Xa = stride*rotate;                                      // Xa value for rotation
//		if (rotate == 0)                                          // hip movement affected by walking angle
//		{
//			Xa = sin(A)*-stride;                                   // Xa hip position multiplier for walking at an angle
//		}
//
//		A = float(step);                                         // angle of leg
//		A = A*PI / 180;                                            // convert degrees to radians for SIN function
//		Knee = sin(A)*stride;
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
//		Xa = stride*rotate;                                      // Xa value for rotation
//		if (rotate == 0)                                          // hip movement affected by walking angle
//		{
//			Xa = sin(A)*-stride;                                   // Xa hip position multiplier for walking at an angle
//		}
//
//		A = float(step + 180);                                     // angle of leg
//		if (A>359) A -= 360;                                      // keep value within 0°-360°
//		A = A*PI / 180;                                            // convert degrees to radians for SIN function
//		Knee = sin(A)*stride;
//		Hip = cos(A)*Xa;
//
//		sv[i * 2].writeMicroseconds(svc[i * 2] + int(Knee));         // update knee  servos 2,4,6
//		sv[i * 2 + 1].writeMicroseconds(svc[i * 2 + 1] + int(Hip));      // update hip servos 2,4,6
//	}
//
//	step += speed;                                             // cycle through circular motion of gait
//	if (step>359) step -= 360;                                 // keep value within 0°-360°
//	if (step<0) step += 360;                                   // keep value within 0°-360°
//}







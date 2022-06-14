#ifndef SERVOCONTROLER_H
#define SERVOCONTROLER_H

#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

class ServoControler{

	public :
		
		ServoControler();
		void init();
		void SetRotation(int ServoNum, float PercentRotation);
	
	protected :
		
		Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();
		
	private :
	
		int SERVOMIN0 = 142;
		int SERVOMAX0 = 426;
		int SERVOMIN1 = 152;
		int SERVOMAX1 = 414;
		int SERVO_FREQ = 50;

};

#endif

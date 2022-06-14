#include "ServoControler.h"
#include <Arduino.h>

ServoControler::ServoControler()
{

}

void ServoControler::init()
{
	pwm.begin();
	pwm.setOscillatorFrequency(27000000);
	pwm.setPWMFreq(SERVO_FREQ);
}

void ServoControler::SetRotation(int ServoNum, float PercentRotation)
{
	float pwmSignal;
	if (ServoNum)
	{
		pwmSignal = (SERVOMAX1-SERVOMIN1)*PercentRotation*(1./100)+SERVOMIN1;
	}
	
	else
	{
		pwmSignal = (SERVOMAX0-SERVOMIN0)*PercentRotation*(1./100)+SERVOMIN0;
	}
	 
	pwm.setPWM(ServoNum,0,pwmSignal);
}


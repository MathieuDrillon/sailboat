#include "RCControler.h"
#include <Arduino.h>
//#include <ServoInput.h>

RCControler::RCControler() 
{

}

void RCControler::init()
{
	const int SteeringPulseMin = 1000;
	const int SteeringPulseMax = 2000;
		
	const int ThrottlePulseMin = 1000;
	const int ThrottlePulseMax = 2000;
		
	pinMode(SteeringSignalPin,INPUT);
	pinMode(ThrottleSignalPin,INPUT);		
	
	m_steering.setRange(SteeringPulseMin,SteeringPulseMax);
	m_throttle.setRange(ThrottlePulseMin,ThrottlePulseMax);

}

bool RCControler::isEnabled()
{
	return (m_steering.read() || m_throttle.read());
}
		
double RCControler::get_throttle()
{
	return m_throttle.map(-100, 100);
}

double RCControler::get_steering()
{
	return 90.0 - m_steering.getAngle();
}

int RCControler::getSteeringPin()
{
	return SteeringSignalPin;
}
		
int RCControler::getThrottlePin()
{
	return ThrottleSignalPin;
}

float RCControler::getThrottleMax()
{
	return ThrottleMax;
}

float RCControler::getThrottleMin()
{
	return ThrottleMin;
}
float RCControler::getSteeringMax()
{
	return SteeringMax;
}
float RCControler::getSteeringMin()
{
	return SteeringMin;
}











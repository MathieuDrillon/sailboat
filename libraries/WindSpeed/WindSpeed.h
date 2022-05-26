#ifndef WINDSPEED_H
#define WINDSPEED_H

#include <math.h>

class WindSpeed
{
	public :
	
		WindSpeed();
		void init();
		void Update();
		double getSpeed();
		void isr_rotation();
		int getWindSpeedPin();
	
	protected :
		
		int WindSpeedPin = 19;
		volatile unsigned long Rotations;
		volatile unsigned long ContactBounceTime;
		volatile unsigned long waitedTime;
		volatile double VRot; 
		float waitingTime = 1500; // in miliseconds 
};


#endif

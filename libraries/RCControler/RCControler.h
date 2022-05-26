#ifndef RCCONTROLER_H
#define RCCONTROLER_H

#include <ServoInput.h>

class RCControler{

	public :
		
		RCControler();
		void init();
		double get_throttle();
		double get_steering();
		int getSteeringPin();
		int getThrottlePin();
		float getThrottleMax();
		float getThrottleMin();
		float getSteeringMax();
		float getSteeringMin();
		
		
	protected :
		
		const int static SteeringSignalPin = 3;
		const int static ThrottleSignalPin = 2;
		
		ServoInputPin<SteeringSignalPin> m_steering;
		ServoInputPin<ThrottleSignalPin> m_throttle;
		
	private :
	
		float ThrottleMax = 65.0;
		float ThrottleMin = -69.0;
		float SteeringMax = 75.0;
		float SteeringMin = -80.0;
};

#endif

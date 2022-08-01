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
		bool isEnabled();
		
		
	protected :
		
		const int static SteeringSignalPin = 3;
		const int static ThrottleSignalPin = 2;
		
		ServoInputPin<SteeringSignalPin> m_steering;
		ServoInputPin<ThrottleSignalPin> m_throttle;
		
	private :
	
		float ThrottleMax = 64.0;
		float ThrottleMin = -72.0;
		float SteeringMax = 88.0;
		float SteeringMin = -65.0;
};

#endif

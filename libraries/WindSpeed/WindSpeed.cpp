#include "WindSpeed.h"
#include <Arduino.h>

WindSpeed::WindSpeed()
{

}

void WindSpeed::init()
{
	pinMode(WindSpeedPin, INPUT);
	//attachInterrupt(digitalPinToInterrupt(WindSpeedPin), isr_rotation, FALLING);
	sei();
}

double WindSpeed::getSpeed()
{
	double windSpeed = 1.207*VRot;
	return windSpeed;
}

void WindSpeed::Update()
{
	if ((millis() - waitedTime) > 4000)
	{
		Rotations = 0;
		VRot = 0;
	}
}

void WindSpeed::isr_rotation() {

if ((millis() - ContactBounceTime) > 15 ) { // debounce the switch contact.
Rotations++;
ContactBounceTime = millis();
}

if ((millis() - waitedTime) > waitingTime)
{
	VRot = Rotations*1000/waitingTime;
	Rotations = 0;
	waitedTime = millis();
}
}

int WindSpeed::getWindSpeedPin()
{
	return WindSpeedPin;
}





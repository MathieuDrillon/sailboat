#include "WindDirection.h"
#include <Arduino.h>

WindDirection::WindDirection()
{

}

void WindDirection::init()
{
	LastValue = 1;
}

void WindDirection::Update()
{
	VaneValue = analogRead(A14);
	Direction = map(VaneValue, 0, 1023, 0, 360);
	CalDirection = Direction + Offset;
	
	if(CalDirection > 360)
		CalDirection = CalDirection - 360;

	if(CalDirection < 0)
		CalDirection = CalDirection + 360;
		
}

int WindDirection::getHeading()
{
	return CalDirection;
}

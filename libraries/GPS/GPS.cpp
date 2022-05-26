#include "GPS.h"
#include <Arduino.h>

GPS::GPS()
{

}

void GPS::init()
{
	Serial2.begin(9600);
}

void GPS::Update()
{
	while (Serial2.available() > 0)
		if (gps.encode(Serial2.read()))
		{
			if (gps.location.isValid())
			{
				Lat = gps.location.lat();
				Long = gps.location.lng(); 
			}
			else 
			{
				//Serial.println("INVALID DATA");
				Lat = 1.111111;
				Long = 9.999999;
			}
		}
	if (millis() > 5000 && gps.charsProcessed() < 10)
  {
    Serial.println(F("No GPS detected: check wiring."));
    while(true);
  }
}

double GPS::getLat()
{
	return Lat;
}

double GPS::getLong()
{
	return Long;
}

double GPS::getSpeed()
{
	if (gps.speed.isValid())
	{
		return gps.speed.kmph();
	}
}

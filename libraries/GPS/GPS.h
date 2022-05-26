#ifndef GPS_H
#define GPS_H

#include <TinyGPS++.h>
#include <Vector.h>

class GPS
{
	public :
	
		GPS();
		void init();
		void Update();
		double getLat();
		double getLong();
		double getSpeed();
		
	protected :
	
		TinyGPSPlus gps;
		double Lat, Long;
};


#endif

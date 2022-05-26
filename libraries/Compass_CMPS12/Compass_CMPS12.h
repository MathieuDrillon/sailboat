#ifndef COMPASS_CMPS12_H
#define COMPASS_CMPS12_H

#include<SoftwareSerial.h>


class Compass
{
	public :
	
		Compass();
		void init();
		unsigned int getAngle8();
		unsigned int getAngle16();
		double getPitch();
		double getRoll();
		
	protected :
	
		
		unsigned char high_byte, low_byte, angle8_char;
		char pitch_char, roll_char;
		unsigned int angle16, angle8;
		double pitch, roll, angle16_high, angle16_low; 
		
		SoftwareSerial CMPS12 = SoftwareSerial(10,11);
	
	private :
	
		int CMPS_GET_ANGLE8 = 0x12;
		int CMPS_GET_ANGLE16 = 0x13;
		int CMPS_GET_PITCH = 0x14;
		int CMPS_GET_ROLL = 0x15;

};



#endif

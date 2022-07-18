#ifndef COMPASS_CMPS12_H
#define COMPASS_CMPS12_H

#include<SoftwareSerial.h>

#define CMPS_GET_ANGLE8 0x12
#define CMPS_GET_ANGLE16 0x13
#define CMPS_GET_PITCH 0x14
#define CMPS_GET_ROLL 0x15

class Compass
{
	public :
	
		Compass();
		void init();
		unsigned int getAngle8();
		double getAngle16();
		double getPitch();
		double getRoll();
		void calibrate();
		
	protected :
	
		
		unsigned char high_byte, low_byte, angle8_char;
		char pitch_char, roll_char;
		unsigned int angle16_raw, angle8;
		double pitch, roll, angle16, angle16_high, angle16_low; 
		
		SoftwareSerial CMPS12 = SoftwareSerial(10,11);
		
		bool calYaw16 = false;
		bool calYaw8 = false;
		bool calPitch = false;
		bool calRoll = false;
	
	//private :

};



#endif

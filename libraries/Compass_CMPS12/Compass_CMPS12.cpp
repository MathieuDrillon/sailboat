#include "Compass_CMPS12.h"
#include <Arduino.h>


Compass::Compass()
{

}

void Compass::init()
{
	CMPS12.begin(9600);
}

double Compass::getAngle16()
{
	CMPS12.write(CMPS_GET_ANGLE16);
	while(CMPS12.available() < 2);
	high_byte = CMPS12.read();
	low_byte = CMPS12.read();
	angle16_raw = high_byte;
	angle16_raw <<= 8;
	angle16_raw += low_byte;
	angle16_high = angle16_raw / 10;
	angle16_low = angle16_raw % 10;
	angle16 = angle16_high + 0.1*angle16_low;
	/*Serial.print("    angle full: ");
  	Serial.print(angle16_high, DEC);
  	Serial.print(".");
 	Serial.print(angle16_low, DEC);
	Serial.print("angle16_high : ");
	Serial.print(angle16_high);
	Serial.print("angle16_low : ");
	Serial.println(angle16_low);*/
	return angle16;
}

unsigned int Compass::getAngle8()
{
	CMPS12.write(CMPS_GET_ANGLE8);
	while(CMPS12.available() < 1);
	angle8_char = CMPS12.read();
	angle8 = angle8_char;
	return angle8;
}

double Compass::getPitch()
{
	CMPS12.write(CMPS_GET_PITCH);
	while(CMPS12.available() < 1);
	pitch_char = CMPS12.read();
	pitch = pitch_char;
	return pitch;
}

double Compass::getRoll()
{
	CMPS12.write(CMPS_GET_ROLL);
	while(CMPS12.available() < 1);//{Serial.println("hello");}
	roll_char = CMPS12.read();
	roll = roll_char;
	return roll;
}

#include "Compass_CMPS12.h"
#include <Arduino.h>


Compass::Compass()
{

}

void Compass::init()
{
	CMPS12.begin(9600);
}

unsigned int Compass::getAngle16()
{
	CMPS12.write(CMPS_GET_ANGLE16);
	while(CMPS12.available() < 2);
	high_byte = CMPS12.read();
	low_byte = CMPS12.read();
	angle16 <<= 8;
	angle16 += low_byte;
	angle16_high = angle16 / 10;
	angle16_low = angle16 % 10;
	/*Serial.print("angle16_high : ");
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
	angle8 = angle8_char - '0';
	return angle8;
}

double Compass::getPitch()
{
	CMPS12.write(CMPS_GET_PITCH);
	while(CMPS12.available() < 1);
	pitch_char = CMPS12.read();
	pitch = pitch_char - '0';
	return pitch;
}

double Compass::getRoll()
{
	CMPS12.write(CMPS_GET_ROLL);
	while(CMPS12.available() < 1);//{Serial.println("hello");}
	roll_char = CMPS12.read();
	roll = roll_char - '0';
	return roll;
}

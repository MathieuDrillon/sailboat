#include "Compass_CMPS12.h"
#include <Arduino.h>


Compass::Compass()
{

}

void Compass::init()
{
	CMPS12.begin(9600);
}

void Compass::calibrate()
{
	if (calYaw16 && calYaw8 && calPitch && calRoll)
	{
		Serial.println("deleting current calibration...");
		CMPS12.write(0xE0); 
		while(CMPS12.available() < 1){Serial.println("waiting delete 1...");}
		Serial.print("CMPS12.read() delete 1 : ");
		Serial.println(CMPS12.read(), HEX);
		delay(20);
		CMPS12.write(0xE5);
		while(CMPS12.available() < 1){Serial.println("waiting delete 2...");}
		Serial.print("CMPS12.read() delete 2 : ");
		Serial.println(CMPS12.read(), HEX);
		delay(20);
		CMPS12.write(0xE2);
		while(CMPS12.available() < 1){Serial.println("waiting delete 3...");}
		Serial.print("CMPS12.read() delete 3 : ");
		Serial.println(CMPS12.read(), HEX);
		Serial.println("deleted");
		delay(50);
		
	
		CMPS12.write(0x24);
		while(CMPS12.available() < 1){Serial.println("waiting state...");}
		int state = CMPS12.read();
		state >>= 4;
		while(state != 3)
		{
			CMPS12.write(0x24);
			while(CMPS12.available() < 1){Serial.println("waiting state...");}
			Serial.print("calibration state : ");
			Serial.println(state, BIN);
			state = CMPS12.read();
			state >>= 4;
			delay(50);
		}
		Serial.print("calibration state : ");
		Serial.println(state);
		Serial.println("CMPS12 fully calibrated !");
		
		
		Serial.println("storing the calibration...");
		CMPS12.write(0xF0);
		while(CMPS12.available() < 1){Serial.println("waiting calibration 1...");}
		Serial.print("CMPS12.read() calibration 1 : ");
		Serial.println(CMPS12.read(), HEX);
		delay(20);
		CMPS12.write(0xF5);
		while(CMPS12.available() < 1){Serial.println("waiting calibration 2...");}
		Serial.print("CMPS12.read() calibration 2 : ");
		Serial.println(CMPS12.read(), HEX);
		delay(20);
		CMPS12.write(0xF6);
		while(CMPS12.available() < 1){Serial.println("waiting calibration 3...");}
		Serial.print("CMPS12.read() calibration 3 : ");
		Serial.println(CMPS12.read(), HEX);
		Serial.println("done");
	}
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
	if (!calYaw16){calYaw16 = true;}
	return angle16;
}

unsigned int Compass::getAngle8()
{
	CMPS12.write(CMPS_GET_ANGLE8);
	while(CMPS12.available() < 1);
	angle8_char = CMPS12.read();
	angle8 = angle8_char;
	if (!calYaw8){calYaw8 = true;}
	return angle8;
}

double Compass::getPitch()
{
	CMPS12.write(CMPS_GET_PITCH);
	while(CMPS12.available() < 1);
	pitch_char = CMPS12.read();
	pitch = pitch_char;
	if (!calPitch){calPitch = true;}
	return pitch;
}

double Compass::getRoll()
{
	CMPS12.write(CMPS_GET_ROLL);
	while(CMPS12.available() < 1);//{Serial.println("hello");}
	roll_char = CMPS12.read();
	roll = roll_char;
	if (!calRoll){calRoll = true;}
	return roll;
}

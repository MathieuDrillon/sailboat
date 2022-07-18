#include "Xbee.h"
#include <Arduino.h>


Xbee::Xbee()
{

}

void Xbee::init()
{
	XBEE.begin(9600);
}


void Xbee::Print(void* param)
{
	XBEE.print(*param)
}

char Xbee::Read()
{
	return XBEE.read()
}

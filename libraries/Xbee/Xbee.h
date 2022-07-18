#ifndef XBEE_H
#define XBEE_H

#include<SoftwareSerial.h>

class Xbee
{
	public :
		Xbee();
		void init();
		void Print();
		char Read();
		
	protected :

		SoftwareSerial XBEE = SoftwareSerial(4,5);
	
	//private :

};



#endif

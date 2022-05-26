#ifndef WINDDIRECTION_H
#define WINDDIRECTION_H

class WindDirection
{
	public :
	 
		WindDirection();
		void init();
		void Update();
		int getHeading();
	
	protected :
	
		int VaneValue;
		int Direction;
		int CalDirection;
		int LastValue;
		int Offset = 0;

};


#endif

#ifndef SIMU_H
#define SIMU_H

#include <math.h>

struct COORD_XY {
	double x = 0;
	double y = 0;
};

struct X_VECTOR {
	double x = 0; 
	double y = 0;
	double theta = 0;
	double v = 0;
};

class Controler{

	public :
		
		Controler();
		void Reg(double a[2], double b[2], double psi);
	
		X_VECTOR get_x();	
		void set_x(double pos_x, double pos_y, double theta, double v);
		//void set_line(double a[2], double b[2]);
		double get_deltasmax();
		double get_deltar();

		
	protected :
		
		X_VECTOR X;
		
		int q;
		
		double deltar;
		double deltasmax;
		
		float zeta = M_PI/4;
		float r = 5;
		double m_theta, m_pos_x, m_pos_y, m_v, m_w; 
		
};

#endif

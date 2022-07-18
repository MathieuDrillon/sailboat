#ifndef SIMU_H
#define SIMU_H

#include <math.h>

class Controler{

	public :
		
		Controler();
		void Reg(double a[2], double b[2], double psi);
	
		//int *get_x();	
		void set_x(double pos_x, double pos_y, double theta, double v);
		//void set_line(double a[2], double b[2]);
		double get_deltasmax();
		double get_deltar();
		
		double x[5];
		
	protected :
		
		int q;
		
		double deltar;
		double deltasmax;
		
		float zeta = M_PI/4;
		float r = 5;
		double m_theta, m_pos_x, m_pos_y, m_v, m_w; 
		
};

#endif

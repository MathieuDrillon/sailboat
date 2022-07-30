#include "Controler.h"
#include <Arduino.h>
#include <math.h>

double norm(double x[2])
{
	double x0_2 = x[0]*x[0];
	double x1_2 = x[1]*x[1];
	return sqrt(x0_2+x1_2);
}

//determinant d'une matrice 2*2 avec mat = [a,b,c,d] ~ [[a,b],[c,d]]
double det(double mat[4]) 
{
	double a,b,c,d;
	a = mat[0]; b = mat[1]; c = mat[2]; d = mat[3];
	
	return (a*d)-(b*c);
}

Controler::Controler()
{

}

X_VECTOR Controler::get_x()
{
	return X;
}


double Controler::get_deltasmax()
{
	return deltasmax;
}

		
double Controler::get_deltar()
{
	return deltar;
}

void Controler::set_x(double pos_x, double pos_y, double theta, double v)
{
	X.x = pos_x; X.y = pos_y; X.theta = theta; X.v = v;
}

void Controler::Reg(double a[2], double b[2], double psi)
{
	m_theta = X.theta;
	m_pos_x = X.x;
	m_pos_y = X.y;
	double b_a[2] = {b[0]-a[0],b[1]-a[1]};
	double norme = norm(b_a);
	double matr[] = {b_a[0]/norme, m_pos_x-a[0], b_a[1]/norme, m_pos_y-a[1]};
	double e = det(matr);
	double phi = atan2(b_a[1],b_a[0]);
	if (abs(e) > r)
	{
		q = e/abs(e);
	}
	double theta_bar = phi-atan(e/r);
	if (cos(psi-theta_bar)+cos(zeta) < 0)
	{
		theta_bar = M_PI + psi - zeta*q;
	}
	deltar = (2/M_PI)*atan(tan(0.5*(m_theta-theta_bar))); //returns value between -1 and 1
	deltasmax = (M_PI/4)*(cos(psi-theta_bar)+1); //returns value between 0 and PI/2
}

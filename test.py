import matplotlib.pyplot as plt
import numpy as np

print("np.arccos(-1) : ", np.arccos(-1), "np.arccos(1) : ", np.arccos(1))


def f(theta_deg):
	theta = np.pi*theta_deg*1./180
	sin_theta = np.sin(theta)
	cos_theta = np.cos(theta)
	if sin_theta == 0:
		sign = 1
	else : 
		sign = np.abs(sin_theta)*1./sin_theta
	return 180*(1 + sign*(np.arccos(cos_theta)/np.pi -1))
	
def sawtooth(theta_deg):
	theta = np.pi*theta_deg*1./180
	return 180.*2*np.arctan(np.tan(theta/2))/np.pi
	
print("f(0) : ", f(0), "f(360) : ", f(360), "f(180) : ", f(180))


thetas = np.arange(0,720,0.01)

#a = np.sqrt(3)/2
#b = np.sqrt(2)/2
#c = 1./2

#sin_thetas = [-1,-a,-b,-c,0,c,b,a,1]
#colors = ["red", "green", "yellow", "orange", "black", "blue", "purple", "pink", "lime"] 

#Y = []
#for i in (sin_thetas):
#	Y.append(f(cos_thetas,i))

Y1 = [f(i) for i in thetas]
Y2 = [sawtooth(i) for i in thetas]
#for i in range(len(sin_thetas)):
#	plt.plot(cos_thetas,Y[i],colors[i])

plt.plot(thetas, Y1, "red")
plt.plot(thetas, Y2, "lime")


plt.show() 

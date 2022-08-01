from roblib import *
import serial
import numpy as np

ser = serial.Serial()
ser.baudrate = 9600
ser.port = '/dev/ttyUSB0'
ser.timeout = 1
ser.open()

def strToTabChar(string):
	tab = []
	for i in range(len(string)):
		tab.append(string[i])
	return tab

def decompose(tab_str):
	numbers = []
	number = ""
	justHadASpace = True
	tab_str = strToTabChar(tab_str)
	tab_str.append(' ')
	for char in tab_str:
		if char != " ":
			number += char
			justHadASpace = False
		elif (char == " ") & (~justHadASpace):
			numbers.append(float(number))
			number = ""
			justHadASpace = True
	return np.array(numbers)
	
init = True

while(1):
	current_line = ser.readline()
	if init:
		throttle,steering,yaw,pitch,roll,windSpeed,lat,lon,heading,delta_s_max, deltar = 0,0,0,0,0,0,0,0,0,0,0
		init = False
	try :
		current_line_d = current_line.decode('utf-8')
		#print("current line : ", current_line_d)
		#getting RC data
		if current_line_d[:2] == "RC":
			if current_line_d[3] == "O":
				throttle = -1
				steering = -1
			else :
				if current_line_d[3] =="t":
					throttle = float(current_line_d[13:])
				elif current_line_d[3] =="s":
					steering = float(current_line_d[13:])
		elif current_line_d[:3] == "com":
			numbers = current_line_d[16:]
			numbers = decompose(numbers)
			yaw, pitch, roll = numbers[0], numbers[1], numbers[2]
		elif current_line_d[:5] == "windS":
			numbers = current_line_d[11:]
			numbers = decompose(numbers)
			windSpeed = numbers[0]
		elif current_line_d[:3] == "lat":
			numbers = current_line_d[10:]
			numbers = decompose(numbers)
			lat, lon = numbers[0], numbers[1]
		elif current_line_d[:3] == "hea":
			numbers = current_line_d[9:]
			numbers = decompose(numbers)
			heading = numbers[0]
		elif current_line_d[:6] == "deltas":
			numbers = current_line_d[12:]
			numbers = decompose(numbers)
			delta_s_max = numbers[0]
		elif current_line_d[:6] == "deltar":
			numbers = current_line_d[9:]
			numbers = decompose(numbers)
			deltar = numbers[0]
		print("throttle : ", throttle, "  steering : ", steering)
		print("compass : ", yaw, " ", pitch, " ", roll)
		print("windSpeed : ", windSpeed, "  heading : ", heading)
		print("gps : ", lat, " ", lon)
		print("delta_s_max : ", delta_s_max, "  deltar : ", deltar)
			
	except UnicodeDecodeError:
		print("erreur : ", current_line)


#compass va l u e s   :   9 . 0 0     6     - 1 . 0 0     5 . 0 0
#windSpeed :   0 . 0 0 
#lat/long :  1 . 1 1     1 0 . 0 0 
#deltasmax :   12
#deltar : 14
#RC throttle : 
#heading : 2 6
#012345678910111213141516171819202122232425262728293031323334353637

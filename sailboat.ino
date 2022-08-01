#include "RCControler.h"
#include "ServoControler.h"
#include "WindDirection.h"
#include "WindSpeed.h"
#include "Compass_CMPS12.h"
#include "GPS.h"
#include "Controler.h"

#include <Vector.h>
#include <arduino-timer.h>
#include <SPI.h>
#include <SD.h>


RCControler RC;
ServoControler servo;
WindDirection windD;
WindSpeed windS;
Compass compass;
GPS Gps;
Controler controler;

//--------------------
//RC variables
float throttleMax = RC.getThrottleMax();
float throttleMin = RC.getThrottleMin();
float steeringMax = RC.getSteeringMax();
float steeringMin = RC.getSteeringMin();

double throttle;
double steering;

double throttlePercent;
double steeringPercent;

Vector<double> Throttles;
Vector<double> Steerings;

int ThrottlesPosMax;
int SteeringsPosMax;
int ThrottlesPosMin;
int SteeringsPosMin;

//wind variables
int heading;
double windSpeed;

//compass variables
unsigned int yaw8;
double yaw16, pitch, roll;

//GPS variables
double Lat, Long, Speed;
double posX, posY;
COORD_XY Coords;

//---------------------

double throttlePercentPrecPrec = (RC.get_throttle()-throttleMin)*100/(throttleMax-throttleMin);
double steeringPercentPrecPrec = (RC.get_steering()-steeringMin)*100/(steeringMax-steeringMin);
double throttlePercentPrec = (RC.get_throttle()-throttleMin)*100/(throttleMax-throttleMin);
double steeringPercentPrec = (RC.get_steering()-steeringMin)*100/(steeringMax-steeringMin);

int headingPrec = windD.getHeading();

SoftwareSerial Xbee = SoftwareSerial(4,5);

//--------------------
//Logs options
const int chipSelect = 53;

File GPSLog;
File WHLog;
File WSLog;
File RCLog;
File CMPSLog;

bool RewriteLogs = true;

//-------------------

bool printHeading;

bool RCOn;

//-------------------

//line following
COORD_XY coords0;
double a[2] = {0,0};
double b[2] = {10,7};















//-------------------------------------------------------------------------------



















Timer<8, millis, void *> timer;


void isr_rotation_arduino()
{
  windS.isr_rotation();
};

int posMax(Vector<double> vec)
{ 
  int n = vec.size();
  int pos_max = 0;
  double maximum = vec[0];
  for (int i = 0; i < n; i++)
  {
    if (vec[i] > maximum)
    {
      maximum = vec[i];
      pos_max = i  ;
    }  
  }
  return pos_max;
}

int posMin(Vector<double> vec)
{ 
  int n = vec.size();
  int pos_min = 0;
  double minimum = vec[0];
  for (int i = 0; i < n; i++)
  {
    if (vec[i] < minimum)
    {
      minimum = vec[i];  
      pos_min = i;
    }  
  }
  return pos_min;
}


void Print(Vector<double> vec)
{
  int n = vec.size();
  Serial.print("<");
  for (int i = 0; i < n-1; i++)
  {
    Serial.print(vec[i]);
    Serial.print(",");
  }
  Serial.print(vec[n-1]);
  Serial.print(">");  
}


void Println(Vector<double> vec)
{
  int n = vec.size();
  Serial.print("<");
  for (int i = 0; i < n-1; i++)
  {
    Serial.print(vec[i]);
    Serial.print(",");
  }
  Serial.print(vec[n-1]);
  Serial.println(">");  
}


void LogsInit()
{
  GPSLog = SD.open("GPSLog.txt", FILE_WRITE);

  GPSLog.println("Those are the GPS logs with lat (left) and long (right) next to each other");
  GPSLog.println(" ");
  GPSLog.println(" ");

  GPSLog.close();

  delay(100);

  if (!SD.exists("GPSLog.txt")){Serial.println("GPSLog doesn't exist mais ptn pourquoiiiii");}

  WHLog = SD.open("WHLog.txt", FILE_WRITE);
  
  WHLog.println("Those are the WindHeading logs");
  WHLog.println(" ");
  WHLog.println(" ");
  
  WHLog.close();

  delay(100);

  if (!SD.exists("WHLog.txt")){Serial.println("WHLog doesn't exist mais ptn pourquoiiiii");}

  WSLog = SD.open("WSLog.txt", FILE_WRITE);
  
  WSLog.println("Those are the WindSpeed logs in IS units");
  WSLog.println(" ");
  WSLog.println(" ");
  
  WSLog.close();

  delay(100);
  
  if (!SD.exists("WSLog.txt")){Serial.println("WSLog doesn't exist mais ptn pourquoiiiii");}

  RCLog = SD.open("RCLog.txt", FILE_WRITE);
  
  RCLog.println("Those are the RC logs with throttle (left) and steering (right) next to each other");
  RCLog.println(" ");
  RCLog.println(" ");

  RCLog.close();

  delay(100);
  
  if (!SD.exists("RCLog.txt")){Serial.println("RCLog doesn't exist mais ptn pourquoiiiii");}


  CMPSLog = SD.open("CMPSLog.txt", FILE_WRITE);
  
  CMPSLog.println("Those are the Compass logs with yaw encoded on 16bits, then yaw on 8 bits, then pitch and eventually roll");
  CMPSLog.println(" ");
  CMPSLog.println(" ");
  
  CMPSLog.close();

  delay(100);

  if (!SD.exists("CMPSLog.txt")){Serial.println("CMPSLog doesn't exist mais ptn pourquoiiiii");}

}

COORD_XY GPSToCart0(double Latitude, double Longitude)
{
  double R = 6371000;
  
  double x_cart = R*cos(Latitude)*cos(Longitude);
  double y_cart = R*cos(Latitude)*sin(Longitude);  

  COORD_XY CoordsCart;
  CoordsCart.x = x_cart;
  CoordsCart.y = y_cart;

  return CoordsCart;
}

COORD_XY GPSToCart(double Latitude, double Longitude)
{
  double R = 6371000;
  
  double x_cart = R*cos(Latitude)*cos(Longitude);
  double y_cart = R*cos(Latitude)*sin(Longitude);  

  COORD_XY CoordsCart;
  CoordsCart.x = x_cart - coords0.x;
  CoordsCart.y = y_cart - coords0.y;

  return CoordsCart;
}

double TrigoToTheta(double cosTheta, double sinTheta)
{
  int sign;
  if (sinTheta == 0){sign = 1;}
  else
  {
    sign = abs(sinTheta)/sinTheta;
  }

  return 180*(1 + sign*(acos(cosTheta)/M_PI - 1));
}

double RealHeading(double ThetaBoatDeg, double FalseHeadingDeg)
{
  double degToRad = M_PI/180;
  double ThetaBoat = degToRad*ThetaBoatDeg;
  double FalseHeading = degToRad*FalseHeadingDeg;
  double cosPsi = cos(ThetaBoat)*cos(FalseHeading) - sin(ThetaBoat)*sin(FalseHeading);
  double sinPsi = sin(ThetaBoat)*cos(FalseHeading) + cos(ThetaBoat)*sin(FalseHeading);
  return TrigoToTheta(cosPsi, sinPsi);
}
















//-------------------------------------------------------------------------------

















bool printAll(void *)
{
  //printing the RCCommande
  
  Serial.print(throttlePercent);
  Serial.print("%      ");
  Serial.print(steeringPercent);
  Serial.print("%      ");
  
  //-------------------------

  Serial.print("  --  ");

  //printing the heading or not

  if (printHeading)
  {
    Serial.print(heading);
  }
  else
  {
    Serial.print("   ");   
  }

  //------------------------

  Serial.print("  --  ");

  //printing Windspeed
  
  Serial.print(windSpeed);

  //------------------------

  Serial.print("  --  ");

  //printing Compass values

  Serial.print(roll);
  Serial.print("  --  ");
  Serial.print(pitch);
  Serial.print("  --  ");
  Serial.print(yaw8);
  Serial.print("  --  ");
  Serial.print(yaw16, DEC);

  //----------------------

  Serial.print("  --  ");

  //printing GPS values

  
  Serial.print(Lat,6);
  Serial.print("  --  ");
  Serial.println(Long,6);

  return true;
}



bool getRCToControlServos(void *)
{
  RCLog = SD.open("RCLog.txt", FILE_WRITE);
  
  if(RCOn)
  {
    double StorageArrayThr[4];
    double StorageArraySte[4];
  
  //initiate the Vectors
  
    Throttles.setStorage(StorageArrayThr);
    Steerings.setStorage(StorageArraySte);
  
  //get 4 values, spaced by 25ms each
    
    for (int i = 0; i < 4; i++)
    {
      Throttles.push_back(RC.get_throttle());
      Steerings.push_back(RC.get_steering());
      if (i < 3){delay(25);}     
    }
  
  //get the extremums in order to get rid of it and thus of non accurate values
  
    int maxiThr = posMax(Throttles);
    int miniThr = posMin(Throttles);
    
    Throttles.remove(maxiThr);
    Throttles.remove(miniThr);
  
  //mean of the other two values to smoothen
  
    throttle = (Throttles[0]+Throttles[1])/2;
  
  //same for the steering
    
    Steerings.remove(posMax(Steerings));
    Steerings.remove(posMin(Steerings));
  
    steering = (Steerings[0]+Steerings[1])/2;
  
  //calculate the percentage of angle that we want
    
    throttlePercent = (throttle-throttleMin)*100/(throttleMax-throttleMin);
    throttlePercent = max(min(throttlePercent,100),0);
    steeringPercent = (steering-steeringMin)*100/(steeringMax-steeringMin);
    steeringPercent = max(min(steeringPercent,100),0);
  
    Serial.print("throttle% : ");
    Serial.println(throttlePercent);
    
  
    Serial.print("steering% : ");
    Serial.println(steeringPercent);

  //print in the logs

    RCLog.print(throttlePercent);
    RCLog.print("  ");
    RCLog.println(steeringPercent);
  
  //send the instruction if its new value is significantly different from the last one (3% and 2%)
  
    if (abs(throttlePercent-throttlePercentPrec) > 3)// && abs(throttlePercent-throttlePercentPrec)<5)
    {
      servo.SetRotation(1,throttlePercent);
      throttlePercentPrec = throttlePercent;
    }
  
    if (abs(steeringPercent-steeringPercentPrec) > 2)// && abs(steeringPercentPrecPrec-steeringPercent) < 2)
    {
      servo.SetRotation(0,steeringPercent); 
      steeringPercentPrec = steeringPercent;
    }
  
    Serial.println("we got the RC !");
    
    Xbee.print("RC throttle : ");
    Xbee.println(throttlePercent);
    Xbee.print("RC steering : ");
    Xbee.println(steeringPercent);
  }
  else
  {
    RCLog.println("RC turned off");
    Xbee.println("RC OFF");
  }
  
  RCLog.close();
  
  return true;
}

bool getWindHeading(void *)
{
  if (!RCOn)
  {
    WHLog = SD.open("WHLog.txt", FILE_WRITE);
    windD.Update();
    heading = windD.getHeading();
  
    if (abs(heading-headingPrec)>3)
    {
      printHeading = true;
      headingPrec = heading;
      Serial.println("we got the wind heading !");  
    }
    
    WHLog.println(heading);
    WHLog.close();

    Xbee.print("heading : ");
    Xbee.println(heading);
    
  }
  return true;
}

bool getWindSpeed(void *)
{
  if (!RCOn)
  {
    WSLog = SD.open("WSLog.txt", FILE_WRITE);
    windS.Update();
    windSpeed = windS.getSpeed();
    
    Serial.println("we got the wind speed !");
    
    WSLog.println(windSpeed);
    WSLog.close();

    Xbee.print("windSpeed : ");
    Xbee.println(windSpeed);
    
  }
  return true;
}

bool getCompassData(void *)
{
  if (!RCOn)
  {
    CMPSLog = SD.open("CMPSLog.txt", FILE_WRITE);
    yaw16 = compass.getAngle16();
    yaw8 = compass.getAngle8();
    pitch = compass.getPitch();
    roll = compass.getRoll();
    
    Serial.println("we got the compass !");
    
    CMPSLog.print(yaw16);
    CMPSLog.print("  ");
    CMPSLog.print(yaw8);
    CMPSLog.print("  ");
    CMPSLog.print(pitch);
    CMPSLog.print("  ");
    CMPSLog.println(roll);

    CMPSLog.close();

    Xbee.print("compass values : ");
    Xbee.print(yaw16);
    Xbee.print("  ");
    Xbee.print(pitch);
    Xbee.print("  ");
    Xbee.println(roll);
  }
  return true;
}

bool getGPSData(void *)
{
  if (!RCOn)
  {
    GPSLog = SD.open("GPSLog.txt", FILE_WRITE);
    Gps.Update();
    Lat = Gps.getLat();
    Long = Gps.getLong();
    
    Serial.println("we got the GPS !");

    GPSLog.print(Lat);
    GPSLog.print("  ");
    GPSLog.println(Long);
    
    GPSLog.close();

    Xbee.print("lat/long : ");
    Xbee.print(Lat);
    Xbee.print("  ");
    Xbee.println(Long);
  }
  return true;
}

bool getControl(void *)
{
  Coords = GPSToCart(Lat,Long);
  controler.set_x(Coords.x, Coords.y, yaw16, 0);
  double realHeading = RealHeading(yaw16,heading);
  controler.Reg(a,b,realHeading);
  double deltar = controler.get_deltar();
  double deltasmax = controler.get_deltasmax();
  // a faire : transformer la valeur des delta en un pourcentage servomoteurs
  Xbee.print("delatsmax : ");
  Xbee.println(deltasmax);
  Xbee.print("deltar : ");
  Xbee.println(deltar);

  return true;
}













//-------------------------------------------------------------------------------



















void setup() {
  
  Serial.begin(9600);
  Xbee.begin(9600);
  
  RC.init();
  servo.init();
  windD.init();
  windS.init();
  compass.init();
  Gps.init();

  Serial.print("Initializing SD card...");

  if (!SD.begin()) {
    Serial.println("initialization failed!");
  }
  else {Serial.println("initialization done.");}

  if(RewriteLogs)
  {
    Serial.println("logs are about to be deleted and recreated");
    SD.remove("GPSLog.txt");
    SD.remove("WHLog.txt");
    SD.remove("WSLog.txt");
    SD.remove("CMPSLog.txt");
    SD.remove("RCLog.txt");
  
    LogsInit();
  }

  //initiate the cartesian coordonates
  coords0 = GPSToCart0(Gps.getLat(), Gps.getLong());

  
  attachInterrupt(digitalPinToInterrupt(windS.getWindSpeedPin()), isr_rotation_arduino, FALLING);
  //Serial.print("ThrottlePercent  --  SteeringPercent  --  Heading  --  WindSpeed  --  Roll  --  Pitch  --  Yaw8  --  Yaw16  --  Latitude  --  Longitude");

  timer.every(80, getRCToControlServos);
  delay(1000);
  timer.every(300, getWindHeading);
  timer.every(300, getWindSpeed);
  delay(1000);
  timer.every(100, getCompassData);
  delay(1000);
  timer.every(1000, getGPSData);
  delay(1000);
  timer.every(100, getControl);
  delay(1000);
  //timer.every(100, printAll);
  //delay(1000);
  
}

void loop() {
  // put your main code here, to run repeatedly:

  printHeading = false;

  RCOn = RC.isEnabled();

  timer.tick();
  
}


//a faire : refaire la liaison xbee en fonction des marqueurs de debut de ligne precises dans get_xbee.py

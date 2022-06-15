#include "RCControler.h"
#include "ServoControler.h"
#include "WindDirection.h"
#include "WindSpeed.h"
#include "Compass_CMPS12.h"
#include "GPS.h"

#include <Vector.h>
#include <arduino-timer.h>

//ligne qui ne sert a rien


RCControler RC;
ServoControler servo;
WindDirection windD;
WindSpeed windS;
Compass compass;
GPS Gps;

//--------------------

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

int heading;
double windSpeed;

unsigned int yaw8;
double yaw16, pitch, roll;

double Lat, Long, Speed;

//---------------------

double throttlePercentPrecPrec = (RC.get_throttle()-throttleMin)*100/(throttleMax-throttleMin);
double steeringPercentPrecPrec = (RC.get_steering()-steeringMin)*100/(steeringMax-steeringMin);
double throttlePercentPrec = (RC.get_throttle()-throttleMin)*100/(throttleMax-throttleMin);
double steeringPercentPrec = (RC.get_steering()-steeringMin)*100/(steeringMax-steeringMin);

int headingPrec = windD.getHeading();

//--------------------

bool printHeading;

bool RCOn;

int cmpt;


//--------------------

Timer<5, millis, void *> timer;


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
  }
  return true;
}

bool getWindHeading(void *)
{
  if (!RCOn)
  {
    windD.Update();
    heading = windD.getHeading();
  
    if (abs(heading-headingPrec)>3)
    {
      printHeading = true;
      headingPrec = heading;
      Serial.println("we got the wind heading !");  
    }
  }
  return true;
}

bool getWindSpeed(void *)
{
  if (!RCOn)
  {
    windS.Update();
    windSpeed = windS.getSpeed();
    Serial.println("we got the wind speed !");
  }
  return true;
}

bool getCompassData(void *)
{
  if (!RCOn)
  {
    yaw16 = compass.getAngle16();
    yaw8 = compass.getAngle8();
    pitch = compass.getPitch();
    roll = compass.getRoll();
    Serial.println("we got the compass !");
  }
  
  return true;
}

bool getGPSData(void *)
{
  if (!RCOn)
  {
    Gps.Update();
    Lat = Gps.getLat();
    Long = Gps.getLong();
    Serial.println("we got the GPS !");
  }
  return true;
}

void setup() {

  Serial.begin(115200);
  RC.init();
  servo.init();
  windD.init();
  windS.init();
  compass.init();
  Gps.init();
  attachInterrupt(digitalPinToInterrupt(windS.getWindSpeedPin()), isr_rotation_arduino, FALLING);
  Serial.print("ThrottlePercent  --  SteeringPercent  --  Heading  --  WindSpeed  --  Roll  --  Pitch  --  Yaw8  --  Yaw16  --  Latitude  --  Longitude");

  timer.every(80, getRCToControlServos);
  delay(1000);
  timer.every(300, getWindHeading);
  timer.every(300, getWindSpeed);
  delay(1000);
  timer.every(100, getCompassData);
  delay(1000);
  timer.every(1000, getGPSData);
  delay(1000);
  timer.every(100, printAll);
  delay(1000);
  
}

void loop() {
  // put your main code here, to run repeatedly:

  printHeading = false;

  RCOn = RC.isEnabled();

  //getRCToControlServos();

  //getWindHeadingAndSpeed();
  
  //getCompassData();

  //getGPSData();

  //printAll();

  timer.tick();
  
  //delay(20);
  
}

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

int heading;
double windSpeed;

unsigned int yaw16, yaw8;
double pitch, roll;

double Lat, Long, Speed;

//---------------------

double throttlePercentPrec = (RC.get_throttle()-throttleMin)*100/(throttleMax-throttleMin);
double steeringPercentPrec = (RC.get_steering()-steeringMin)*100/(steeringMax-steeringMin);
int headingPrec = windD.getHeading();

//--------------------

bool printHeading;

//--------------------

Timer<5, millis, void*> timer;


void isr_rotation_arduino()
{
  windS.isr_rotation();
};


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

}

void printAll()
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
  Serial.print(yaw16);

  //----------------------

  Serial.print("  --  ");

  //printing GPS values

  
  Serial.print(Lat,6);
  Serial.print("  --  ");
  Serial.println(Long,6);

  
}

void getRCToControlServos()
{
  throttle = RC.get_throttle();
  steering = RC.get_steering();

  throttlePercent = (throttle-throttleMin)*100/(throttleMax-throttleMin);
  steeringPercent = (steering-steeringMin)*100/(steeringMax-steeringMin);
  

  if (abs(throttlePercent-throttlePercentPrec)>3)
  {
  servo.SetRotation(1,throttlePercent);
  throttlePercentPrec = throttlePercent;
  }

  if (abs(steeringPercent-steeringPercentPrec)>2)
  {
  servo.SetRotation(0,steeringPercent);
  steeringPercentPrec = steeringPercent;
  }
}

void getWindHeadingAndSpeed()
{
  windD.Update();
  heading = windD.getHeading();

  if (abs(heading-headingPrec)>3)
  {
    printHeading = true;
    headingPrec = heading;  
  }

  windS.Update();
  windSpeed = windS.getSpeed();
}

void getCompassData()
{
  roll = compass.getRoll();
  pitch = compass.getPitch();
  yaw8 = compass.getAngle8();
  yaw16 = compass.getAngle16();
}

void getGPSData()
{
  Gps.Update();
  Lat = Gps.getLat();
  Long = Gps.getLong();
}

void loop() {
  // put your main code here, to run repeatedly:

  printHeading = false;

  getRCToControlServos();

  getWindHeadingAndSpeed();
  
  getCompassData();

  getGPSData();

  printAll();
  
  
  delay(20);
  
}

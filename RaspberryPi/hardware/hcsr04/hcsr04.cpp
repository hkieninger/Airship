#include <stdio.h>
#include <stdlib.h>
#include <pigpiod_if2.h>
#include <iostream>
#include "hcsr04.h"

Hcsr04::Hcsr04(int trig, int echo){
      gpioSetMode(trig, PI_OUTPUT);
      gpioSetMode(echo, PI_INPUT);
      this->trig = trig;
      //trig pin must start LOW
      gpioWrite(trig, 0);
      usleep(3000);
}

Hcsr04::~Hcsr04() {
  gpioWrite(trig, LOW);
}

double Hcsr04::getDistance(){
  //Send trig pulse
  gpioWrite(trig, 1);
  usleep(20);
  gpioWrite(trig, 0);
  //Wait for echo start
  while(gpioRead(echo) == 0);
  //Wait for echo end
  long startTime = micros();
  while(gpioRead(echo) == 1);
  long travelTime = micros() - startTime;
  //Get distance in m
  double distance = travelTime / 5800;
  //Make sure that the next Trigger is at least 50us away
  usleep(70);

  return distance;


}

double Hcsr04::getSpeed(){
  dist1 = getDistance();
  startTimeSpeed = micros()
  delay(40);
  dist2 = getDistance();
  endTimeSpeed = micros()
  return (dist1 - dist2) / (endTimeSpeed-startTimeSpeed);

}

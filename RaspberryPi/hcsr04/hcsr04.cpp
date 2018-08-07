#include <stdio.h>
#include <stdlib.h>
#include <wiringPi.h>
#include <iostream>


#include "hcsr04.h"

Hcsr04::Hcsr04(int trig, int echo){
      wiringPiSetup();
      pinMode(trig, OUTPUT);
      pinMode(echo, INPUT);

      //trig pin must start LOW
      digitalWrite(trig, LOW);
      delay(30);
}

Hcsr04::~Hcsr04() {
  digitalWrite(this.trig, LOW);
}

double Hcsr04::getDistance(){
  //Send trig pulse
  digitalWrite(trig, HIGH);
  delayMicroseconds(20);
  digitalWrite(trig, LOW);
  //Wait for echo start
  while(digitalRead(echo) == LOW);
  //Wait for echo end
  long startTime = micros();
  while(digitalRead(echo) == HIGH);
  long travelTime = micros() - startTime;
  //Get distance in m
  double distance = travelTime / 5800;
  //Make sure that the next Trigger is at least 50us away
  while(micros() - startTime< 70);

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

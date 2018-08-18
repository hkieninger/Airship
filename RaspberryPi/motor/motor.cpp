#include <stdio.h>
#include <stdlib.h>
#include <pigpio.h>
#include <unistd.h>

#include "motor.h"

Motor::Motor(int signalpin, int relaypin){
  //sets relaypin to output
  this->relaypin = relaypin;
  this->signalpin = signalpin;
  gpioSetMode(relaypin, PI_OUTPUT);
  //sets signalpin to pwm
  gpioServo(signalpin, 0);
}

Motor::~Motor() {
  gpioServo(signalpin, 0);

}


void Motor::setSpeed(int speed){
  if (speed < 0){
    gpioWrite(relaypin, 1);
  }else{
    gpioWrite(relaypin, 0);
  }

  if(running){
    gpioServo(signalpin, 1000 + (speed * 10));
  }else{
    startop();
    gpioServo(signalpin, 1000 + (speed * 10));
  }

  if(abs(speed) < 10){
    running = 0;
  }else{
    running = 1;
  }
  //write the speed to the esc

}

void Motor::arm(){
  gpioServo(signalpin, 0);
  usleep(1000*500);
}

void startup(){
  gpioServo(signalpin, 1600);
  usleep(300*1000);
}

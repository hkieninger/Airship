#include <stdio.h>
#include <stdlib.h>
#include <pigpio.h>

#include "motor.h"

Motor::Motor(int signalpin, int relaypin){
  //sets relaypin to output
  this->relaypin = relaypin;
  this->signalpin = signalpin;
  gpioSetMode(relaypin, PI_OUTPUT);
  gpioSetMode(signalpin, PI_OUTPUT);
  //sets signalpin to pwm
  gpioServo(signalpin, 0);
}

Motor::~Motor() {

}


void Motor::setSpeed(int speed){
  if (speed < 0){
    gpioWrite(relaypin, 1);
  }else{
    gpioWrite(relaypin, 0);
  }
  if(abs(speed)>100){
    speed = 100;
  }
  //are these variables in bounds?
  gpioServo(signalpin, 1200+(speed * 10));

}

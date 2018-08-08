#include <stdio.h>
#include <stdlib.h>
#include <pigpio.h>

#include "motor.h"

Motor::Motor(int signalpin, int relaypin){
  //sets relaypin to output
  gpioSetMode(relaypin, PI_OUTPUT);
  gpioSetMode(signalpin, PI_OUTPUT);
  //sets signalpin to pwm
  gpioServo(signalpin, 0);
}

Motor::~Motor() {

}


void Motor::setStandby(int speed){
  if (speed < 0){
    gpioWrite(relaypin, 1);
  }else{
    gpioWrite(relaypin, 0);
  }
  if(abs(speed)>100){
    speed = 100;
  }
  //are these variables in bounds?
  gpioServo(0, 1000+(speed * 10));

}

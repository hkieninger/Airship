#include <stdio.h>
#include <stdlib.h>
#include <pigpio.h>

#include "motor.h"

Motor::Motor(int signalpin, int relaypin){
    if (gpioInitialise() < 0){
     // pigpio initialisation failed.
  }
  else{
     // pigpio initialised okay.
  }
  //sets relaypin to output
  gpioSetMode(relaypin, PI_OUTPUT);
  gpioSetMode(signalpin, PI_OUTPUT);
  //sets signalpin to pwm
  gpioPWM(signalpin, 0);
  //change frequenzy for the PWM signalpin
  gpioSetPWMfrequency(signalpin, PWM_FREQUENZY);
  //sets range for easy usage
  int duration = 1/PWM_FREQUENZY
  int range = duration*1000000
  gpioSetPWMrange(signalpin, range)
  //sets signalpin to 1ms
  gpioPWM(signalpin, ((0.002/duration) * range))
}

Motor::~Motor() {
  gpioTerminate();

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
  gpioPWM(signalpin, (abs(speed)/100+1) * (0.002/duration) * range))

}

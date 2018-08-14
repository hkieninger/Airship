#include <pigpio.h>

#include "Servo.h"

Servo::Servo(int signalpin){
  //sets relaypin to output
  this->signalpin = signalpin;
  //sets signalpin to pwm
  gpioServo(signalpin, 0);
}

Servo::~Servo() {

}


void Servo::setAngle(int angle){
  gpioServo(signalpin, 500 + (angle + 90) * 2000/180);

}

void Servo::poweroff(){
  gpioServo(signalpin, 0);
}

#include <pigpio.h>
#include <stdexcept>

#include "servo.h"

Servo::Servo(int signalpin) : signalpin(signalpin) {
  setAngle(0);
}

Servo::~Servo() {
  powerOff();
}

void Servo::setAngle(int angle){
  if(angle > 90 || angle < -90)
    throw std::invalid_argument("servo set angle: angle is out of bounds");
  signalpin.setPulsewidth(500 + (angle + 90) * 2000 / 180);
}

void Servo::powerOff(){
  signalpin.setPulsewidth(0);
}

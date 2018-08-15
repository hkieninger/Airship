#include "Flighcontroller.h"
#include "../servo/servo.h"
#include "../motor/motor.h"

Flightcontroller::Flightcontroller(Motor& motorleft, Motor& motorright, Servo& servotop, Servo& servoleft, Servo& servoright){
    this->motorleft = motorleft;
    this->motorright = motorright;
    this->servotop = servotop;
    this->servoleft = servoleft;
    this->servoright = servoright;

}

Flightcontroller::~Flightcontroller(){

}

Flightcontroller::forward(int speed){
  motorleftspeed = speed;
  motorrightspeed = speed;
  motorleft.setSpeed(speed);
  motorright.setSpeed(speed);
}

Flightcontroller::left(int level){

}

Flightcontroller::right(int level){

}

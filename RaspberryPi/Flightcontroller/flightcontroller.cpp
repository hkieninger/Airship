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
  servotop.setAngle(0);
  servoleft.setAngle(0);
  servoright.setAngle(0);
}

Flightcontroller::turn(int speedLeftmotor, int speedRightmotor, int dir){
  //upmost pedal
  int absat180 = abs(dir - 180);
  if(absat180 <= 90){
    servotop.setAngle(absat180);
  }else{
    servotop.setAngle(180-absat180);
  }
  //right pedal rotates to whole math 120 degrees
  dir = dir - 120
  if(dir<0){
    dir = dir + 360
  }
  absat180 = abs(dir - 180);
  if(absat180 <= 90){
    servoright.setAngle(absat180);
  }else{
    servoright.setAngle(180-absat180);
  }

  //left pedal rotates for another 120 degrees
  dir = dir - 120
  if(dir<0){
    dir = dir + 360
  }
  absat180 = abs(dir - 180);
  if(absat180 <= 90){
    servoleft.setAngle(absat180);
  }else{
    servoleft.setAngle(180-absat180);
  }



}

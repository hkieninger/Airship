#include <stdio.h>
#include <stdlib.h>
#include <pigpio.h>
#include <unistd.h>
#include <stdexcept>

#include "motor.h"

//TODO: reduce TIMEs to minimum and find optimal values for PWs (pulse width)
#define ZERO_PW 1000
#define MAX_PW (1900 - ZERO_PW)
#define STARTUP_PW 1400
#define STARTUP_THRESHOLD_PW 1100
#define ZERO_TIME (1000 * 1000)
#define STARTUP_TIME (300 * 1000)
#define RELAIS_TIME (6 * 1000) //from datasheet
#define ARM_TIME (2 * 1000 * 1000)

Motor::Motor(int pwmPin, int relaisPin){
  this->relaisPin = relaisPin;
  this->pwmPin = pwmPin;
  gpioSetMode(relaisPin, PI_OUTPUT);
  gpioWrite(relaisPin, 0);
  usleep(RELAIS_TIME);
  powerOn();
  lastThrust = 0;
}

Motor::~Motor() {
  powerOff();
}

int Motor::thrust2pw(int thrust) {
  return ZERO_PW + abs(thrust) * MAX_PW / MAX_THRUST;
}

void Motor::setZero() {
  gpioServo(pwmPin, ZERO_PW); //turn motor off
  usleep(ZERO_TIME); //wait until it halts
  lastThrust = 0;
}

void Motor::setESC(int pw) {
  //to start the motor you need more power
  /*if(pw < STARTUP_PW && thrust2pw(lastThrust) < STARTUP_THRESHOLD_PW) {
    printf("power start\n");
    gpioServo(pwmPin, STARTUP_PW);
    usleep(STARTUP_TIME);
  }*/
  gpioServo(pwmPin, pw);
}

void Motor::setThrust(int thrust){
  if(thrust > MAX_THRUST || thrust < -MAX_THRUST)
    throw std::invalid_argument("set motor thrust: thrust is out of bounds");

  if(thrust > 0) {
    if(lastThrust < 0) {
      setZero();
      gpioWrite(relaisPin, 0); //turn off the relais
      usleep(RELAIS_TIME); //wait until the relais switches position
    }
    setESC(thrust2pw(thrust));
  } else if(thrust < 0) {
    if(lastThrust >= 0) {
      if(lastThrust > 0)
        setZero();
      gpioWrite(relaisPin, 1); //turn the relais on
      usleep(RELAIS_TIME); //wait until the relais switches position
    }
    setESC(thrust2pw(thrust));
  } else { //thrust == 0
    setZero();
    gpioWrite(relaisPin, 0); //turn off the relais
  }
  lastThrust = thrust;
}

void Motor::powerOn(){
  gpioServo(pwmPin, ZERO_PW);
  usleep(ARM_TIME);
}

void Motor::powerOff() {
  setThrust(0);
  gpioServo(pwmPin, 0);
}

int Motor::getThrust() {
  return lastThrust;
}

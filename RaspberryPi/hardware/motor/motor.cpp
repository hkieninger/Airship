#include <stdlib.h>
#include <unistd.h>
#include <stdexcept>

#include "motor.h"

//TODO: reduce TIMEs to minimum and find optimal values for PWs (pulse width)
#define ZERO_PW 1100
#define MAX_PW (1900 - ZERO_PW)
#define ZERO_TIME (500 * 1000)
#define RELAIS_TIME (6 * 1000) //from datasheet
#define ARM_TIME (2 * 1000 * 1000) //from datasheet

Motor::Motor(int pwmPin, int relaisPin) : pwmPin(pwmPin), relaisPin(relaisPin) {
  this->pwmPin.setPinMode(PIN_OUTPUT);
  this->relaisPin.setPinMode(PIN_OUTPUT);
  this->relaisPin.writePin(LOW);
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
  pwmPin.setPulsewidth(ZERO_PW); //turn motor off
  usleep(ZERO_TIME); //wait until it halts
  lastThrust = 0;
}

void Motor::setESC(int pw) {
  pwmPin.setPulsewidth(pw);
}

void Motor::setThrust(int thrust) {
  if(thrust > MAX_THRUST || thrust < -MAX_THRUST)
    throw std::invalid_argument("set motor thrust: thrust is out of bounds");

  if(thrust > 0) {
    if(lastThrust < 0) {
      setZero();
      relaisPin.writePin(LOW); //turn off the relais
      usleep(RELAIS_TIME); //wait until the relais switches position
    }
    setESC(thrust2pw(thrust));
  } else if(thrust < 0) {
    if(lastThrust >= 0) {
      if(lastThrust > 0)
        setZero();
      relaisPin.writePin(HIGH); //turn the relais on
      usleep(RELAIS_TIME); //wait until the relais switches position
    }
    setESC(thrust2pw(thrust));
  } else if(lastThrust != 0) { //thrust == 0
    setZero();
    relaisPin.writePin(LOW); //turn off the relais
    usleep(RELAIS_TIME); //wait until the relais switches position
  }
  lastThrust = thrust;
}

void Motor::powerOn(){
  pwmPin.setPulsewidth(ZERO_PW);
  usleep(ARM_TIME);
}

void Motor::powerOff() {
  setThrust(0);
  pwmPin.setPulsewidth(0);
}

int Motor::getThrust() {
  return lastThrust;
}

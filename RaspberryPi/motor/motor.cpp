#include <stdio.h>
#include <stdlib.h>
#include <pigpio.h>
#include <unistd.h>
#include <stdexcept>

#include "motor.h"

//TODO: reduce TIMEs to minimum and find optimal values for PWs (pulse width)
#define ZERO_PW 1000
#define MAX_PW (1900 - ZERO_PW)
#define STARTUP_PW 1600
#define STARTUP_THRESHOLD_PW 1100
#define ZERO_TIME (1000 * 1000)
#define STARTUP_TIME (300 * 1000)
#define RELAIS_TIME (6 * 1000) //from datasheet
#define ARM_TIME (500 * 1000)

Motor::Motor(int signalpin, int relaypin){
  this->relaypin = relaypin;
  this->signalpin = signalpin;
  gpioSetMode(relaypin, PI_OUTPUT);
  gpioWrite(relaispin, 0);
  usleep(RELAIS_TIME);
  powerOn();
  lastThrust = 0;
}

Motor::~Motor() {
  powerOff();
}

static inline void thrust2pw(int thrust) {
  return ZERO_PW + abs(thrust) * MAX_PW / MAX_THRUST;
}

static inline void setZero() {
  gpioServo(signalpin, ZERO_PW); //turn motor off
  usleep(ZERO_TIME); //wait until it halts
  lastThrust = 0;
}

static inline void setESC(int pw) {
  //to start the motor you need more power
  if(pw < STATUP_PW && thrust2pw(lastThrust) < STARTUP_THRESHOLD_PW) {
    gpioServo(signalpin, STARTUP_PW);
    usleep(STARTUP_TIME);
  }
  gpioServo(signalpin, pw);
}

void Motor::setThrust(int thrust){
  if(thrust > MAX_THRUST || thrust < -MAX_THRUST)
    throw std::invalid_argument("set motor thrust: thrust is out of bounds");

  if(thrust > 0) {
    if(lastThrust < 0) {
      setZero();
      gpioWrite(relaispin, 0); //turn off the relais
      usleep(RELAIS_TIME); //wait until the relais switches position
    }
    setESC(thrust2pw(thrust));
  } else if(thrust < 0) {
    if(lastThrust >= 0) {
      if(lastThrust > 0)
        setZero();
      gpioWrite(relaispin, 1); //turn the relais on
      usleep(RELAIS_TIME); //wait until the relais switches position
    }
    setESC(thrust2pw(thrust));
  } else { //thrust == 0
    setZero();
    gpioWrite(relaispin, 0); //turn off the relais
  }
  lastThrust = thrust;
}

void Motor::powerOn(){
  gpioServo(signalpin, 0);
  usleep(ARM_TIME);
}

void Motor::powerOff() {
  setThrust(0);
  gpioServo(signalpin, 0);
}

void Motor::getThrust() {
  return lastThrust;
}

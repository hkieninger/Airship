#ifndef STEERING_H
#define STEERING_H 1

#include <stdint.h>

#include "../../hardware/motor/motor.h"
#include "../../hardware/servo/servo.h"

#define MAX_YAW 100
#define MAX_PITCH 100
#define MAX_VELOCITY MAX_THRUST

class Steering {
    Motor &leftMotor, &rightMotor;
    Servo &leftRudder, &rightRudder, &topRudder;
    int8_t yaw, pitch, velocity, callibrationYaw, callibrationPitch;

    void updateActuators();
public:
    Steering(Motor &leftMotor, Motor &rightMotor, Servo &leftRudder, Servo &rightRudder, Servo &topRudder);

    void setDirection(int8_t yaw, int8_t pitch);
    void setVelocity(int8_t vel);
    void setCallibration(int8_t calYaw, int8_t calPitch);

};

#endif /* STEERING_H */
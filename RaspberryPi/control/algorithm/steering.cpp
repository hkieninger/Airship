#include <stdlib.h>
#include <stdexcept>
#include <math.h>
#include <stdio.h>

#include "steering.h"

#define PI ((float) M_PI)

float direction2angle(int8_t yaw, int8_t pitch) {
    if(pitch == 0) {
        if(yaw > 0) {
            return PI / 2;
        } else {
            return -PI / 2;
        }
    } else {
        float angle = atanf(-1.0 * yaw / pitch);
        if(pitch > 0)
            return angle + PI;
        return angle;
    }
}

float direction2radius(int8_t yaw, int8_t pitch) {
    return sqrtf(yaw * yaw + pitch * pitch);
}

int radian2degree(float radian) {
    return roundf(radian * 180 / PI);
}

int sign(int i) {
    if(i > 0)
        return 1;
    else if(i < 0)
        return -1;
    return 0;
}

Steering::Steering(Motor &leftMotor, Motor &rightMotor, Servo &leftRudder, Servo &rightRudder, Servo &topRudder) :
    leftMotor(leftMotor), rightMotor(rightMotor), leftRudder(leftRudder), rightRudder(rightRudder), topRudder(topRudder),
    yaw(0), pitch(0), velocity(0), callibrationYaw(0), callibrationPitch(0) {
        setCallibration(0, 0);
        setDirection(0, 0);
        setVelocity(0);
}

void Steering::updateActuators() {
    /*
     * could be customized for a flying vehicle with n rudders with custom angles between the rudders
     */

    //apply callibration
    int yaw = this->yaw + callibrationYaw;
    int pitch = this->pitch + callibrationPitch;
    yaw = -yaw;

    //calculate how to control the rudders
    float angle = direction2angle(yaw, pitch);
    float radius = direction2radius(yaw, pitch);
    //set radius to bound
    if(radius > MAX_RADIUS)
        radius = MAX_RADIUS;

    float topForce = sinf(angle);
    float rightForce = sinf(angle + 2 * PI / 3);
    float leftForce = sinf(angle + 4 * PI / 3);
    float maxForce = fmaxf(fabsf(topForce), fmaxf(fabsf(rightForce), fabsf(leftForce)));

    float factor = maxForce * MAX_RADIUS / radius;
    topForce /= factor;
    rightForce /= factor;
    leftForce /= factor;

    int topAngle = radian2degree(asinf(topForce) / 2);
    int rightAngle = radian2degree(asinf(rightForce) / 2);
    int leftAngle = radian2degree(asinf(leftForce) / 2);

    topRudder.setAngle(topAngle);
    rightRudder.setAngle(rightAngle);
    leftRudder.setAngle(leftAngle);

    //calculate how to control the motors
    /*
     * velocity has priority over yaw
     */
    int leftThrust;
    int rightThrust;

    int vel = abs(velocity);
    int sgn = sign(velocity);
    if(yaw > 0) { //turn right
        leftThrust = roundf(2 * vel / (2 - (float) yaw / MAX_YAW));
        if(leftThrust > MAX_VELOCITY)
            leftThrust = MAX_VELOCITY;
        rightThrust = 2 * vel - leftThrust;
    } else { //turn left
        rightThrust = roundf(2 * vel / (2 + (float) yaw / MAX_YAW));
        if(rightThrust > MAX_VELOCITY)
            rightThrust = MAX_VELOCITY;
        leftThrust = 2 * vel - rightThrust;
    }
    leftThrust *= sgn;
    rightThrust *= sgn;

    leftMotor.setThrust(leftThrust);
    rightMotor.setThrust(rightThrust);
}

void Steering::setDirection(int8_t yaw, int8_t pitch) {
    if(yaw > MAX_YAW || yaw < -MAX_YAW || pitch > MAX_PITCH || pitch < -MAX_PITCH)
        throw std::invalid_argument("steering set direction: direction is out of range.");
    this->yaw = yaw;
    this->pitch = pitch;
    updateActuators();
}

void Steering::setVelocity(int8_t vel) {
    if(vel > MAX_VELOCITY || vel < -MAX_VELOCITY)
        throw std::invalid_argument("steering set velocity: velocity is out of range.");
    velocity = vel;
    updateActuators();
}

void Steering::setCallibration(int8_t calYaw, int8_t calPitch) {
    if(calYaw > MAX_YAW || calYaw < -MAX_YAW || calPitch > MAX_PITCH || calPitch < -MAX_PITCH)
        throw std::invalid_argument("steering set callibrate: callibration is out of range.");
    callibrationPitch = calPitch;
    callibrationYaw = calYaw;
}
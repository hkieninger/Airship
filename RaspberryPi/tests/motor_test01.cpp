#include <stdio.h>
#include <unistd.h>
#include <iostream>
#include <pigpio.h>
#include "../motor/motor.h"

int main(){
    gpioInitialise();
    Motor motor(20,18);
    motor.setSpeed(0);
    usleep(1000*1000);
    motor.arm();
    motor.setSpeed(100);
    usleep(1000*1000);
    motor.setSpeed(20);
    usleep(1000*1000*100);
}

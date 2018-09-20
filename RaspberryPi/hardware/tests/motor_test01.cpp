#include <stdio.h>
#include <unistd.h>
#include <iostream>
#include <pigpio.h>
#include "../motor/motor.h"

//left and right not true yet
#define RELAIS_LEFT 27
#define RELAIS_RIGHT 17
#define ESC_RIGHT 22
#define ESC_LEFT 18

#define THRUST 50
#define DURATION 10

int main(){
    gpioInitialise();
    printf("create motor object\n");
    Motor lm(ESC_LEFT, RELAIS_LEFT);
    Motor rm(ESC_RIGHT, RELAIS_RIGHT);

    printf("set thrust forwards for x s\n");
    lm.setThrust(2 * THRUST);
    rm.setThrust(2 * THRUST);
    //usleep(2 * 1000 * 1000);
    lm.setThrust(THRUST);
    rm.setThrust(THRUST);
    sleep(DURATION);

    printf("set thrust 0 for x s\n");
    lm.setThrust(0);
    rm.setThrust(0);
    sleep(3);

    printf("set thrust backwards for x s\n");
    lm.setThrust(-THRUST);
    rm.setThrust(-THRUST);
    sleep(DURATION);

    printf("finished\n");

    return 0;
}

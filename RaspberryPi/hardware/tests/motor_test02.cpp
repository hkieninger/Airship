#include <stdio.h>
#include <unistd.h>
#include <pigpio.h>

//left and right not true yet
#define RELAIS_LEFT 17
#define RELAIS_RIGHT 27
#define ESC_RIGHT 22
#define ESC_LEFT 18

#define THRUST 30
#define DURATION 10

int main(){
    gpioInitialise();
    gpioServo(ESC_RIGHT, 0);
    sleep(3);
    gpioServo(ESC_RIGHT, 1400);
    sleep(10);
    gpioServo(ESC_RIGHT, 0);

    return 0;
}
#include <stdio.h>
#include <unistd.h>
#include <iostream>
#include <pigpio.h>
#include "../motor/motor.h"

int main(){
    gpioInitialise();
    Motor motor(20,21);
    motor.setSpeed(0)
    for(int i = 0; i<101; i++){
        usleep(1000*100);
        motor.setSpeed(i);
        printf("Distance: %x\n", i);
    }


}

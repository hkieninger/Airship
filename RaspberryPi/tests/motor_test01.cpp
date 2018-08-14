#include <stdio.h>
#include <unistd.h>
#include <iostream>
#include <pigpio.h>
#include "../motor/motor.h"

int main(){
    gpioInitialise();
    Motor motor(20,18);
    motor.setSpeed(0);
<<<<<<< HEAD
    usleep(1000*1000);
    motor.arm();
    motor.setSpeed(100);
    usleep(1000*1000);
    motor.setSpeed(20);
    usleep(1000*1000*100);
=======
    usleep(5* 1000*1000);
    while(1){
      for(int i = 0; i<50; i++){
          usleep(100*1000);
          motor.setSpeed(i);
          printf("Speed: %d\n", i);
      }
    }


>>>>>>> a56590e0a659cbd3489e3d93e2e176128356d24b
}

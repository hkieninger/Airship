#include <stdio.h>
#include <unistd.h>
#include <iostream>
#include <pigpio.h>
#include "../motor/motor.h"

int main(){
    printf("%d\n",gpioInitialise());
    Motor motor(18,21);
    motor.setSpeed(0);
    usleep(5* 1000*1000);
    while(1){
      for(int i = 0; i<50; i++){
          usleep(100*1000);
          motor.setSpeed(i);
          printf("Speed: %d\n", i);
      }
    }


}

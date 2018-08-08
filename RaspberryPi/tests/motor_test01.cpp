#include <stdio.h>
#include <unistd.h>
#include <iostream>
#include <pigpio.h>
#include "../motor/motor.h"

int main(){
    printf("%d\n",gpioInitialise());
    Motor motor(18,21);
    motor.setSpeed(0);
    usleep(1000*10000);
    while(1){
      for(int i = 0; i<101; i++){
          usleep(1000*100);
          motor.setSpeed(i);
          printf("Speed: %d\n", i);
      }
    }


}

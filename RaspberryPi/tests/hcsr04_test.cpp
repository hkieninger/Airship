#include <stdio.h>
#include <stdlib.h>
#include <wiringPi.h>
#include <iostream>
#include "../hcsr04/hcsr04.h"

int main(){
  Hcsr04 hcsr(28,29);

  //test function
  printf("Distance: %x\n", hcsr.getDistance());

}

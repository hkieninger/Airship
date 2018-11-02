#include <stdio.h>
#include "../hcsr04/hcsr04.h"

int main() {
  GpioDevice::initialiseGpio();
  Hcsr04 hcsr(20,21);
  while(true) {
    printf("distance: %d cm\n", hcsr.getDistance());
  }
  GpioDevice::terminateGpio();
}

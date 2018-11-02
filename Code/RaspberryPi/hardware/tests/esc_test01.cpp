#include <pigpiod_if2.h>
#include <stdlib.h>
#include <stdio.h>


int main() {
    gpioInitialise();
    while(1) {
        int i = 0;
        scanf("%d", &i);
        gpioServo(17, i);
    }
    return 0;
}
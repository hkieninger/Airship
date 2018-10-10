#include <pigpio.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define PIN 25
#define OPS (100 * 1000)

static time_t seconds() {
    return time(NULL);
}

static void setMode(unsigned int pin, unsigned int mode) {
    int ret = gpioSetMode(pin, mode);
    if(ret != 0)
        exit(1);
}

static void writePin(unsigned int pin, unsigned int level) {
    int ret = gpioWrite(pin, level);
    if(ret != 0)
        exit(1);
}

static int readPin(unsigned int pin) {
    int ret = gpioRead(pin);
    if(ret == PI_BAD_GPIO)
        exit(1);
    return ret;
}

int main() {
    if(gpioInitialise() < 0)
        return 1;
    //init pins
    setMode(PIN, PI_OUTPUT);
    //test speed
    printf("starting\n");
    time_t start = seconds();
    for(int i = 0; i < OPS; i++) {
        writePin(PIN, 1);
        writePin(PIN, 0);
    }
    time_t stop = seconds();
    printf("stopped (start: %d, stop: %d)\n", start, stop);
    time_t diff = stop - start;
    printf("time used %d\n", diff);
    if(diff > 0)
        printf("working with %d Hz\n", OPS * 2 / diff);
    gpioTerminate();
    return 0;
}
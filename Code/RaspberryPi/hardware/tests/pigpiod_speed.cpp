#include <stdio.h>
#include <stdlib.h>
#include <pigpiod_if2.h>
#include <unistd.h>
#include <stdint.h>
#include <stdexcept>
#include <errno.h>
#include <string>
//#include <sys/time.h>
#include <string.h>
#include <time.h>

#define PIN 25
#define OPS (100 * 1000)

int gpioHandle = -1;

static time_t seconds() {
    return time(NULL);
}

static void setMode(int pin, int mode) {
    int ret = set_mode(gpioHandle, pin, mode);
    if(ret != 0)
        exit(1);
}

static void writePin(int pin, int level) {
    int ret = gpio_write(gpioHandle, pin, level);
    if(ret != 0)
        exit(1);
}

static int readPin(int pin) {
    int ret = gpio_read(gpioHandle, pin);
    if(ret == PI_BAD_GPIO)
        exit(1);
    return ret;
}

int main() {
    //init pigpio
    gpioHandle = pigpio_start(NULL, NULL);
    if(gpioHandle < 0)
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
    printf("working with %d Hz\n", OPS * 2 / diff);
    //uninit pigpio
    pigpio_stop(gpioHandle);
    return 0;
}
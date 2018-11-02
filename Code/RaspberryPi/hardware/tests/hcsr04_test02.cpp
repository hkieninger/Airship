#include <stdio.h>
#include <stdlib.h>
#include <pigpio.h>
#include <unistd.h>
#include <stdint.h>
#include <stdexcept>
#include <errno.h>
#include <string>
#include <sys/time.h>
#include <string.h>

#define TRIG 20
#define ECHO 21

static uint64_t micros() {
    struct timeval tv;
    if(gettimeofday(&tv, NULL) < 0)
        throw std::runtime_error("get time of day: " + std::string(strerror(errno)));
    return tv.tv_sec + tv.tv_usec;
}

static void setMode(int pin, int mode) {
    int ret = gpioSetMode(pin, mode);
    if(ret != 0)
        exit(1);
}

static void writePin(int pin, int level) {
    int ret = gpioWrite(pin, level);
    if(ret != 0)
        exit(1);
}

static int readPin(int pin) {
    int ret = gpioRead(pin);
    if(ret == PI_BAD_GPIO)
        exit(1);
    return ret;
}

static void triggerPin(int pin) {
    if(gpioTrigger(pin, 10, 1) != 0)
        exit(1);
}

static int distance() {
    //trigger a measurement
    triggerPin(TRIG);
    //measure the duration of the answer pulse
    uint64_t start, stop;
    do {
        start = micros();
    } while(readPin(ECHO) == 0);
    do {
        stop = micros();
    } while(readPin(ECHO) == 1);
    int diff = stop - start;
    //calculate the distance from the duration
    int distance = diff * 343 / 1000 / 2;
    usleep(50 * 1000);
    return distance;
}

int main() {
    printf("millis %d\n", micros() / 1000);
    //init pigpio
    if(gpioInitialise() < 0)
        return 1;
    //init pins
    setMode(TRIG, PI_OUTPUT);
    writePin(TRIG, 0);
    setMode(ECHO, PI_INPUT);
    //loop
    while(true) {
        //sleep(1);
        printf("distance %d cm\n", distance());
    }
    //uninit pigpio
    gpioTerminate();
    return 0;
}
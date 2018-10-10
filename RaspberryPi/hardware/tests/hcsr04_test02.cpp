#include <stdio.h>
#include <stdlib.h>
#include <pigpiod_if2.h>
#include <unistd.h>
#include <stdint.h>
#include <stdexcept>
#include <errno.h>
#include <string>
#include <sys/time.h>
#include <string.h>

#define TRIG 20
#define ECHO 21

int gpioHandle = -1;

static uint64_t micros() {
    struct timeval tv;
    if(gettimeofday(&tv, NULL) < 0)
        throw std::runtime_error("get time of day: " + std::string(strerror(errno)));
    return tv.tv_sec + tv.tv_usec;
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

static int distance(int gpioHandle) {
    //trigger a measurement
    writePin(TRIG, 1);
    usleep(10);
    writePin(TRIG, 0);
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
    if(distance < 20 || distance > 4000)
        return -1;
    //ensure that the max repetition rate is 50 micro seconds
    int delay = 50 - 10 - diff;
    if(delay > 0)
        usleep(delay);
    
    return distance;
}

int main() {
    //init pigpio
    gpioHandle = pigpio_start(NULL, NULL);
    if(gpioHandle < 0)
        return 1;
    //init pins
    setMode(TRIG, PI_OUTPUT);
    writePin(TRIG, 0);
    setMode(ECHO, PI_INPUT);
    //loop
    while(true) {
        printf("distance %d cm\n", distance(gpioHandle));
    }
    //uninit pigpio
    pigpio_stop(gpioHandle);
    return 0;
}
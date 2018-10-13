#include <unistd.h>
#include <stdexcept>
#include <string>
#include <string.h>
#include <errno.h>
#include <sys/time.h>

#include "hcsr04.h"

#define TIMEOUT_MICROS (25 * 1000)

static uint64_t micros() {
    struct timeval tv;
    if(gettimeofday(&tv, NULL) < 0)
        throw std::runtime_error("get time of day: " + std::string(strerror(errno)));
    return 1000 * 1000 * tv.tv_sec + tv.tv_usec;
}

static void sort(int n, int16_t *x) {
    for(int i = 0; i < n; i++) {
        for(int j = i + 1; j < n; j++) {
            if(x[i] > x[j]) {
                int16_t temp = x[i];
                x[i] = x[j];
                x[j] = temp;
            }
        }
    }
}

Hcsr04::Hcsr04(int trigPin, int echoPin) : trig(trigPin), echo(echoPin) {
    trig.setPinMode(PIN_OUTPUT);
    trig.writePin(LOW);
    echo.setPinMode(PIN_INPUT);
}

int16_t Hcsr04::getDistance(){
    //trigger a measurement
    trig.triggerPin(10, HIGH);
    //measure the duration of the answer pulse
    uint64_t start, stop, timeout = micros();
    do {
        start = micros();
        if(start - timeout > TIMEOUT_MICROS) {
            usleep(15 * 1000);
            return -1;
        }
    } while(echo.readPin() == LOW);
    timeout = start;
    do {
        stop = micros();
        if(stop - timeout > TIMEOUT_MICROS) {
            usleep(15 * 1000);
            return -1;
        }
    } while(echo.readPin() == HIGH);
    int diff = stop - start;
    //calculate the distance from the duration
    int distance = diff * 343 / 1000 / 2;
    //ensure a certain delay between subsequent measurements
    usleep(15 * 1000);
    //return the distance
    if(distance < 20 || distance > 4000)
        return -1;
    return distance;
}

#include <stdio.h>

int16_t Hcsr04::getMedian() {
    int16_t array[SAMPLES_COUNT];
    for(int i = 0; i < SAMPLES_COUNT; i++) {
        array[i] = getDistance();
    }
    sort(SAMPLES_COUNT, array);
    int median = SAMPLES_COUNT / 2;
    if(SAMPLES_COUNT % 2 == 0) {
        return (array[median] + array[median + 1]) / 2;
    } else {
        return array[median];
    }
}
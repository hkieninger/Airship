#include <unistd.h>
#include <stdexcept>
#include <string>
#include <string.h>
#include <errno.h>
#include <sys/time.h>

#include "hcsr04.h"

static uint64_t micros() {
    struct timeval tv;
    if(gettimeofday(&tv, NULL) < 0)
        throw std::runtime_error("get time of day: " + std::string(strerror(errno)));
    return tv.tv_sec + tv.tv_usec;
}

Hcsr04::Hcsr04(int trigPin, int echoPin) : trig(trigPin), echo(echoPin) {
    trig.setPinMode(PIN_OUTPUT);
    trig.writePin(LOW);
    echo.setPinMode(PIN_INPUT);
}

int16_t Hcsr04::getDistance(){
    //trigger a measurement
    trig.writePin(HIGH);
    usleep(10);
    trig.writePin(LOW);
    //measure the duration of the answer pulse
    uint64_t start, stop;
    do {
        start = micros();
    } while(echo.readPin() == LOW);
    do {
        stop = micros();
    } while(echo.readPin() == HIGH);
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
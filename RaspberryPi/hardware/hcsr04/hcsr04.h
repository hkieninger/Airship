#ifndef HCSR04_H
#define HCSR04_H 1

#include <stdint.h>
#include "../gpio/pin_dev.h"

#define SAMPLES_COUNT 3

class Hcsr04{
    PinDevice trig;
    PinDevice echo;

public:
    Hcsr04(int trigPin, int echoPin);

    /*
     * measures the distance in mm
     * the measurement takes at least ~15ms and maximal ~40ms
     * @return: the distance in mm (20 - 4000) or -1 if the distance is too big or too small
     */
    int16_t getDistance();

    /*
     * measures the distance SAMPLE_COUNT times and returns the median
     * it is recommended to use this function if the programm is run with several threads
     * through sheduling the programm may miss a edge leading to a missmeasurement
     * the problem couldn't be solved with interrupts since they also have a latency of approx 50us
     * @return: the median of the distance measurements, see getDistance()
     */
    int16_t getMedian();
};

#endif /* HCSR04_H */

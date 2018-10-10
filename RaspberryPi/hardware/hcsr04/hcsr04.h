#ifndef HCSR04_H
#define HCSR04_H 1

#include <stdint.h>
#include "../gpio/pin_dev.h"

class Hcsr04{
    PinDevice trig;
    PinDevice echo;

public:
    Hcsr04(int trigPin, int echoPin);

    /*
     * measures the distance in mm
     * @return: the distance in mm (20 - 4000) or -1 if the distance is too big or too small
     */
    int16_t getDistance();
};

#endif /* HCSR04_H */

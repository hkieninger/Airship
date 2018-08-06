#ifndef HCSR04_H
#define HCSR04_H 1

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <wiringPi.h>
#include <iostream>



class Hcsr04{

public:
    /*
     * Adds a hcsr04 Ultrasonic distance sensor
     */
    Hcsr04(int trig, int echo);

    /*
     * destructor
     */
    ~Hcsr04();

    /*
     * get distance in Meters
     */
     double getDistance();

     double getSpeed();


};

#endif /* HCSR04_H */

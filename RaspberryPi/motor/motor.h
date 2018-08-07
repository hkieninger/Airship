ifndef MOTOR_H
#define MOTOR_H 1

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <wiringPi.h>
#include <iostream>


class Motor{

public:
    /*
     *
     */
    Motor(int signal, int relay);

    /*
     * destructor
     */
    ~Motor();



};

#endif /* MOTOR_H */

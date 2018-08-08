#ifndef MOTOR_H
#define MOTOR_H 1

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <wiringPi.h>
#include <iostream>


<<<<<<< HEAD
class Motor {

=======
class Motor{
  int signalpin;
  int relaypin;
>>>>>>> 9b0ae79f8ff8a5e02d1ece2639a4a6b2898771cf
public:

    /*
     *
     */
    Motor(int signalpin, int relaypin);

    /*
     * destructor
     */
    ~Motor();

    void setSpeed(int speed);



};

#endif /* MOTOR_H */

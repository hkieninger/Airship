#ifndef MOTOR_H
#define MOTOR_H 1

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <wiringPi.h>
#include <iostream>


class Motor{
  int signalpin;
  int relaypin;
  bool running = 0;
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

    void arm();

    void startup();



};

#endif /* MOTOR_H */

#ifndef MOTOR_H
#define MOTOR_H 1

#define MAX_THRUST 100

#include "../gpio/pin_dev.h"

class Motor {
    PinDevice pwmPin;
    PinDevice relaisPin;
    int lastThrust;

    /*
     * helper methods
     */

    //calculates the pulswidth from the thrust
    int thrust2pw(int thrust);

    //turns off the motor and wait that it stops
    void setZero();

    //sets the pulse witdth of the esc
    void setESC(int pw);
public:

    /*
     * setup the pins and arm the esc via powerOn()
     */
    Motor(int pwmPin, int relaisPin);

    /*
     * unarm the esc via powerOff()
     */
    ~Motor();

    /*
     * set the seed of the motor
     * form MAX_THRUST to -MAX_THRUST
     */
    void setThrust(int thrust);

    /*
     * arms the esc
     */
    void powerOn();

    /*
     * unarm the esc
     */
    void powerOff();

    /*
     * returns the last thrust
     */
    int getThrust();

};

#endif /* MOTOR_H */

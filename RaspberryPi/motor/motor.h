#ifndef MOTOR_H
#define MOTOR_H 1

#define MAX_THRUST 100

class Motor{
  int signalpin;
  int relaypin;
  int lastThrust;
public:

    /*
     * setup the pins and arm the esc via powerOn()
     */
    Motor(int signalpin, int relaypin);

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

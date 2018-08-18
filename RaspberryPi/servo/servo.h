#ifndef Servo_H
#define Servo_H 1



class Servo{
  int signalpin;
public:

    /*
     *
     */
    Servo(int signalpin);

    /*
     * destructor
     */
    ~Servo();
    //Accepts  -90 to 90 as input
    void setAngle(int angle);

    void poweroff();




};

#endif /* Servo_H */

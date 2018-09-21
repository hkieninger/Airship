#ifndef Servo_H
#define Servo_H 1

class Servo{
  int signalpin;
public:

    /*
     * sets the initial angle to 0°
     */
    Servo(int signalpin);

    /*
     * powers off the servo
     */
    ~Servo();

    /*
     * angle should have a value between -90 to 90°
     * its up to you not to pass values which are to high or low, which could damage the servo
     */
    void setAngle(int angle);

    /*
     * powers off the servo
     */
    void powerOff();
};

#endif /* Servo_H */

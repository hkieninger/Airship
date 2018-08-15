#ifndef FLIGHCONTROLLER_H
#define FLIGHTCONTROLLER_H 1



class Flightcontroller{
  Motor *motorright,*leftmotor;
  Servo *servotop,*servoleft,*servoright;

  int motorleftspeed = 0;
  int motorrightspeed = 0;
public:

    /*
     *
     */
    Flightcontroller();

    /*
     * destructor
     */
    ~Flightcontroller();

    //fly at an certain speed
    forward(int speed);

    left(int levle);

    right(int level);




};

#endif /* Flightcontroller_H */

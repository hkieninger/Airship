#ifndef CONTROL_THREAD_H
#define CONTROL_THREAD_H 1

#include <deque>

#include "connection.h"
#include "steering.h"

#include <../hardware/mpu6050/mpu6050.h>
#include <../hardware/servo/servo.h>
#include <../hardware/neo6m/neo6m_exception.h>
#include <../hardware/neo6m/neo6m.h>
#include <../hardware/bmp280/bmp280.h>
#include <../hardware/ads1115/ads1115.h>
#include <../hardware/qmc5883l/qmc5883l.h>
#include <../hardware/motor/motor.h>
#include <../hardware/hcsr04/hcsr04.h>

class ControlThread: public Thread {
    //the hardware
    Motor leftMotor(LEFT_MOTOR_ESC, LEFT_MOTOR_RELAIS), rightMotor(RIGHT_MOTOR_ESC, RIGHT_MOTOR_RELAIS);
    Servo leftRudder(LEFT_RUDDER_SERVO), rightRudder(RIGHT_RUDDER_SERVO), topRudder(TOP_RUDDER_SERVO);
    Steering steering(leftMotor, rightMotor, leftRudder, rightRudder, topRudder);
    /*Ads1115 ads;
    Bmp280 bmp;
    Mpu6050 mpu; //add interrupt pin and function
    Qmc5883l qmc; //add interrupt pin and function
    Hcsr04 hcFront(FRONT_HCSR04_TRIG, FRONT_HCSR04_ECHO);
    Hcsr04 hcBottom(BOTTOM_HCSR04_TRIG, BOTTOM_HCSR04_ECHO);
    Neo6MThread neo6mT;
    CameraThread camBottomT(CSI);
    CameraThread camtFrontT(USB); */

    //the connection to the controlling computer
    Connection connection;
    //deque of incoming pakets
    std::deque<Paket *> paketDeque;
    //mutex to synchronize the deque
    pthread_mutex_t dequeMutex;

    bool running;

    void configureRpi(Paket &paket);
    void configureActuator(Paket &paket);
    void configureSteering(Paket &paket);
    void configureAutopilot(Paket &paket);
    void configureSensor(Paket &paket);

    void handlePaket(Paket &paket);
public:
    ControlThread();
    ~ControlThread();
    void stopRunning();
    void dequePaket(Paket *paket);
    virtual void run();

    Connection &getConnection() {
        return connection;
    }
};

#endif /* CONTROL_THREAD_H */
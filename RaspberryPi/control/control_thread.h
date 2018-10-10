#ifndef CONTROL_THREAD_H
#define CONTROL_THREAD_H 1

#include <pthread.h>
#include <deque>

#include "connection.h"
#include "algorithm/steering.h"

/*
use "" instead of <>

#include <../hardware/neo6m/neo6m_exception.h>
#include <../hardware/neo6m/neo6m.h>> */
#include "../hardware/hcsr04/hcsr04.h"
#include "../hardware/ads1115/ads1115.h"
#include "../hardware/mpu6050/mpu6050.h"
#include "../hardware/bmp280/bmp280.h"
#include "../hardware/qmc5883l/qmc5883l.h"
#include "../hardware/servo/servo.h"
#include "../hardware/motor/motor.h"
#include "../thread/thread.h"

class ControlThread: public Thread {
    int gpioHandle;

    //the hardware
    //actuators
    Motor *leftMotor, *rightMotor;
    Servo *leftRudder, *rightRudder, *topRudder;
    Steering *steering;
    //sensors
    Ads1115 *ads;
    Mpu6050 *mpu;
    Bmp280 *bmp;
    Qmc5883l *qmc;
    Hcsr04 *hcFront;
    Hcsr04 *hcBottom;

    /*
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

    void measureQmc();
    void measureHcsr();
    void measureBmp();
    void measureAds();
    void measureMpu();
    void measureData();
public:
    ControlThread();
    ~ControlThread();
    void stopRunning();
    void pushPaket(Paket *paket);
    virtual void run();

    Connection &getConnection() {
        return connection;
    }
};

#endif /* CONTROL_THREAD_H */
#include <arpa/inet.h>
#include <stdint.h>
#include <stdio.h>

#include "pin.h"
#include "makros.h"
#include "control_thread.h"

inline void invalidParam(const char *deviceName, int paramNr) {
    fprintf(stderr, "invalid %s configuration paket with param: %d\n", deviceName, paramNr);
}

ControlThread::ControlThread() : connection(*this), running(true) {
    pthread_mutex_init(&dequeMutex, NULL);
}

ControlThread::~ControlThread() {
    pthread_mutex_destroy(&dequeMutex);
}

void ControlThread::stopRunning() {
    running = false;
}

void ControlThread::pushPaket(Paket *paket) {
    pthread_mutex_lock(&dequeMutex);
    paketDeque.push_back(paket);
    pthread_mutex_unlock(&dequeMutex);
}

void ControlThread::configureRpi(Paket &paket) {
    NOT_IMPLEMENTED;
}

void ControlThread::configureActuator(Paket &paket) {
    int8_t val = *((int8_t *) paket.data);
    switch(paket.param) {
        case Configuration::LEFT_MOTOR: leftMotor->setThrust(val); break;
        case Configuration::RIGHT_MOTOR: rightMotor->setThrust(val); break;
        case Configuration::LEFT_RUDDER: leftRudder->setAngle(val); break;
        case Configuration::RIGHT_RUDDER: rightRudder->setAngle(val); break;
        case Configuration::TOP_RUDDER: topRudder->setAngle(val); break;
        default: invalidParam("actuator", paket.param);
    }
}

void ControlThread::configureSteering(Paket &paket) {
    switch(paket.param) {
        case Configuration::VELOCITY: {
            int8_t *vel = (int8_t *) paket.data;
            steering->setVelocity(vel[0]);
        }
        break;
        case Configuration::DIRECTION: {
            int8_t *dir = (int8_t *) paket.data;
            steering->setDirection(dir[0], dir[1]);
        }
        break;
        case Configuration::CALLIBRATION: {
            int8_t *cal = (int8_t *) paket.data;
            steering->setCallibration(cal[0], cal[1]);
        }
        break;
        default:
            invalidParam("steering", paket.param);
    }
}

void ControlThread::configureAutopilot(Paket &paket) {
    NOT_IMPLEMENTED;
}

void ControlThread::configureSensor(Paket &paket) {
    NOT_IMPLEMENTED;
}

void ControlThread::handlePaket(Paket &paket) {
    switch(paket.device) {
        case Configuration::RPI: configureRpi(paket); break;
        case Configuration::ACTUATOR: configureActuator(paket); break;
        case Configuration::STEERING: configureSteering(paket); break;
        case Configuration::AUTOPILOT: configureAutopilot(paket); break;
        case Configuration::SENSOR: configureSensor(paket); break;
        default: fprintf(stderr, "invalid configuration paket with device: %d\n", paket.device);
    }
}

void ControlThread::run() {
    ads = new Ads1115();
    ads->setInputPin(3);

    leftMotor = new Motor(LEFT_MOTOR_ESC, LEFT_MOTOR_RELAIS);
    rightMotor = new Motor(RIGHT_MOTOR_ESC, RIGHT_MOTOR_RELAIS);
    leftRudder = new Servo(LEFT_RUDDER_SERVO); 
    rightRudder = new Servo(RIGHT_RUDDER_SERVO);
    topRudder = new Servo(TOP_RUDDER_SERVO);
    steering = new Steering(*leftMotor, *rightMotor, *leftRudder, *rightRudder, *topRudder);

    mpu = new Mpu6050();
    qmc = new Qmc5883l();
    bmp = new Bmp280();
    hcFront = new Hcsr04(FRONT_HCSR04_TRIG, FRONT_HCSR04_ECHO);
    hcBottom = new Hcsr04(BOTTOM_HCSR04_TRIG, BOTTOM_HCSR04_ECHO);
    camBottom = new CameraThread();

    //start the sub threads
    camBottom->start();
    /*neo6mT.start();
    camFrontT.start();
    camBottomT.start();*/

    printf("Hardware has been initialised successfully.\n");
    //the control loop of the zeppelin
    while(running) {
        pthread_mutex_lock(&dequeMutex);
        while (!paketDeque.empty()) {
            Paket *paket = paketDeque.front();
            paketDeque.pop_front();
            pthread_mutex_unlock(&dequeMutex);
            //do not block mutex, while processing paket
            handlePaket(*paket);
            delete paket->data;
            delete paket;
            pthread_mutex_lock(&dequeMutex);
        }
        pthread_mutex_unlock(&dequeMutex);

        //read out sensors and send measured data
        measureData(); //impementation is in control_thread_meas.cpp
    }

    //stop the sub threads and wait for them to terminate
    camBottom->stopRunning();
    camBottom->join();

    /*neo6mT.stopRunning();
    camFrontT.stopRunning();
    camBottomT.stopRunning();
    neo6mT.join();
    camFrontT.join();
    camBottomT.join();*/

    delete camBottom;
    delete mpu;
    delete qmc;
    delete bmp;
    delete hcBottom;
    delete hcFront;

    delete steering;
    delete leftMotor;
    delete rightMotor;
    delete leftRudder;
    delete rightRudder;
    delete topRudder;

    delete ads;
}

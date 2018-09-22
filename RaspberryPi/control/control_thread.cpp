#include <arpa/inet.h>
#include <stdint.h>
#include <pigpio.h>
#include <signal.h>

#include "makros.h"
#include "control_thread.h"

inline void invalidParam(const char *deviceName, int paramNr) {
    fprintf(stderr, "invalid %s configuration paket with param: %d\n", deviceName, paramNr);
}

ControlThread::ControlThread() : gpioHandle(-1), 
    leftMotor(NULL), rightMotor(NULL),
    leftRudder(NULL), rightRudder(NULL), topRudder(NULL),
    steering(NULL), connection(*this), running(true) {
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
    //initialise gpio
    GpioDevice::initialiseGpio();
    
    leftMotor = new Motor(LEFT_MOTOR_ESC, LEFT_MOTOR_RELAIS);
    rightMotor = new Motor(RIGHT_MOTOR_ESC, RIGHT_MOTOR_RELAIS);
    leftRudder = new Servo(LEFT_RUDDER_SERVO); 
    rightRudder = new Servo(RIGHT_RUDDER_SERVO);
    topRudder = new Servo(TOP_RUDDER_SERVO);
    steering = new Steering(*leftMotor, *rightMotor, *leftRudder, *rightRudder, *topRudder);

    //start the sub threads
    /*neo6mT.start();
    camFrontT.start();
    camBottomT.start();*/

    //the control loop of the zeppelin
    while(running) {
        pthread_mutex_lock(&dequeMutex);
        while (!paketDeque.empty()) {
            Paket *paket = paketDeque.front();
            handlePaket(*paket);
            delete paket->data;
            delete paket;
            paketDeque.pop_front();
        }
        pthread_mutex_unlock(&dequeMutex);

        //read out sensors and send measured data
    }

    //stop the sub threads and wait for them to terminate
    /*neo6mT.stopRunning();
    camFrontT.stopRunning();
    camBottomT.stopRunning();
    neo6mT.join();
    camFrontT.join();
    camBottomT.join();*/

    delete leftMotor;
    delete rightMotor;
    delete leftRudder;
    delete rightRudder;
    delete topRudder;

    //release the resources asociated with gpio
    GpioDevice::terminateGpio();
}

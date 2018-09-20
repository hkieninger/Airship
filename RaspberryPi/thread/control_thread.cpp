#include <pthread.h>
#include <arpa/inet.h>

#include "../makros.h"
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

#include "control_thread.h"

inline void invalidParam(const char *deviceName, int paramNr) {
    fprintf(stderr, "invalid %s configuration paket with param: %d\n", deviceName, paramNr);
}

ControlThread::ControlThread() : running(true) {
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
    switch(paket.param) {
        case Configuration::LEFT_MOTOR: leftMotor.setThrust(*paket.data); break;
        case Configuration::RIGHT_MOTOR: rightMotor.setThrust(*paket.data); break;
        case Configuration::LEFT_RUDDER: leftRudder.setAngle(*paket.data); break;
        case Configuration::RIGHT_RUDDER: rightRudder.setAngle(*paket.data); break;
        case Configuration::TOP_RUDDER: topRudder.setAngle(*paket.data); break;
        default: invalidParam("actuator", paket.param);
    }
}

void ControlThread::configureSteering(Paket &paket) {
    switch(paket.param) {
        case Configuration::VELOCITY:
            int8_t *data = (int8_t *) paket.data;
            steering.setVelocity(data[0]);
            break;
        case Configuration::DIRECTION:
            int8_t *data = (int8_t *) paket.data;
            steering.setDirection(data[0], data[1]);
            break;
        case Configuration::CALLIBRATION:
            int8_t *data = (int8_t *) paket.data;
            steering.setCallibration(data[0], data[1]);
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
            delete paket.data;
            delete paket;
            paketDeque.pop_front();
        }
        pthread_mutex_unlock(&dequeMutex);

        if() {

        }
    }
    //stop the sub threads and wait for them to terminate
    /*neo6mT.stopRunning();
    camFrontT.stopRunning();
    camBottomT.stopRunning();
    neo6mT.join();
    camFrontT.join();
    camBottomT.join();*/
}

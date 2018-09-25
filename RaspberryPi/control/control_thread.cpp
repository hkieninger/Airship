#include <arpa/inet.h>
#include <stdint.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/time.h>
#include <stdexcept>
#include <math.h>

#include "makros.h"
#include "control_thread.h"

inline void invalidParam(const char *deviceName, int paramNr) {
    fprintf(stderr, "invalid %s configuration paket with param: %d\n", deviceName, paramNr);
}

ControlThread::ControlThread() : gpioHandle(-1), 
    leftMotor(NULL), rightMotor(NULL),
    leftRudder(NULL), rightRudder(NULL), topRudder(NULL),
    steering(NULL), ads(NULL),
    connection(*this), running(true) {
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

uint64_t micros() {
    struct timeval tv;
    if(gettimeofday(&tv, NULL) < 0)
        throw std::runtime_error("get time of day: " + std::string(strerror(errno)));
    return tv.tv_sec + tv.tv_usec;
}

//replace with realistic function (non linear)
int8_t voltage2percentage(float voltage) {
    float batteryVoltage = voltage * (67.3f + 224) / 67.3f; //firt resistor has 224 kOhm, second resistor has 67.3 kOhm
    return roundf((100 * batteryVoltage - 620) / 2.2f); //3.1 V per cell considered as 0%, 4.2V per cell considered as 100%
}

void ControlThread::measureAds() {
    float voltage = ads->getSingleShot();
    int8_t percent = voltage2percentage(voltage);
    struct Paket paket;
    paket.device = Measurement::SENSOR;
    paket.param = Measurement::BATTERY;
    paket.len = 1;
    paket.data = (uint8_t *) &percent;
    connection.sendPaket(paket);
}

#define ADS_MEASUREMENT_RATE (500 * 1000)

void ControlThread::measureData() {
    static uint64_t lastAdsMeas = 0;
    uint64_t now = micros();
    if(now - lastAdsMeas > ADS_MEASUREMENT_RATE) {
        lastAdsMeas = now;
        measureAds();
    }
}

void ControlThread::run() {
    //initialise gpio
    GpioDevice::initialiseGpio();
    
    ads = new Ads1115();
    ads->setInputPin(3);

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
        measureData();
    }

    //stop the sub threads and wait for them to terminate
    /*neo6mT.stopRunning();
    camFrontT.stopRunning();
    camBottomT.stopRunning();
    neo6mT.join();
    camFrontT.join();
    camBottomT.join();*/

    delete steering;
    delete leftMotor;
    delete rightMotor;
    delete leftRudder;
    delete rightRudder;
    delete topRudder;
    delete ads;

    //release the resources asociated with gpio
    GpioDevice::terminateGpio();
}

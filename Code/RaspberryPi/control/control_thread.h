#ifndef CONTROL_THREAD_H
#define CONTROL_THREAD_H 1

#include <pthread.h>
#include <deque>

#include "../thread/thread.h"
#include "connection.h"
#include "algorithm/steering.h"
#include "thread/camera_thread.h"
#include "thread/jpg_camera_thread.h"
#include "../hardware/ads1115/ads1115.h"
#include "../hardware/mpu6050/mpu6050.h"
#include "../hardware/bmp280/bmp280.h"
#include "../hardware/qmc5883l/qmc5883l.h"
#include "../hardware/servo/servo.h"
#include "../hardware/motor/motor.h"
#include "../hardware/neo6m/neo6m_thread.h"

class ControlThread: public Neo6MThreadListener, public Thread {
    //the connection to the controlling computer
    Connection connection;
    //deque of incoming pakets
    std::deque<Paket *> paketDeque;
    //mutex to synchronize the deque
    pthread_mutex_t dequeMutex;

    bool running;

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
    
    //cameras
    CameraThread *camBottom;
    JpgCameraThread *camFront;
    //GPS
    Neo6MThread *neo6m;

    struct GPSGeodeticPos position;
    struct GPSGeodeticVel velocity;
    struct GPSFixStatus status;

    uint64_t lastAdsMeas = 0;
    uint64_t lastMpuMeas = 0;
    uint64_t lastBmpMeas = 0;
    uint64_t lastQmcMeas = 0;
    uint64_t lastGPSSend = 0;

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

    void sendGPS();
    void handleNavMessage(struct UBXMsg &msg);
public:
    ControlThread();
    virtual ~ControlThread();
    void stopRunning();
    void pushPaket(Paket *paket);
    virtual void run();

    virtual void onNMEAMessage(std::string &nmea, bool valid);
    virtual void onUBXMessage(struct UBXMsg &msg, bool valid);
    virtual void onACKMessage(struct UBXMsg &ack, bool valid);

    Connection &getConnection() {
        return connection;
    }
};

#endif /* CONTROL_THREAD_H */

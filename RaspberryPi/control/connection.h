#ifndef CONNECTION_H
#define CONNECTION_H 1

#include <stdint.h>
#include <pthread.h>

#include "../socket/server_socket.h"
#include "../socket/socket.h"

/*
 * Control Devices
 */
namespace Configuration {

enum ConfDevice {
    RPI, 
    ACTUATOR,
    STEERING,
    AUTOPILOT,
    SENSOR 
};

//parameters
enum ConfRPI {
    ECHO_REQUEST,
    INFO
};
enum ConfActuator {
    LEFT_MOTOR,
    RIGHT_MOTOR,
    LEFT_RUDDER,
    RIGHT_RUDDER,
    TOP_RUDDER
};
enum ConfSteering {
    VELOCITY,
    DIRECTION,
    CALLIBRATION
};
enum ConfAutopilot {
    ROUTE
};
enum {
    BATTERY,
    ACCEL,
    GYRO,
    COMPASS,
    GPS,
    BARO,
    CAM_FRONT,
    CAM_BOTTOM,
    DIST_FRONT,
    DIST_BOTTOM,
    KALMAN
};

};

/*
 * Measurment Devices
 */
namespace Measurement {

enum MeasDevice {
    RPI,
    SENSOR
};

//parameters
enum MeasRPI {
    ECHO_REPLY,
    INFO
};
enum MeasSensor {
    BATTERY,
    ACCEL,
    GYRO,
    COMPASS,
    GPS,
    BARO,
    CAM_FRONT,
    CAM_BOTTOM,
    DIST_FRONT,
    DIST_BOTTOM,
    KALMAN
};

};

#define PORT 0xCCCC
#define SYNC 0xABCD

struct Paket {
    uint8_t device;
    uint8_t param;
    uint16_t len;
    uint8_t *data = NULL;
};

//forward declaration of ControlThread, only possible since only reference to it is needed
class ControlThread;

class Connection {
    ServerSocket server;
    Socket *sock;
    ControlThread &control;
    pthread_mutex_t sendMutex;

    uint64_t lastEchoRequest;

    void syncSocket();
    void sendEchoReply();
public:
    Connection(ControlThread &control);
    ~Connection();

    void loop();
    bool sendPaket(Paket &paket);
    bool isConnected();
    bool isLost();
};

#endif /* CONNECTION_H */
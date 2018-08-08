#include <stdio.h>
#include <stdio.h>
#include <stdint.h>
#include <byteswap.h>

#include "../socket/socket_exception.h"
#include "../socket/server_socket.h"
#include "../socket/socket.h"

#define DEFAULT_PORT 0xCCCC

#define SYNC 0xABCD

/*
* Device IDs
*/
#define D_RASPBERRY 0

//Actuators
#define D_LEFT_MOTOR 2
#define D_RIGHT_MOTOR 3

#define D_LEFT_RUDDER 4
#define D_RIGHT_RUDDER 5
#define D_TOP_RUDDER 6

//Sensors
#define D_BATTERY 1
#define D_NEO6M 7
#define D_HCSRO4_FRONT 8
#define D_HCSRO4_DOWN 9
#define D_BMP280 10
#define D_QMC5883L 11
#define D_MPU6050 12

/*
* Parameter IDs
* if the most significant bit is set, its not setting the parameter but requesting it
*/
#define P_ECHO_REQUEST 0 //D_RASPBERRY
#define P_ECHO_REPLY 1 //D_RASPBERRY

//Actuators
#define P_THRUST 0 //D_*_MOTOR
#define P_ANGLE 0 //D_*_RUDDER

//Sensors
#define P_PERCENT 0 //D_BATTERY

//Pins and port
#define PORT 0xCCCC

#define LEFT_MOTOR_ESC
#define LEFT_MOTOR_RELAIS
#define RIGHT_MOTOR_ESC
#define RIGHT_MOTOR_RELAIS

#define LEFT_RUDDER_SERVO
#define RIGHT_RUDDER_SERVO
#define TOP_RUDDER_SERVO


struct Paket {
    uint8_t device;
    uint8_t parameter;
    void *data;
    int len;
};

bool isRunning = true;

void handleSignal(int sig) {
    if(sig == SIGINT || sig == SIGTERM) {
        isRunning = false;
    }
}

void sendPaket(Socket &sock, struct Paket &paket) {
    uint16_t sync = bswap_16(SYNC);
    sock.sendAll(&sync, 2);
    sock.sendAll(&paket.device, 1);
    sock.sendAll(&paket.parameter, 1);
    sock.sendAll(paket.data, paket.len);
}

void handleRaspberry(Socket &sock, uint8_t param) {
    if(param == P_ECHO_REQUEST) {
        int64_t timestamp;
        sock.recvAll(&timestamp, 8);
        struct Paket paket;
        paket.device = D_RASPBERRY;
        paket.parameter = P_ECHO_REPLY;
        paket.data = &timestamp;
        paket.len = 8;
        sendPaket(sock, paket);
    } else
        fprintf(stderr, "D_RASPBERRY invalide param: %d\n", param);
}

void handleMotor(Socket &sock, Motor &motor, uint8_t param) {
    if(param == P_THRUST) {
        int32_t thrust;
        sock.recvAll(&thrust, 4);
        motor.setThrust(thrust);
    } else
        fprintf(stderr, "D_LEFT_MOTOR invalide param: %d\n", param);
}

void handleRudder(Socket &sock, Rudder &rudder, uint8_t param) {
    if(param == P_ANGLE) {
        int32_t angle;
        sock.recvAll(&angle, 4);
        rudder.setAngle(angle);
    } else
        fprintf(stderr, "D_LEFT_MOTOR invalide param: %d\n", param);
}

void syncSocket(Socket &sock) {
    uint8_t lastByte, byte;
    //go to the next sync mark
    sock.recvAll(&lastByte, 1);
    sock.recvAll(&byte, 1);
    while((lastByte << 8 | byte) != SYNC) {
        lastByte = byte;
        sock.recvAll(&byte, 1);
    }
}


int main() {
    //setup signal handling
    signal(SIGINT, handleSignal);
    signal(SIGTERM, handleSignal);
    //setup the hardware
    Motor leftMotor(LEFT_MOTOR_ESC, LEFT_MOTOR_RELAIS), rightMotor(RIGHT_MOTOR_ESC, RIGHT_MOTOR_RELAIS);
    Rudder leftRudder(LEFT_RUDDER_SERVO), rightRudder(RIGHT_RUDDER_SERVO), topRudder(TOP_RUDDER_SERVO);
    //start listening on the port
    ServerSocket server(PORT);
    //accept incoming connections until programm gets a signal to stop, see handleSignal()
    while(isRunning) {
        Socket sock = server.acceptConnection();
        printf("client %s has connected\n", sock.getRemoteIPString().c_str());
        try {
            while(true) {
                syncSocket(sock);
                //read the header
                uint8_t header[2];
                sock.recvAll(header, 2);
                //read and handle the data
                switch(header[0]) {
                    case D_RASPBERRY: handleRaspberry(sock, header[1]); break;
                    case D_LEFT_MOTOR: handleMotor(sock, leftMotor, header[1]); break;
                    case D_RIGHT_MOTOR: handleMotor(sock, rightMotor, header[1]); break;
                    case D_LEFT_RUDDER: handleRudder(sock, leftRudder, header[1]); break;
                    case D_RIGHT_RUDDER: handleRudder(sock, rightRudder, header[1]); break;
                    case D_TOP_RUDDER: handleRudder(sock, topRudder, header[1]); break;
                    default:
                        fprintf(stderr, "unknow device: %d\n", header[0]);
                }
            }
        } catch(SocketClosedException &e) {
            printf("connection has been closed by client\n");
        }
    }
    return EXIT_SUCCESS;
}
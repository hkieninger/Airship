#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <byteswap.h>
#include <signal.h>
#include <pigpio.h>

#include "../socket/socket_exception.h"
#include "../socket/server_socket.h"
#include "../socket/socket.h"
#include "../servo/servo.h"
#include "../motor/motor.h"

#define SYNC 0xABCD
#define PORT 0xCCCC

#define D_RASPBERRY 0
#define P_ECHO_REQUEST 0
#define P_ECHO_REPLY 0

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
                    default:
                        printf("device: %d / parameter: %d\n", header[0], header[1]);
                }
            }
        } catch(SocketClosedException &e) {
            printf("connection has been closed by client\n");
        }
    }
    return EXIT_SUCCESS;
}
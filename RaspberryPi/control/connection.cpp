#include <stdio.h>
#include <arpa/inet.h>
#include <string>

#include "../socket/socket_exception.h"
#include "control_thread.h"

#include "connection.h"

Connection::Connection(ControlThread &control) : server(PORT), sock(NULL), control(control) {
    pthread_mutex_init(&sendMutex, NULL);
}

Connection::~Connection() {
    pthread_mutex_destroy(&sendMutex);
}

void Connection::syncSocket() {
    uint8_t lastByte, byte;
    //go to the next sync mark
    sock->recvAll(&lastByte, 1);
    sock->recvAll(&byte, 1);
    while((lastByte << 8 | byte) != SYNC) {
        lastByte = byte;
        sock->recvAll(&byte, 1);
    }
}

void Connection::sendEchoReply() {
    int64_t timestamp;
    sock->recvAll(&timestamp, 8);
    struct Paket paket;
    paket.device = Measurement::RPI;
    paket.param = Measurement::ECHO_REPLY;
    paket.data = (uint8_t *) &timestamp;
    paket.len = 8;
    sendPaket(paket);
}

bool Connection::sendPaket(Paket &paket) {
    if(!isConnected())
        return false;
    pthread_mutex_lock(&sendMutex);
    uint16_t sync = htons(SYNC);
    sock->sendAll(&sync, 2);
    sock->sendAll(&paket.device, 1);
    sock->sendAll(&paket.param, 1);
    uint16_t bigendian = htons(paket.len);
    sock->sendAll(&bigendian, 2);
    sock->sendAll(paket.data, paket.len);
    pthread_mutex_unlock(&sendMutex);
    return true;
}

bool Connection::isConnected() {
    return sock != NULL;
}

void Connection::loop() {
    //loop for accepting incoming connections
    while(true) { //exited when interrupted exception is thrown
        Socket connectedSocket = server.acceptConnection();
        sock = &connectedSocket;
        printf("client %s has connected\n", sock->getRemoteIPString().c_str());
        try {
            //loop for handling connections
            while(true) { //exited when socket (closed) exception is thrown
                syncSocket();
                uint8_t header[4];
                sock->recvAll(header, 4);
                if(header[0] == Configuration::RPI && header[1] == Configuration::ECHO_REQUEST) {
                    sendEchoReply();
                } else {
                    Paket *paket = new Paket(); //don't forget to delete
                    paket->device = header[0];
                    paket->param = header[1];
                    paket->len = ntohs(*((uint16_t *) (header + 2)));
                    paket->data = new uint8_t[paket->len]; //don't forget to delete
                    sock->recvAll(paket->data, paket->len);
                    control.pushPaket(paket);
                }
            }
        } catch(const SocketClosedException &e) {
            printf("Connection has been closed by client.\n");
        } catch(const SocketException &e) {
            fprintf(stderr, "The following socket exception has occured: %s\n", e.what());
        }
        sock = NULL;
    }
}
#include <stdio.h>
#include <arpa/inet.h>
#include <string>
#include <errno.h>
#include <string.h>
#include <sys/time.h>
#include <stdexcept>

#include "../socket/socket_exception.h"
#include "../thread/interrupted_exception.h"
#include "control_thread.h"

#include "connection.h"

#define CONNECTION_LOST_TIME (10 * 1000 * 1000)
#define RECEIVE_TIMEOUT (CONNECTION_LOST_TIME / 1000)

static uint64_t micros() {
    struct timeval tv;
    if(gettimeofday(&tv, NULL) < 0)
        throw std::runtime_error("get time of day: " + std::string(strerror(errno)));
    return 1000 * 1000 * tv.tv_sec + tv.tv_usec;
}

Connection::Connection(ControlThread &control) : server(PORT), sock(NULL), control(control), lastEchoRequest(0) {
    pthread_mutex_init(&sendMutex, NULL);
    pthread_mutex_init(&echoMutex, NULL);
}

Connection::~Connection() {
    pthread_mutex_destroy(&sendMutex);
    pthread_mutex_destroy(&echoMutex);
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
    try {
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
    } catch(const SocketException &e) {
        pthread_mutex_unlock(&sendMutex);
        return false;
    } catch(const InterruptedException &e) {
        pthread_mutex_unlock(&sendMutex);
        return false;
    }
}

bool Connection::isConnected() {
    return sock != NULL;
}

bool Connection::isLost() {
    pthread_mutex_lock(&echoMutex);
    bool lost = micros() - lastEchoRequest > CONNECTION_LOST_TIME;
    pthread_mutex_unlock(&echoMutex);
    return lost;
}

void Connection::loop() {
    //loop for accepting incoming connections
    while(true) { //exited when interrupted exception is thrown
        Socket connectedSocket = server.acceptConnection();
        sock = &connectedSocket;
        sock->setRecvTimeout(RECEIVE_TIMEOUT);
        printf("client %s has connected\n", sock->getRemoteIPString().c_str());
        try {
            //loop for handling connections
            while(true) { //exited when socket (closed) exception is thrown
                syncSocket();
                uint8_t header[4];
                sock->recvAll(header, 4);
                if(header[0] == Configuration::RPI && header[1] == Configuration::ECHO_REQUEST) {
                    sendEchoReply();
                    pthread_mutex_lock(&echoMutex);
                    lastEchoRequest = micros();
                    pthread_mutex_unlock(&echoMutex);
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
        } catch(const TimeoutException &e) {
            fprintf(stderr, "Connection to client has been lost: %s\n", e.what());
        } catch(const SocketClosedException &e) {
            printf("Connection has been closed by client: %s\n", e.what());
        } catch(const SocketException &e) {
            fprintf(stderr, "The following socket exception has occured: %s\n", e.what());
        }
        sock = NULL;
    }
}
#include <stdio.h>
#include <stdint.h>
#include <arpa/inet.h>
#include <string>

#include "../socket/server_socket.h"
#include "../socket/socket.h"
#include "../socket/socket_exception.h"
#include "control_thread.h"

#include "connection.h"

Connection::Connection(ControlThread &control) : control(control) {
    
}

Connection::~Connection() {

}

void Connection::syncSocket(Socket &sock) {
    uint8_t lastByte, byte;
    //go to the next sync mark
    sock.recvAll(&lastByte, 1);
    sock.recvAll(&byte, 1);
    while((lastByte << 8 | byte) != SYNC) {
        lastByte = byte;
        sock.recvAll(&byte, 1);
    }
}

void Connection::sendEchoReply(uint8_t param) {
    int64_t timestamp;
    sock.recvAll(&timestamp, 8);
    struct Paket paket;
    paket.device = Measurement::RPI;
    paket.parameter = Measurement::ECHO_REPLY;
    paket.data = &timestamp;
    paket.len = 8;
    sendPaket(sock, paket);
}

void Connection::sendPaket(Paket &paket) {
    uint16_t sync = bswap_16(SYNC);
    sock.sendAll(&sync, 2);
    sock.sendAll(&paket.device, 1);
    sock.sendAll(&paket.parameter, 1);
    uint16_t bigendian = htons(paket.len);
    sock.sendAll(&bigendian, 2);
    sock.sendAll(paket.data, paket.len);
}

void Connection::loop() {
    //loop for accepting incoming connections
    while(true) { //exited when interrupted exception is thrown
        sock = server.acceptConnection();
        printf("client %s has connected\n", sock.getRemoteIPString().c_str());
        try {
            //loop for handling connections
            while(true) { //exited when socket (closed) exception is thrown
                syncSocket();
                uint8_t header[4];
                sock.recvAll(header, 4);
                if(header[0] == Configuration::RPI && header[1] == Configuration::ECHO_REQUEST)
                    sendEchoReply();
                else {
                    Paket *paket = new Paket(); //don't forget to delete
                    paket.device = header[0];
                    paket.param = header[1];
                    paket.len = ntohs(*((uint16_t *) (header + 2)));
                    paket.data = new uint8_t[paket.len]; //don't forget to delete
                    socket.recvAll(paket.data, paket.len);
                    control.pushPacket(paket);
                }
            }
        } catch(const SocketClosedException &e) {
            printf("Connection has been closed by client.\n");
        } catch(const SocketException &e) {
            fprintf(stderr, "The following socket exception has occured: %s\n", e.what().c_str());
        }
    }
}
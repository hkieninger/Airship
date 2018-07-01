#include <wiringSerial.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h>

#include <stdexcept>
#include <string>

#include "../gpio_exception.h"
#include "neo6m.h"


Neo6M::Neo6M(const std::string &serialport) {
    fd = serialOpen(serialport.c_str(), DEFAULT_BAUDRATE);
    if(fd < 0)
        throw UARTException("opening serial port: " + std::string(strerror(errno)));
}

Neo6M::~Neo6M() {
    serialClose(fd);
}

void Neo6M::wakeUp() {
    uint8_t sequence = 0xFF;
    if(write(fd, &sequence, 1) < 0)
        throw UARTException("writing to UART: " + std::string(strerror(errno)));
}

void Neo6M::flushSerial() {
    serialFlush(fd);
}

int Neo6M::getFd() { //DEBUG
    return fd;
}

void Neo6M::reset() {
    uint8_t msg[] = {0, 0, 1, 0};
    sendUBXMessage(0x06, 0x04, 4, msg); //performs a complete software reset, followed by a hotstart
}

void Neo6M::setInactiveState() {

}

void Neo6M::pollUBXMessage(uint8_t cls, uint8_t id) {
    sendUBXMessage(cls, id, 0, NULL);
}

void Neo6M::sendUBXMessage(uint8_t cls, uint8_t id, uint16_t length, uint8_t *playload) {
    uint16_t sync = 0x62B5; //because of little endian the order it will be sent will be swapped
    uint16_t checksum = calcUBXChecksum(cls, id, length, playload);
    writeAll(&sync, 2);
    writeAll(&cls, 1);
    writeAll(&id, 1);
    writeAll(&length, 2);
    writeAll(playload, length);
    writeAll(&checksum, 2);
}

uint16_t Neo6M::calcUBXChecksum(uint8_t cls, uint8_t id, uint16_t length, uint8_t *msg) {
    uint8_t sumA = cls, sumB = cls;
    sumA += id;
    sumB += sumA;
    sumA += length & 0xFF;
    sumB += sumA;
    sumA += length >> 8 & 0xFF;
    sumB += sumA;
    for(int i = 0; i < length; i++) {
        sumA += msg[i];
        sumB += sumA;
    }
    return sumA | sumB << 8;
}

void Neo6M::disableNMEAMessage(const std::string &msg) {
    if(msg.length() != 3)
        throw std::length_error("NMEA message identifier must contain 3 characters");
    char buffer[24]; //don't forget \0
    if(snprintf(buffer, sizeof(buffer), "PUBX,40,%s,0,0,0,0,0,0", msg.c_str()) != sizeof(buffer) - 1)
        throw std::runtime_error("creating NMEA message: " + std::string(strerror(errno)));
    sendNMEAMessage(buffer);
}

void Neo6M::enableNMEAMessage(const std::string &msg) {
    if(msg.length() != 3)
        throw std::length_error("NMEA message identifier must contain 3 characters");
    char buffer[24]; //don't forget \0
    if(snprintf(buffer, sizeof(buffer), "PUBX,40,%s,0,1,0,0,0,0", msg.c_str()) != sizeof(buffer) - 1)
        throw std::runtime_error("creating NMEA message: " + std::string(strerror(errno)));
    sendNMEAMessage(buffer);
}

void Neo6M::sendNMEAMessage(const std::string &msg) {
    char buffer[msg.length() + 7]; //don't forget \0
    uint8_t checksum = calcNMEAChecksum(msg);
    if(snprintf(buffer, sizeof(buffer), "$%s*%.2X\r\n", msg.c_str(), checksum) != sizeof(buffer) - 1)
        throw std::runtime_error("creating NMEA message: " + std::string(strerror(errno)));
    writeAll(buffer, sizeof(buffer)-1);
}

uint8_t Neo6M::calcNMEAChecksum(const std::string &msg) {
    uint8_t sum = 0;
    for(int i = 0; i < msg.length(); i++) {
        sum ^= msg[i];
    }
    return sum;
}

void Neo6M::writeAll(const void *buf, size_t count) {
    int sent = 0;
    while(sent < count) {
        int ret = write(fd, (uint8_t *) buf + sent, count - sent);
        if(ret < 0)
            throw UARTException("writing to UART: " + std::string(strerror(errno)));
        sent += ret;
    }
}
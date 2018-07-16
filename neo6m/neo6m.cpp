#include <wiringSerial.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h>

#include <stdexcept>
#include <string>

#include "neo6m_exception.h"
#include "../gpio/uart_dev.h"
#include "neo6m.h"

Neo6M::Neo6M(const std::string &serialport) : UARTDev(serialport) {
    powerOn();
    clearConfiguration();
    loadConfiguration();
    setNMEA(false);

    //set dynamic platform model to pedestrian
    struct UBXMsg msg;
    msg.cls = NEO6M_CLS_CFG;
    msg.id = NEO6M_CFG_NAV5;
    msg.length = 36;
    uint8_t playload[msg.length];
    memset(playload, 0, msg.length);
    playload[0] = 1;
    playload[2] = 3;
    msg.playload = playload;
    sendUBXMessage(msg);
}

Neo6M::~Neo6M() {
    setNMEA(false);
    clearConfiguration();
    powerOff(0); //poweroff the device for saving power
    serialClose(fd);
}

struct UBXMsg &Neo6M::receiveUBXMessage(struct UBXMsg &msg) {
    uint8_t lastByte = getChar();
    uint8_t byte = getChar();
    while((lastByte | byte << 8) != UBX_SYNC) {
        lastByte = byte;
        byte = getChar();
    }
    uint16_t checksum; //because of little endian the order will be swapped
    readAll(&msg.cls, 1);
    readAll(&msg.id, 1);
    readAll(&msg.length, 2);
    readAll(msg.playload, msg.length);
    readAll(&checksum, 2);
    if(checksum != calcUBXChecksum(msg))
        throw Neo6MException("receive message: checksums don't match");
    return msg;
}

struct UBXMsg &Neo6M::pollUBXMessage(struct UBXMsg &msg) {
    /* Note for messages of the class CFG:
     * the neo6m sends first the polled message and 
     * then the acknowledge message or onyl a not acknowledge message */
    msg.length = 0;
    msg.playload = NULL;
    sendUBXMessage(msg);
    receiveUBXMessage(msg);
    if(msg.cls == NEO6M_CLS_CFG) {
        if(msg.id == NEO6M_ACK_NAK)
            throw Neo6MException("received not acknowledge message");
        else {
            struct UBXMsg ack;
            receiveUBXMessage(ack);
            if(msg.id != NEO6M_ACK_ACK)
                throw Neo6MException("acknowledge message not received");
        }
    }
    return msg;
}

void Neo6M::sendUBXMessage(struct UBXMsg &msg) {
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

void Neo6M::reset() {
    uint8_t playload[] = {0, 0, 1, 0};
    sendUBXMessage(0x06, 0x04, sizeof(playload), playload); //performs a complete software reset, followed by a hotstart
}

//duration of powersave mode in ms, 0 means infinit duration
void Neo6M::powerOff(uint32_t duration) {
    uint32_t playload[] = {duration, 0x02};
    sendUBXMessage(0x02, 0x41, sizeof(playload), (uint8_t *) playload);
}

void Neo6M::powerOn() {
    uint8_t sequence = 0xFF;
    if(writeAll(&sequence, 1) < 0)
        throw UARTException("writing to UART: " + std::string(strerror(errno)));
    //add usleep(500 * 1000) ?
}

void Neo6M::setSBAS(bool enabled) {
    uint8_t mode;
    if(enabled)
        mode = 0x03;
    else
        mode = 0x00;
    uint8_t playload[] = {mode, 0x07, 0x03, 0x00, 0x59, 0x62, 0x07, 0x00}; //adapt when satelites change PRNs
    neo->sendUBXMessage(0x06, 0x16, 8, playload);
}

void Neo6M::setPPP(bool enabled) {
    pollUBXMessage(0x06, 0x23);
    uint8_t config[46];

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
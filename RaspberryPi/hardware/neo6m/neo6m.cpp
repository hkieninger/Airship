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

Neo6M::Neo6M(const std::string &serialport, int baudrate) : UARTDev(serialport, baudrate) {
    powerOn();
    clearConfiguration();
    loadConfiguration();
    setProtocol(UBX_ONLY);

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

    //message rates are 0 by default
}

Neo6M::~Neo6M() {
    setProtocol(UBX_ONLY);
    clearConfiguration();
    powerOff(0); //poweroff the device for saving power
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
     * then the acknowledge message or only a not acknowledge message */
    uint8_t cls = msg.cls;
    msg.length = 0;
    sendUBXStructure(msg);
    receiveUBXMessage(msg);
    if(cls == NEO6M_CLS_CFG) {
        if(msg.cls == NEO6M_CLS_ACK/* && msg.id == NEO6M_ACK_NAK*/)
            throw Neo6MException("received not acknowledge message");
        else {
            struct UBXMsg ack;
            uint8_t playload[2];
            ack.playload = playload;
            receiveUBXMessage(ack);
            if(/*msg.cls == NEO6M_CLS_ACK && */ack.id != NEO6M_ACK_ACK)
                throw Neo6MException("acknowledge message not received");
        }
    }
    return msg;
}

void Neo6M::sendUBXMessage(const struct UBXMsg &msg) {
    sendUBXStructure(msg);
    if(msg.cls == NEO6M_CLS_CFG) {
        struct UBXMsg ack;
        uint8_t playload[2];
        ack.playload = playload;
        receiveUBXMessage(ack);
        if(/*msg.cls == NEO6M_CLS_ACK && */ack.id != NEO6M_ACK_ACK)
            throw Neo6MException("acknowledge message not received");
    }
}

void Neo6M::sendUBXStructure(const struct UBXMsg &msg) {
    uint16_t sync = UBX_SYNC; //because of little endian the order it will be sent will be swapped
    uint16_t checksum = calcUBXChecksum(msg);
    writeAll(&sync, 2);
    writeAll(&msg.cls, 1);
    writeAll(&msg.id, 1);
    writeAll(&msg.length, 2);
    writeAll(msg.playload, msg.length);
    writeAll(&checksum, 2);
}

uint16_t Neo6M::calcUBXChecksum(const struct UBXMsg &msg) {
    uint8_t sumA = msg.cls, sumB = msg.cls;
    sumA += msg.id;
    sumB += sumA;
    sumA += msg.length & 0xFF;
    sumB += sumA;
    sumA += msg.length >> 8 & 0xFF;
    sumB += sumA;
    for(int i = 0; i < msg.length; i++) {
        sumA += msg.playload[i];
        sumB += sumA;
    }
    return sumA | sumB << 8;
}

void Neo6M::reset() {
    uint8_t playload[] = {0, 0, 1, 0};
    struct UBXMsg msg;
    msg.cls = NEO6M_CLS_CFG;
    msg.id = NEO6M_CFG_RST;
    msg.length = sizeof(playload);
    msg.playload = playload;
    sendUBXMessage(msg); //performs a complete software reset, followed by a hotstart
}

//duration of powersave mode in ms, 0 means infinit duration
void Neo6M::powerOff(uint32_t duration) {
    uint32_t playload[] = {duration, 0x02};
    struct UBXMsg msg;
    msg.cls = NEO6M_CLS_RXM;
    msg.id = NEO6M_RXM_PMREQ;
    msg.length = sizeof(playload);
    msg.playload = (uint8_t *) playload;
    sendUBXMessage(msg);
}

void Neo6M::powerOn() {
    uint8_t sequence = 0xFF;
    writeAll(&sequence, 1);
    usleep(500 * 1000);
}

/* not tested
void Neo6M::setMessageRate(uint8_t cls, uint8_t id, uint8_t rate) {
    uint8_t playload[] = {cls, id, rate};
    struct UBXMsg msg;
    msg.cls = NEO6M_CLS_CFG;
    msg.id = NEO6M_CFG_MSG;
    msg.length = sizeof(playload);
    msg.playload = playload;
    sendUBXMessage(msg);
}*/

struct GPSFixStatus &Neo6M::getFixStatus(struct GPSFixStatus &status) {
    struct UBXMsg msg;
    msg.cls = NEO6M_CLS_NAV;
    uint8_t playload[8 + 12 * 56];
    msg.playload = playload;

    msg.id = NEO6M_NAV_STATUS;
    pollUBXMessage(msg);
    status.time = ((uint32_t *) msg.playload)[0];
    status.gpsFix = msg.playload[4];
    status.dgps = msg.playload[5] & 0x02;
    status.fixValid = msg.playload[5] & 0x01;
    status.timeValid = msg.playload[5] & 0x04 && msg.playload[5] & 0x08;

    msg.id = NEO6M_NAV_SVINFO;
    pollUBXMessage(msg);
    status.satellites = msg.playload[4];

    return status;
}

struct GPSEcefPos &Neo6M::getEcefPos(struct GPSEcefPos &pos) {
    struct UBXMsg msg;
    msg.cls = NEO6M_CLS_NAV;
    msg.id = NEO6M_NAV_POSECEF;
    msg.playload = (uint8_t *) &pos;
    pollUBXMessage(msg);
    return pos;
}

struct GPSEcefVel &Neo6M::getEcefVel(struct GPSEcefVel &vel) {
    struct UBXMsg msg;
    msg.cls = NEO6M_CLS_NAV;
    msg.id = NEO6M_NAV_VELECEF;
    msg.playload = (uint8_t *) &vel;
    pollUBXMessage(msg);
    return vel;
}

struct GPSGeodeticPos &Neo6M::getGeodeticPos(struct GPSGeodeticPos &pos) {
    struct UBXMsg msg;
    msg.cls = NEO6M_CLS_NAV;
    msg.id = NEO6M_NAV_POSLLH;
    msg.playload = (uint8_t *) &pos;
    pollUBXMessage(msg);
    return pos;
}

struct GPSGeodeticVel &Neo6M::getGeodeticVel(struct GPSGeodeticVel &vel) {
    struct UBXMsg msg;
    msg.cls = NEO6M_CLS_NAV;
    msg.id = NEO6M_NAV_VELNED;
    msg.playload = (uint8_t *) &vel;
    pollUBXMessage(msg);
    return vel;
}

void Neo6M::setSBAS(bool enabled) {
    uint8_t mode;
    if(enabled)
        mode = 0x03;
    else
        mode = 0x00;
    uint8_t playload[] = {mode, 0x07, 0x03, 0x00, 0x59, 0x62, 0x07, 0x00}; //adapt when satellites change PRNs
    struct UBXMsg msg;
    msg.cls = NEO6M_CLS_CFG;
    msg.id = NEO6M_CFG_SBAS;
    msg.length = sizeof(playload);
    msg.playload = playload;
    sendUBXMessage(msg);
}

void Neo6M::setPPP(bool enabled) {
    uint8_t playload[40];
    memset(playload, 0, sizeof(playload));
    ((uint16_t *) playload)[1] = 0x2000;
    playload[26] = enabled;
    struct UBXMsg msg;
    msg.cls = NEO6M_CLS_CFG;
    msg.id = NEO6M_CFG_NAVX5;
    msg.length = sizeof(playload);
    msg.playload = playload;
    sendUBXMessage(msg);
}

void Neo6M::clearConfiguration() {
    uint32_t playload[3] = {0x0000061F, 0, 0};
    struct UBXMsg msg;
    msg.cls = NEO6M_CLS_CFG;
    msg.id = NEO6M_CFG_CFG;
    msg.length = sizeof(playload);
    msg.playload = (uint8_t *) playload;
    sendUBXMessage(msg);
}

void Neo6M::saveConfiguration() {
    uint32_t playload[3] = {0, 0x0000061F, 0};
    struct UBXMsg msg;
    msg.cls = NEO6M_CLS_CFG;
    msg.id = NEO6M_CFG_CFG;
    msg.length = sizeof(playload);
    msg.playload = (uint8_t *) playload;
    sendUBXMessage(msg);
}

void Neo6M::loadConfiguration() {
    uint32_t playload[3] = {0, 0, 0x0000061F};
    struct UBXMsg msg;
    msg.cls = NEO6M_CLS_CFG;
    msg.id = NEO6M_CFG_CFG;
    msg.length = sizeof(playload);
    msg.playload = (uint8_t *) playload;
    sendUBXMessage(msg);
}

void Neo6M::setMeasurementRate(uint16_t millis) {
    uint16_t playload[] = {millis, 1, 0};
    struct UBXMsg msg;
    msg.cls = NEO6M_CLS_CFG;
    msg.id = NEO6M_CFG_RATE;
    msg.length = sizeof(playload);
    msg.playload = (uint8_t *) playload;
    sendUBXMessage(msg);
}

void Neo6M::setProtocol(bool nmea_enabled) {
    uint8_t playload[20];
    struct UBXMsg msg;
    msg.cls = NEO6M_CLS_CFG;
    msg.id = NEO6M_CFG_PRT;
    msg.playload = playload;
    pollUBXMessage(msg); //get current configuration
    playload[12] = 0x01 | nmea_enabled << 1;
    playload[14] = 0x01 | nmea_enabled << 1;
    sendUBXMessage(msg); //set configuration
}



void Neo6M::disableNMEAMessage(const std::string &msg) {
    if(msg.length() != 3)
        throw std::length_error("NMEA message identifier must contain 3 characters");
    char buffer[24]; //don't forget \0
    if(snprintf(buffer, sizeof(buffer), "PUBX,40,%s,0,0,0,0,0,0", msg.c_str()) != (int) (sizeof(buffer) - 1))
        throw std::runtime_error("creating NMEA message: " + std::string(strerror(errno)));
    sendNMEAMessage(buffer);
}

void Neo6M::enableNMEAMessage(const std::string &msg) {
    if(msg.length() != 3)
        throw std::length_error("NMEA message identifier must contain 3 characters");
    char buffer[24]; //don't forget \0
    if(snprintf(buffer, sizeof(buffer), "PUBX,40,%s,0,1,0,0,0,0", msg.c_str()) != (int) (sizeof(buffer) - 1))
        throw std::runtime_error("creating NMEA message: " + std::string(strerror(errno)));
    sendNMEAMessage(buffer);
}

void Neo6M::sendNMEAMessage(const std::string &msg) {
    char buffer[msg.length() + 7]; //don't forget \0
    uint8_t checksum = calcNMEAChecksum(msg);
    if(snprintf(buffer, sizeof(buffer), "$%s*%.2X\r\n", msg.c_str(), checksum) != (int) (sizeof(buffer) - 1))
        throw std::runtime_error("creating NMEA message: " + std::string(strerror(errno)));
    writeAll(buffer, sizeof(buffer)-1);
}

static int8_t ascii2hex(char ascii) {
    if(ascii >= '0' && ascii <= '9')
        return ascii - '0';
    else if(ascii >= 'a' && ascii <= 'f')
        return ascii - 'a' + 10;
    else if(ascii >= 'A' && ascii <= 'F')
        return ascii - 'A' + 10;
    return -1;
}

std::string Neo6M::readNMEAMessage() {
    //discard rubbish in the buffer and look for the beginning of a NMEA message
    std::string str("$GP");
    char lastlastC = getChar();
    char lastC = getChar();
    char c = getChar();
    while(lastlastC != str[0] || lastC != str[1] || c != str[2]) {
        lastlastC = lastC;
        lastC = c;
        c = getChar();
    }
    //read the NMEA message
    do {
        c = getChar();
        str.push_back(c);
    } while(c != '*');
    //read and compare the checksum
    str.push_back(getChar());
    str.push_back(getChar());
    int len = str.length();
    uint8_t checksum = ascii2hex(str[len - 2]) << 4 | ascii2hex(str[len - 1]);
    if(checksum != calcNMEAChecksum(str.substr(1, len - 4)))
        throw Neo6MException("read NMEA message: checksum don't match");
    str.push_back(getChar()); // \r
    str.push_back(getChar()); // \n
    return str;
}

uint8_t Neo6M::calcNMEAChecksum(const std::string &msg) {
    uint8_t sum = 0;
    for(unsigned int i = 0; i < msg.length(); i++) {
        sum ^= msg[i];
    }
    return sum;
}
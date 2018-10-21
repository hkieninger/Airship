#include <unistd.h>
#include <time.h>
#include <errno.h>
#include <string.h>
#include <string>
#include <stdexcept>
#include <stdio.h>

#include "neo6m_exception.h"
#include "neo6m_thread.h"  

/*
 * TODO: implement error handling for pthread functions
 */

Neo6MThread::Neo6MThread(Neo6MThreadListener &listener, const std::string &serialport, int baudrate) : 
    UARTDev(serialport, baudrate), Thread("neo6m thread"), listener(listener), running(true), measurementRate(1000) {

    flush();

    pthread_mutex_init(&sendMutex, NULL);
    pthread_mutex_init(&queueMutex, NULL);
    
    powerOn();

    start(); //start the thread

    clearConfiguration();
    //loadConfiguration();
    setSBAS(true);
    //setProtocol(UBX_AND_NMEA); default

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
    sendAndWaitUBXMsg(msg);

    //message rates of UBX are 0 by default

    //set message rate of NMEA to 0
    disableNMEAMessage("GSV");
    disableNMEAMessage("GSA");
    disableNMEAMessage("GLL");
    disableNMEAMessage("VTG");
    disableNMEAMessage("GGA");
    disableNMEAMessage("RMC");

    usleep(250 * 1000); //sleep 250ms, it's necessary
}

Neo6MThread::~Neo6MThread() {
    /* //reset the configuration to default
    clearConfiguration();
    loadConfiguration();
    //poweroff the device for saving power
    powerOff(0); */

    running = false;
    join();

    pthread_mutex_destroy(&queueMutex);
    pthread_mutex_destroy(&sendMutex);
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

#define AVAILABLE_DELAY (500 * 1000)

void Neo6MThread::run() {
    while(running) {
        //check if data is available, to avoid blocking a long time, which would make the thread unresponsive to stopRunning()
        if(available() > 2) {
            //go to the next synchronization mark in the stream
            uint8_t lastByte = getChar();
            uint8_t byte = getChar();
            while(((lastByte | byte << 8) != UBX_SYNC) && (lastByte != '$' || byte != 'G')) {
                //printf("skipping byte %X, lastByte %X\n", byte, lastByte); //DEBUG
                lastByte = byte;
                byte = getChar();
            }
            //determine if the next message is UBX or NMEA
            if(lastByte == '$') { //NMEA
                std::string str("$G");
                char c;
                //read the message
                do {
                    c = getChar();
                    str.push_back(c);
                } while(c != '*');
                //read and compare the checksum
                str.push_back(getChar());
                str.push_back(getChar());
                int len = str.length();
                uint8_t checksum = ascii2hex(str[len - 2]) << 4 | ascii2hex(str[len - 1]);
                bool valid = (checksum == calcNMEAChecksum(str.substr(1, len - 4)));
                //read the ending
                str.push_back(getChar()); // \r
                str.push_back(getChar()); // \n
                
                //DEBUG
                if(str[str.length() - 1] != '\n')
                    printf("NMEA error\n");

                //call the callback
                listener.onNMEAMessage(str, valid);
            } else { //UBX
                //read the message
                struct UBXMsg msg;
                uint16_t checksum; //because of little endian the order will be swapped
                readAll(&msg.cls, 1);
                readAll(&msg.id, 1);
                readAll(&msg.length, 2);
                msg.playload = new uint8_t[msg.length];
                readAll(msg.playload, msg.length);
                readAll(&checksum, 2);
                //compare the checksum
                bool valid = (checksum == calcUBXChecksum(msg));

                //other thread is waiting for the message?

                //DEBUG
                if(msg.cls == NEO6M_CLS_ACK) {
                    printf("received ack: for cls: %X, id: %X\n", msg.playload[0], msg.playload[1]);
                }

                pthread_mutex_lock(&queueMutex);
                if(!waitQueue.empty()) {
                    struct CondWaitUBX *wait = waitQueue.front();
                    if((wait->msg->cls == msg.cls && wait->msg->id == msg.id) || 
                      (msg.cls == NEO6M_CLS_ACK && msg.playload[0] == wait->msg->cls && msg.playload[1] == wait->msg->id)) {
                        if(msg.cls == NEO6M_CLS_ACK && msg.id == NEO6M_ACK_ACK) {
                            wait->valid &= valid;
                            waitQueue.pop_front();
                            delete msg.playload;
                            pthread_cond_signal(&wait->cond);
                        } else {
                            memcpy(wait->msg, &msg, sizeof(msg));
                            wait->valid = valid;
                            if(msg.cls != NEO6M_CLS_CFG) {
                                waitQueue.pop_front();
                                pthread_cond_signal(&wait->cond);
                            } //else wait for the ack
                        }
                        pthread_mutex_unlock(&queueMutex);
                        continue;
                    }
                }
                pthread_mutex_unlock(&queueMutex);
                
                //call the appropriate callback
                if(msg.cls == NEO6M_CLS_ACK)
                    listener.onACKMessage(msg, valid);
                else
                    listener.onUBXMessage(msg, valid);
                delete msg.playload;
            }
        } else {
            usleep(AVAILABLE_DELAY);
        }
    }
}

void Neo6MThread::requestUBXMsg(uint8_t cls, uint8_t id) {
    struct UBXMsg msg;
    msg.cls = cls;
    msg.id = id,
    msg.length = 0;
    msg.playload = NULL;
    sendUBXMsg(msg);
}

struct UBXMsg *Neo6MThread::requestAndWaitUBXMsg(uint8_t cls, uint8_t id) {
    struct UBXMsg *msg = new struct UBXMsg;
    msg->cls = cls;
    msg->id = id,
    msg->length = 0;
    msg->playload = NULL;
    return sendAndWaitUBXMsg(*msg);
}

void Neo6MThread::sendUBXMsg(const struct UBXMsg &msg) {
    uint16_t sync = UBX_SYNC; //because of little endian the order it will be sent will be swapped
    uint16_t checksum = calcUBXChecksum(msg);
    pthread_mutex_lock(&sendMutex);
    writeAll(&sync, 2);
    writeAll(&msg.cls, 1);
    writeAll(&msg.id, 1);
    writeAll(&msg.length, 2);
    writeAll(msg.playload, msg.length);
    writeAll(&checksum, 2);
    pthread_mutex_unlock(&sendMutex);
    printf("send cls: %X, id: %X\n", msg.cls, msg.id); //DEBUG
}

#define WAIT_TIMEOUT_FACTOR 5

struct UBXMsg *Neo6MThread::sendAndWaitUBXMsg(struct UBXMsg &msg) {
    bool request = (msg.length == 0);
    if(request || msg.cls == NEO6M_CLS_CFG) {
        struct CondWaitUBX wait;
        pthread_cond_init(&wait.cond, NULL);
        wait.msg = &msg;
        wait.valid = true;

        pthread_mutex_lock(&queueMutex);
        sendUBXMsg(msg);
        waitQueue.push_back(&wait);

        struct timespec ts;
        clock_gettime(CLOCK_REALTIME, &ts);
        ts.tv_sec += measurementRate * WAIT_TIMEOUT_FACTOR / 1000;
        ts.tv_nsec += measurementRate * WAIT_TIMEOUT_FACTOR % 1000 * 1000 * 1000;
        int ret = pthread_cond_timedwait(&wait.cond, &queueMutex, &ts);
        if(ret) {
            if(ret == ETIMEDOUT) {
                for(std::deque<struct CondWaitUBX *>::iterator it = waitQueue.begin(); it != waitQueue.end(); it++) {
                    if(*it == &wait) {
                        waitQueue.erase(it);
                        break;
                    }
                }
                pthread_mutex_unlock(&queueMutex);
                pthread_cond_destroy(&wait.cond);
                throw Neo6MException("neo6m: wait for answer timed out.");
            }
        }

        pthread_mutex_unlock(&queueMutex);
        pthread_cond_destroy(&wait.cond);
        
        //handle the errors
        if(!wait.valid) {
            delete msg.playload;
            throw Neo6MException("neo6m: received answer is invalid, checksums don't match.");
        } 
        if(msg.cls == NEO6M_CLS_ACK) { //msg.id == NEO6M_ACK_NAK
            const char *format = "neo6m: CFG message with id %.2X and length %d was answered wit NAK.";
            char str[strlen(format) + 1 + 2 + 5]; //message + \0 + 2 digits + max. 5 digts
            snprintf(str, sizeof(str), format, msg.playload[1], msg.length); 
            delete msg.playload;
            throw Neo6MException(str);
        }

        if(request)
            return &msg;
        else
            return NULL;
    } else {
        sendUBXMsg(msg);
        return NULL;
    }
}

uint16_t Neo6MThread::calcUBXChecksum(const struct UBXMsg &msg) {
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

void Neo6MThread::reset() {
    uint8_t playload[] = {0, 0, 1, 0};
    struct UBXMsg msg;
    msg.cls = NEO6M_CLS_CFG;
    msg.id = NEO6M_CFG_RST;
    msg.length = sizeof(playload);
    msg.playload = playload;
    sendAndWaitUBXMsg(msg); //performs a complete software reset, followed by a hotstart
}

//duration of powersave mode in ms, 0 means infinit duration
void Neo6MThread::powerOff(uint32_t duration) {
    uint32_t playload[] = {duration, 0x02};
    struct UBXMsg msg;
    msg.cls = NEO6M_CLS_RXM;
    msg.id = NEO6M_RXM_PMREQ;
    msg.length = sizeof(playload);
    msg.playload = (uint8_t *) playload;
    sendAndWaitUBXMsg(msg);
}

void Neo6MThread::powerOn() {
    uint8_t sequence = 0xFF;
    writeAll(&sequence, 1);
    usleep(500 * 1000);
}

void Neo6MThread::setMessageRate(uint8_t cls, uint8_t id, uint8_t rate) {
    uint8_t playload[] = {cls, id, rate};
    struct UBXMsg msg;
    msg.cls = NEO6M_CLS_CFG;
    msg.id = NEO6M_CFG_MSG;
    msg.length = sizeof(playload);
    msg.playload = playload;
    sendAndWaitUBXMsg(msg);
}

void Neo6MThread::setSBAS(bool enabled) {
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
    sendAndWaitUBXMsg(msg);
}

void Neo6MThread::clearConfiguration() {
    uint32_t playload[3] = {0x0000061F, 0, 0};
    struct UBXMsg msg;
    msg.cls = NEO6M_CLS_CFG;
    msg.id = NEO6M_CFG_CFG;
    msg.length = sizeof(playload);
    msg.playload = (uint8_t *) playload;
    sendAndWaitUBXMsg(msg);
}

void Neo6MThread::saveConfiguration() {
    uint32_t playload[3] = {0, 0x0000061F, 0};
    struct UBXMsg msg;
    msg.cls = NEO6M_CLS_CFG;
    msg.id = NEO6M_CFG_CFG;
    msg.length = sizeof(playload);
    msg.playload = (uint8_t *) playload;
    sendAndWaitUBXMsg(msg);
}

void Neo6MThread::loadConfiguration() {
    uint32_t playload[3] = {0, 0, 0x0000061F};
    struct UBXMsg msg;
    msg.cls = NEO6M_CLS_CFG;
    msg.id = NEO6M_CFG_CFG;
    msg.length = sizeof(playload);
    msg.playload = (uint8_t *) playload;
    sendAndWaitUBXMsg(msg);
}

void Neo6MThread::setProtocol(bool nmea_enabled) {
    struct UBXMsg *msg = requestAndWaitUBXMsg(NEO6M_CLS_CFG, NEO6M_CFG_PRT); //get current configuration
    msg->playload[12] = 0x01 | nmea_enabled << 1;
    msg->playload[14] = 0x01 | nmea_enabled << 1;
    sendAndWaitUBXMsg(*msg); //set configuration
    delete msg->playload;
    delete msg;
}


void Neo6MThread::disableNMEAMessage(const std::string &msg) {
    if(msg.length() != 3)
        throw std::length_error("NMEA message identifier must contain 3 characters");
    char buffer[24]; //don't forget \0
    if(snprintf(buffer, sizeof(buffer), "PUBX,40,%s,0,0,0,0,0,0", msg.c_str()) != (int) (sizeof(buffer) - 1))
        throw std::runtime_error("creating NMEA message: " + std::string(strerror(errno)));
    sendNMEAMessage(buffer);
}

void Neo6MThread::enableNMEAMessage(const std::string &msg) {
    if(msg.length() != 3)
        throw std::length_error("NMEA message identifier must contain 3 characters");
    char buffer[24]; //don't forget \0
    if(snprintf(buffer, sizeof(buffer), "PUBX,40,%s,0,1,0,0,0,0", msg.c_str()) != (int) (sizeof(buffer) - 1))
        throw std::runtime_error("creating NMEA message: " + std::string(strerror(errno)));
    sendNMEAMessage(buffer);
}

void Neo6MThread::sendNMEAMessage(const std::string &msg) {
    char buffer[msg.length() + 7]; //don't forget \0
    uint8_t checksum = calcNMEAChecksum(msg);
    if(snprintf(buffer, sizeof(buffer), "$%s*%.2X\r\n", msg.c_str(), checksum) != (int) (sizeof(buffer) - 1))
        throw std::runtime_error("creating NMEA message: " + std::string(strerror(errno)));
    writeAll(buffer, sizeof(buffer)-1);
}

uint8_t Neo6MThread::calcNMEAChecksum(const std::string &msg) {
    uint8_t sum = 0;
    for(unsigned int i = 0; i < msg.length(); i++) {
        sum ^= msg[i];
    }
    return sum;
}
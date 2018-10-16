#include <unistd.h>

#include "neo6m_thread.h"  

Neo6MThread::Neo6MThread(Neo6MThreadListener &listener, const std::string &serialport, int baudrate) : 
    Neo6M(serialport, baudrate), listener(listener), running(true) {
        setProtocol(UBX_AND_NMEA);
        setSBAS(true);
        disableNMEAMessage("GSV");
        disableNMEAMessage("GSA");
        disableNMEAMessage("GLL");
        disableNMEAMessage("VTG");
        disableNMEAMessage("GGA");
        disableNMEAMessage("RMC");
        usleep(250 * 1000); //sleep 250ms, it's necessary
        flush();
}

Neo6MThread::~Neo6MThread() {

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
                uint8_t checksum = ascii2hex((*str)[len - 2]) << 4 | ascii2hex((*str)[len - 1]);
                bool valid = (checksum == calcNMEAChecksum(str.substr(1, len - 4)));
                //read the ending
                str.push_back(getChar()); // \r
                str.push_back(getChar()); // \n
                //call the callback
                listener.onNMEAMessage(str, valid);
            } else { //UBX
                //read the message
                struct UBXMsg msg;
                uint16_t checksum; //because of little endian the order will be swapped
                readAll(&msg.cls, 1);
                readAll(&msg.id, 1);
                readAll(&msg.length, 2);
                uint8_t playload[msg.length];
                msg.playload = &playload[0];
                readAll(msg.playload, msg.length);
                readAll(&checksum, 2);
                //compare the checksum
                bool valid = (checksum == calcUBXChecksum(msg));
                //check the class and call the callback
                if(msg.cls == NEO6M_CLS_ACK)
                    listener.onACKMessage(msg, valid);
                else
                    listener.onUBXMessage(msg, valid);
            }
        } else {
            usleep(AVAILABLE_DELAY);
        }
    }
}

void Neo6MThread::stopRunning() {
    running = false;
}

void Neo6MThread::requestUBXMsg(uint8_t cls, uint8_t id) {
    struct UBXMsg msg;
    msg.cls = cls;
    msg.id = id;
    msg.length = 0;
    msg.playload = NULL;
    sendUBXStructure(msg);
}

void Neo6MThread::sendUBXMsg(const struct UBXMsg &msg) {
    sendUBXStructure(msg);
}

void Neo6MThread::sendNMEAMsg(const std::string &nmea) {
    sendNMEAMessage(nmea);
}

//copied from Neo6M class
void Neo6MThread::setMessageRate(uint8_t cls, uint8_t id, uint8_t rate) {
    uint8_t playload[] = {cls, id, rate};
    struct UBXMsg msg;
    msg.cls = NEO6M_CLS_CFG;
    msg.id = NEO6M_CFG_MSG;
    msg.length = sizeof(playload);
    msg.playload = playload;
    sendUBXMsg(msg);
}
#ifndef NEO6M_H
#define NEO6M_H 1

#include <string>
//#include "TinyGPSPlus/TinyGPS++.h" DEBUG

#define DEFAULT_BAUDRATE 9600

class Neo6M {
protected:
    /*
     * file descriptor to the device file representing the serial interface
     */
    int fd; 
    /*
     * object to get the information from the NMEA0183 string
     */
    //TinyGPSPlus gps; DEBUG
public:
    Neo6M(const std::string &serialport);
    ~Neo6M();
    
    int getFd(); //DEBUG
    void wakeUp();
    void flushSerial();

    //UBX protocol based methods
    void pollUBXMessage(uint8_t cls, uint8_t id);
    void sendUBXMessage(uint8_t cls, uint8_t id, uint16_t length, uint8_t *playload);
    uint16_t calcUBXChecksum(uint8_t cls, uint8_t id, uint16_t length, uint8_t *msg);
    void reset();
    void setInactiveState();

    //NMEA protocol based methods
    void disableNMEAMessage(const std::string &nmea);
    void enableNMEAMessage(const std::string &nmea);
    void sendNMEAMessage(const std::string &msg);
    uint8_t calcNMEAChecksum(const std::string &msg);

    void writeAll(const void *buf, size_t count);
};

#endif /* NEO6M_H */
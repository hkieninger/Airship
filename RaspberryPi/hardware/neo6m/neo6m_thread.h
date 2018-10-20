#ifndef NEO6M_THREAD_H
#define NEO6M_THREAD_H 1

#include <deque>

#include "neo6m.h"
#include "../../thread/thread.h"

class Neo6MThreadListener {
public:
    /*
     * Callback for NMEA Messages
     * @nmea: the serial message including the $, *, checksum and \r\n\0
     * @valid: false if the checksum mismatches, else true
     */
    virtual void onNMEAMessage(std::string &nmea, bool valid) = 0;

    /*
     * Callback for UBX Messages
     * @valid: false if the checksum mismatches, else true
     */
    virtual void onUBXMessage(struct UBXMsg &msg, bool valid) = 0;

    /*
     * Callback for ACK and nak messages following when a CFG message was sent
     * when requesting a CFG message the neo6m will first send the answer and then the ACK or only a nak
     * @valid: false if the checksum mismatches, else true
     */
    virtual void onACKMessage(struct UBXMsg &ack, bool valid) = 0;

};

struct CondWaitUBX {
    pthread_cond_t cond;
    struct UBXMsg *msg;
    bool valid;
};

/* 
 * this class isn't a subclass of the Neo6M because
 * problem with inheriting Neo6M, destructor of Neo6M called after own destructor would cause SIGSEV
 * the class is more a other version, which has not the inconvinience to wait one cycle per answer
 */
class Neo6MThread: protected UARTDev, public Thread {
    Neo6MThreadListener &listener;
    bool running;

    /*
     * queue containing the messages 
     */
    std::deque<struct CondWaitUBX *> waitQueue;
    pthread_mutex_t queueMutex;

    pthread_mutex_t sendMutex;

    uint16_t measurementRate; //millis

public:
    /*
     * opens the serial device file indicated by serialport in raw mode
     * powers the neo6m on if it was powered off
     * clears the configuration of the neo6m
     * turn NMEA and UBX on
     * sets the dynamic platform model to pedestrian
     * enables SBAS
     * sets all message rates to zero, but you will still receive $GPTXT messages
     * @listener: the listener defines the callbacks for the requests and for message reveived periodically
     * you have to start the thread
     */
    Neo6MThread(Neo6MThreadListener &listener, const std::string &serialport = DEFAULT_SERIALPORT, int baudrate = DEFAULT_BAUDRATE);
    
    /*
     * stops the thread, if it hasn't stopped yet
     * clears the configuration of the neo6m
     * powers off the neo6m
     * closes the serial device file
     */
    virtual ~Neo6MThread();

    /*
     * run method of the thread
     */
    virtual void run();

    /*
     * causes the thread to stop
     */
    void stopRunning();

    /*
     * ================================================================================
     * UBX based methods
     * ================================================================================
     */

    /*
     * request a UBX message of @cls and @id
     * the listener will be notified of the answer
     */
    void requestUBXMsg(uint8_t cls, uint8_t id);

    /*
     * request a UBX message of @cls and @id and wait for the answer
     * calls internally sendAndWaitUBXMsg()
     * @msg.playload: will be a new(ly) allocated buffer filled with the data you requested, you have to delete it afterwards
     * @msg.length: will be be set to the amount of bytes the data received consist of and corresponds to the size of @msg.playload
     * @return: a new(ly) allocated UBXMsg struct you have to delete when you are done with it
     */
    struct UBXMsg *requestAndWaitUBXMsg(uint8_t cls, uint8_t id);

    /*
     * sends a UBX message
     * if @msg.cls is NEO6M_CLS_CFG, the ACK or NAK answer will be sent to the listener
     * if the message corresponds to a request @msg.length equals 0, the answer will be sent to the listener
     */
    void sendUBXMsg(const struct UBXMsg &msg);

    /*
     * sends a UBX message and waits for the answer (if there is one)
     * if @msg.cls is NEO6M_CLS_CFG, the method will wait for the ACK or NAK
     * if the answer is NAK a Neo6MException will be thrown
     * if there is an error in the transmission a Neo6MException will be thrown
     * (if an exception is thrown and the next message in the waitQueue is an identical CFG message, the behaviour is undefined)
     * if @msg.length is 0, the message is a request and the method will wait for the answer
     *     @msg.playload: will be a new(ly) allocated buffer filled with the data you requested, you have to delete it afterwards
     *     @msg.length: will be be set to the amount of bytes the data received consist of and corresponds to the size of @msg.playload
     * @return: NULL if there is no answer other than ACK or NAK, else a pointer to @msg with the updated @msg.playload and @msg.length fields
     */
    struct UBXMsg *sendAndWaitUBXMsg(struct UBXMsg &msg);

    /*
     * calculates the checksum of the passed UBXMsg
     */
    uint16_t calcUBXChecksum(const struct UBXMsg &msg);
    
    /*
     * performs a complete software reset followed by a hotstart
     */
    void reset();

    /*
     * powers the neo6m off, when powered off the neo6m needs ~5mA, it can be powered on again with powerOn()
     * when powered off the configuration gets lost, therefore you might want to save it with saveConfiguration()
     * @duration: the time in milliseconds the neo6m is powered off until it reboots (or reboot it earlier with powerOn())
     *            if it is 0 it is powered off for an infinit time
     */
    void powerOff(uint32_t duration);

    /*
     * powers the neo6m on by sending 0xFF, when powered on the neo6m needs ~50mA in maximum performance mode
     * if the neo6m is already powered on, the 0xFF sequence gets ignored
     */
    void powerOn();

    /*
     * makes that a UBX message is sent in a regular rate
     * @cls: the class of the message (MON, NAV or RXM)
     * @id: the id of the message
     * @rate: the rate of the message per cycle (per default a cycle is 1 second could be configured with CFG-RATE)
     */
    void setMessageRate(uint8_t cls, uint8_t id, uint8_t rate);

    /*
     * enables or disables the use of SBAS, SBAS allows to increase the accuracy from ~10m to ~2m
     */
    void setSBAS(bool enabled);

    /*
     * the current configuration is stored in volatile memory
     * thus it will get lost on a powercycle or on reset
     * the current configuration can be saved in the permanent memory (non-volatile) with saveConfiguration()
     * to load the permanent configuration into the current use loadConfiguration()
     * to convert the permanent configuration back to default (factory) values use clearConfiguration()
     */
    void clearConfiguration();
    void saveConfiguration();
    void loadConfiguration();

    /*
     * enables or disables the NMEA protocol
     */
    void setProtocol(bool nmea_enabled);

    /*
     * ===============================================================================
     * NMEA based methods
     * ===============================================================================
     */

    /*
     * disables a NMEA message
     * @nmea: the NMEA message to be disabled e.g RMC, GGA, VTG, GLL, GSA
     */
    void disableNMEAMessage(const std::string &nmea);

    /*
     * enables a NMEA message
     * @nmea: the NMEA message to be enabled e.g. RMC, GGA, VTG, GLL, GSA
     */
    void enableNMEAMessage(const std::string &nmea);

    /*
     * sends a NMEA message
     * @msg: the NMEA message to be sent, functions adds the leading $, * and checksum to the message
     */
    void sendNMEAMessage(const std::string &msg);

    /*
     * calculates the checksum of the passed nmea message, msg shouldn't contain the leading $
     */
    uint8_t calcNMEAChecksum(const std::string &msg);

};

#endif /* NEO6M_THREAD_H */
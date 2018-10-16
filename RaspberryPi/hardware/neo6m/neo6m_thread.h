#ifndef NEO6M_THREAD_H
#define NEO6M_THREAD_H 1

#include "neo6m.h"
#include "../../thread/thread.h"

/*
 * there is still a lot of improvement, which can be done for the class
 */

class Neo6MThreadListener {
public:
    /*
     * Callback for NMEA Messages
     * @valid: false if the checksum mismatches, else true
     */
    virtual void onNMEAMessage(std::string &nmea, bool valid) = 0;

    /*
     * Callback for UBX Messages
     * @valid: false if the checksum mismatches, else true
     */
    virtual void onUBXMessage(struct UBXMsg &msg, bool valid) = 0;

    /*
     * Callback for ack and nak messages following when a CFG message was sent
     * when requesting a CFG message the neo6m will first send the answer and then the ack or only a nak
     * @valid: false if the checksum mismatches, else true
     */
    virtual void onACKMessage(struct UBXMsg &ack, bool valid) = 0;

};

class Neo6MThread: private Neo6M, public Thread {
    Neo6MThreadListener &listener;
    bool running;
public:
    /*
     * see constructor of Neo6M
     * but NMEA is enabled, however nmea messages are switched off
     * furthermore sbas is enabled by default
     * @listener: the listener implementing the callbacks
     */
    Neo6MThread(Neo6MThreadListener &listener, const std::string &serialport = DEFAULT_SERIALPORT, int baudrate = DEFAULT_BAUDRATE);
    
    /*
     * see destructor of Neo6M
     */
    virtual ~Neo6MThread();

    virtual void run();

    /*
     * stops the thread
     */
    void stopRunning();

    /*
     * request a UBX message of @cls and @id
     * the listeners will be notified via the Callbacks
     */
    void requestUBXMsg(uint8_t cls, uint8_t id);

    /*
     * send a UBX message
     * see Neo6M::sendUBXStructure()
     */
    void sendUBXMsg(const struct UBXMsg &msg);

    /*
     * send a NMEA message
     * see Neo6M::sendNMEAMessage()
     */
    void sendNMEAMsg(const std::string &nmea);

    /*
     * makes that a UBX message is sent in a regular rate
     * @cls: the class of the message (MON, NAV or RXM)
     * @id: the id of the message
     * @rate: the rate of the message per cycle (per default a cycle is 1 second could be configured with CFG-RATE)
     */
    void setMessageRate(uint8_t cls, uint8_t id, uint8_t rate)

};

#endif /* NEO6M_THREAD_H */
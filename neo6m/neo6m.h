#ifndef NEO6M_H
#define NEO6M_H 1

#include <stdint.h>
#include <string>

#include "../gpio/uart_dev.h"

/*
 * possible improvements:
 * -make use of the different power modes
 * -allow to configure serial interface e.g baudrate
 * -several other features -> datasheet
 */

/*
 * default parameteres for the serial asynchronuous interface (uart)
 */
#define DEFAULT_BAUDRATE 9600
#define DEFAULT_SERIALPORT "/dev/serial0"

/*
 * char sequence for synchronization of the ubx protocol
 */
#define UBX_SYNC 0x62B5 //because of little endian the order will be swapped


/*
 * used ubx classes
 */
#define NEO6M_CLS_CFG 0x06
#define NEO6M_CLS_NAV 0x01
#define NEO6M_CLS_ACK 0x05
#define NEO6M_CLS_RXM 0x02

/*
 * used ubx ids
 */
#define NEO6M_CFG_CFG 0x09 /* save, load, clear configuration configuration */
#define NEO6M_CFG_NAV5 0x24 /* for dynamic platform model configuration */
#define NEO6M_CFG_NAVX5 0x23 /* for PPP configuration */
#define NEO6M_CFG_SBAS 0x16 /* SBAS configuration */
#define NEO6M_CFG_MSG 0x01 /* message rate */
#define NEO6M_CFG_RST 0x04  /* reset */
#define NEO6M_CFG_PRT 0x00 /* port and protocols */

#define NEO6M_NAV_POSECEF 0x01 /* position in coordinate system with earth mass center as (0, 0, 0) */
#define NEO6M_NAV_POSLLH 0x02 /* position in system with longitude and latitude (geodetic) */
#define NEO6M_NAV_VELECEF 0x11 /* velocity for the ecef solution */
#define NEO6M_NAV_VELNED 0x12 /* velocity for the geodetic solution */
#define NEO6M_NAV_STATUS 0x03 /* information about gps fix type (dead reckoning and dgps) */
#define NEO6M_NAV_SVINFO 0x30 /* information about use satelites */

#define NEO6M_ACK_ACK 0x01 /* acknowledge */
#define NEO6M_ACK_NAK 0x00 /* not acknowledge */

#define NEO6M_RXM_PMREQ 0x41 /* power off device */

/*
 * constants for gpsFix field in structure GPSFixStatus
 */
#define NEO6M_FIX_NO_FIX 0
#define NEO6M_FIX_DEAD_RECKONING_ONLY 1
#define NEO6M_FIX_2D 2
#define NEO6M_FIX_3D 3
#define NEO6M_FIX_DEAD_RECKONING_GPS 4
#define NEO6M_FIX_TIME_ONLY_FIX 5

/*
 * structure contains the elements of a message of the UBX protocol (NMEA messages are packed as strings)
 */
struct UBXMsg {
    uint8_t cls;
    uint8_t id;
    uint16_t length;
    uint8_t *playload;
};

/*
 * structure contains information about the GPS fix
 */
struct GPSFixStatus {
    uint8_t gpsFix; //fix type see constants defined above
    uint8_t satellites; //amount of satellites used
    bool dgps; //sbas used
};

/*
 * structure contains position in ecef with timestamp
 */
struct __attribute__((__packed__)) GPSEcefPos { //avoid alignment reduces performance, but allows easier implementation
    uint32_t time; //gps millisecond time of week
    float x, y, z; //position in cm
    uint32_t accuracy; //estimated position accuracy in cm
};

/*
 * structure contains velocity in ecef with timestamp
 */
struct __attribute__((__packed__)) GPSEcefVel { //avoid alignment reduces performance, but allows easier implementation
    uint32_t time; //gps millisecond time of week
    float x, y, z; //velocity in cm/s
    uint32_t accuracy; //estimated velocity accuracy in cm/s
};

/*
 * structure contains geodetic position with timestamp
 */
struct __attribute__((__packed__)) GPSGeodeticPos { //avoid alignment reduces performance, but allows easier implementation
    uint32_t time; //gps milliseconds of week
    float lon, lat; //longitude, latitude in dergree
    float hae, has; //height above ellipsoid, height above sea level in mm
    uint32_t haccuracy, vaccuracy; //estimated horizontal accuracy, estimated vertical accuracy in mm
};

/*
 * structure contains velocity in the geodetic system with timestamp
 */
struct __attribute__((__packed__)) GPSGeodeticVel { //avoid alignment reduces performance, but allows easier implementation
    uint32_t time; //gps milliseconds of week
    float north, east, down; //velocity in the specified directions in cm/s
    uint32_t speed, gspeed; //velocity 3d, velocity over ground (2d) in cm/s
    float heading; //heading of motion in degree scaled with 10^-5
    uint32_t saccuracy, haccuracy; //speed accuracy estimate in cm/s, heading accuracy estimate in degree scaled with 10^-5
};

/*
 * represents a Neo6M GPS module, only basic and required functionallities are implemented
 * NMEA and UBX protocol shouldn't be mixed for this library to work properly
 * so switch off NMEA if you use UBX methods or set the message rate of all UBX messages to 0 when using NMEA methods
 * NMEA based methods contain the word NMEA in their name
 */
class Neo6M: public UARTDev {
public:
    /*
     * opens the serial device file indicated by serialport in raw mode
     * powers the neo6m on if it was powered off
     * clears the configuration of the neo6m
     * turns off the NMEA protocol
     * sets the dynamic platform model to pedestrian
     */
    Neo6M(const std::string &serialport = DEFAULT_SERIALPORT);

    /*
     * clears the configuration of the neo6m
     * powers off the neo6m
     * closes the serial device file
     */
    ~Neo6M();


    
    /*
     * ================================================================================
     * UBX based methods
     * ================================================================================
     */

    /*
     * reads a UBX message from the serial device
     * @msg: structure to write the received message in
     * @return: msg
     */
    struct UBXMsg &receiveUBXMessage(struct UBXMsg &msg);

    /*
     * polls a UBX message and reads the answer
     * takes care of the ack or nack for messages from the CFG class
     * @UBXMsg: cls and id fields must be filled out, function completes the structure
     * @return: the completed UBXMsg structure
     */
    struct UBXMsg &pollUBXMessage(struct UBXMsg &msg);

    /*
     * sends the UBXMsg structure
     * takes care of the ack or nack for messages from the CFG class
     * and length unequal to 0 (polling messages)
     */
    void sendUBXMessage(const struct UBXMsg &msg);

    /*
     * calculates the checksum of the passed UBXMsg
     */
    uint16_t calcUBXChecksum(struct UBXMsg *msg);
    
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
     * methods fill out the passed structure by polling the neo6m
     * @return: the filled out structure
     */
    struct GPSFixStatus *getGPSFixStatus(struct GPSFixStatus *status);
    struct GPSEcefPos *getEcefPos(struct GPSEcefPos *pos);
    struct GPSEcefVel *getEcefVel(struct GPSEcefVel *vel);
    struct GPSGeodeticPos *getGeodeticPos(struct GPSGeodeticPos *pos);
    struct GPSGeodeticVel *getGPSGeodeticVel(struct GPSGeodeticVel *vel);

    /*
     * enables or disables the use of SBAS, SBAS allows to increase the accuracy from ~10m to ~2m
     */
    void setSBAS(bool enabled);

    /*
     * enables or disables the use of PPP, PPP allows to reach an accuracy of less than 1m
     * PPP requires SBAS and therefore SBAS will get enabled too
     * if PPP gets disabled, SBAS is kept enabled
     */
    void setPPP(bool enabled);

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
     * reads a NMEA message from the serial device
     * @return: the serial message including the $, *, checksum and \r\n\0
     */
    std::string readNMEAMessage();

    /*
     * calculates the checksum of the passed nmea message, msg shouldn't contain the leading $
     */
    uint8_t calcNMEAChecksum(const std::string &msg);
};

#endif /* NEO6M_H */
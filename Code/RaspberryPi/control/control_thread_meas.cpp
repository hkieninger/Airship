#include <byteswap.h>
#include <stdint.h>
#include <errno.h>
#include <string.h>
#include <sys/time.h>
#include <stdexcept>
#include <math.h>
#include <stdio.h>

#include "makros.h"
#include "control_thread.h"

static uint64_t micros() {
    struct timeval tv;
    if(gettimeofday(&tv, NULL) < 0)
        throw std::runtime_error("get time of day: " + std::string(strerror(errno)));
    return 1000 * 1000 * tv.tv_sec + tv.tv_usec;
}

static double bswap_double(double d) {
    uint64_t i = *((uint64_t *) &d);
    i = bswap_64(i);
    return *((double *) &i);
}

static float bswap_float(float f) {
    uint32_t i = *((uint32_t *) &f);
    i = bswap_32(i);
    return *((float *) &i);
}

static int8_t voltage2percentage(float voltage) {
    //calculate the voltage of the battery
    voltage = voltage * (67.3f + 224) / 67.3f; //firt resistor has 224 kOhm, second resistor has 67.3 kOhm
    
    /*
    //calculate the voltage for one cell
    voltage /= 2;
    //use a interpolation function to calculate the percentage from the voltage of a cell
    float function = 34423760.7101f;
    function += -50969927.8613f * voltage;
    voltage *= voltage; //pow 2
    function += +31421708.1782f * voltage;
    voltage *= voltage; //pow 3
    function += -10323913.2151f * voltage;
    voltage *= voltage; //pow 4
    function += +1906811.7969f * voltage;
    voltage *= voltage; //pow 5
    function += -187724.2842f * voltage;
    voltage *= voltage; // pow 6
    function += 7696.5308f * voltage;
    return roundf(function);
    */
    
    return roundf((100 * voltage - 620) / 2.2f); //3.1 V per cell considered as 0%, 4.2V per cell considered as 100%
}

void ControlThread::measureAds() {
    float voltage = ads->getSingleShot();
    int8_t percent = voltage2percentage(voltage);
    struct Paket paket;
    paket.device = Measurement::SENSOR;
    paket.param = Measurement::BATTERY;
    paket.len = 1;
    paket.data = (uint8_t *) &percent;
    connection.sendPaket(paket);
}

void ControlThread::measureMpu() {
    mpu->getTemperature(); //TODO: send temperature
    float accel[3], gyro[3];
	mpu->getAccel(accel);
	mpu->getGyro(gyro);
    //change endianess to network order
    for(int i = 0; i < 3; i++) {
        accel[i] = bswap_float(accel[i]);
        gyro[i] = bswap_float(gyro[i]);
    }
    struct Paket paket;
    paket.device = Measurement::SENSOR;
    paket.len = sizeof(float) * 3;
    //send accel data
    paket.param = Measurement::ACCEL;
    paket.data = (uint8_t *) &accel[0];
    connection.sendPaket(paket);
    //send gyro data
    paket.param = Measurement::GYRO;
    paket.data = (uint8_t *) &gyro[0];
    connection.sendPaket(paket);
}

void ControlThread::measureQmc() {
    float mag[3];
	qmc->getMag(mag);
    //change endianess to network order
    for(int i = 0; i < 3; i++) {
        mag[i] = bswap_float(mag[i]);
    }
    struct Paket paket;
    paket.device = Measurement::SENSOR;
    paket.param = Measurement::COMPASS;
    paket.len = sizeof(float) * 3;
    paket.data = (uint8_t *) &mag[0];
    connection.sendPaket(paket);
}

void ControlThread::measureBmp() {
    bmp->getTemperature(); //temperature should be read for callibration, TODO: send temperature
    double pres = bmp->getPressure();
    pres = bswap_double(pres);
    struct Paket paket;
    paket.device = Measurement::SENSOR;
    paket.param = Measurement::BARO;
    paket.len = sizeof(double);
    paket.data = (uint8_t *) &pres;
    connection.sendPaket(paket);
}

#define ADS_MEASUREMENT_RATE (500 * 1000)
#define MPU_MEASUREMENT_RATE (500 * 1000)
#define BMP_MEASUREMENT_RATE (500 * 1000)
#define QMC_MEASUREMENT_RATE (500 * 1000)
#define GPS_SEND_RATE (1000 * 1000) //corresponds to the default measurement rate

void ControlThread::measureData() {
    uint64_t now = micros();

    if(now - lastAdsMeas > ADS_MEASUREMENT_RATE) {
        lastAdsMeas = now;
        measureAds();
    }
    if(now - lastMpuMeas > MPU_MEASUREMENT_RATE) {
        lastMpuMeas = now;
        measureMpu();
    }
    if(now - lastBmpMeas > BMP_MEASUREMENT_RATE) {
        lastBmpMeas = now;
        measureBmp();
    }
    if(now - lastQmcMeas > QMC_MEASUREMENT_RATE) {
        lastQmcMeas = now;
        measureQmc();
    }
    if(now - lastGPSSend > GPS_SEND_RATE) {
        lastGPSSend = now;
        sendGPS();
    }
}

void ControlThread::sendGPS() {
    if(status.fixValid && status.timeValid) {
        struct Paket paket;
        paket.device = Measurement::SENSOR;
        paket.param = Measurement::GPS;

        //           lat, long alt         haccur, vaccur         vel: north, east, down    heading           satellites
        uint8_t data[sizeof(int32_t) * 3 + sizeof(uint32_t) * 2 + sizeof(int32_t) * 3 +     sizeof(int32_t) + sizeof(uint8_t)];

        int32_t *dataInt32 = (int32_t *) &data[0];
        dataInt32[0] = bswap_32(position.lat);
        dataInt32[1] = bswap_32(position.lon);
        dataInt32[2] = bswap_32(position.has);
        dataInt32[3] = bswap_32(position.haccuracy);
        dataInt32[4] = bswap_32(position.vaccuracy);
        dataInt32[5] = bswap_32(velocity.north);
        dataInt32[6] = bswap_32(velocity.east);
        dataInt32[7] = bswap_32(velocity.down);
        dataInt32[8] = bswap_32(velocity.heading);

        data[sizeof(data)-1] = status.satellites;

        paket.len = sizeof(data); 
        paket.data = &data[0];
        connection.sendPaket(paket);
    }
}

void ControlThread::onNMEAMessage(std::string &nmea, bool valid) {
    if(valid) {
        struct Paket paket;
        paket.device = Measurement::RPI;
        paket.param = Measurement::INFO;
        paket.len = nmea.length() - 1; //+1 for \0, -2 to remove \r\n -> -1
        paket.data = (uint8_t *) nmea.c_str();
        connection.sendPaket(paket);
    }
}

void ControlThread::handleNavMessage(struct UBXMsg &msg) {
    switch(msg.id) {
        case NEO6M_NAV_POSLLH:
            memcpy(&position, msg.playload, sizeof(position));
            break;
        case NEO6M_NAV_VELNED: 
            memcpy(&velocity, msg.playload, sizeof(velocity));
            break;
        case NEO6M_NAV_STATUS:
            status.time = ((uint32_t *) msg.playload)[0];
            status.gpsFix = msg.playload[4];
            status.dgps = msg.playload[5] & 0x02;
            status.fixValid = msg.playload[5] & 0x01;
            status.timeValid = msg.playload[5] & 0x04 && msg.playload[5] & 0x08;
            break;
        case NEO6M_NAV_SVINFO:
            status.satellites = msg.playload[4];
            break;
        default:
            NOT_IMPLEMENTED;
    }
}

void ControlThread::onUBXMessage(struct UBXMsg &msg, bool valid) {
    if(valid) {
        switch(msg.cls) {
            case NEO6M_CLS_NAV:
                handleNavMessage(msg);
                break;
            default: 
                NOT_IMPLEMENTED;
        }
    }
}

void ControlThread::onACKMessage(struct UBXMsg &ack, bool valid) {
    if(valid && ack.id == NEO6M_ACK_NAK) {
        fprintf(stderr, "Neo6M: following configuration failed: class %d, id %d\n", ack.playload[0], ack.playload[1]);
    }
}
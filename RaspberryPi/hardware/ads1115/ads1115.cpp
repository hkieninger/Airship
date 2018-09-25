#include <byteswap.h>
#include <unistd.h>
#include <stdint.h>
#include <stdexcept> 

#include "../gpio/gpio_exception.h"
#include "../gpio/i2c_dev.h"
#include "ads1115.h"

#define SINGLE_SHOT_SLEEP_TIME 1000
#define SINGLE_SHOT_SLEEP_COUNT 1000

Ads1115::Ads1115(int addr) : I2CDev(addr) {
    writeReg16(ADS1115_REG_CONF, 0x8345); //set configuration
}

Ads1115::~Ads1115() {
    setContinuousMode(false);
}

float Ads1115::getSingleShot() {
    writeBitReg16(ADS1115_REG_CONF, 15, 1);
    usleep(25 + 1163); //time to powerup + time for a measurement
    for(int i = 0; i < SINGLE_SHOT_SLEEP_COUNT; i++) {
        bool bit = readBitReg16(ADS1115_REG_CONF, 15);
        if(bit)
            return getVoltage();
        usleep(SINGLE_SHOT_SLEEP_TIME);
    }
    throw I2CException("get single shot timed out");
}

float Ads1115::getSingleShot(int pin) {
    setInputPin(pin);
    return getSingleShot();
}

float Ads1115::getVoltage() {
    int16_t value = readReg16(ADS1115_REG_CONV);
    value = bswap_16(value);
    if(value == 0x7FFF)
        throw std::overflow_error("voltage at input pin exceeds the range of +- 2.048 V");
    if(value == 0x8000)
        throw std::underflow_error("voltage at input pin is below the range of +- 2.048 V");
    return value * 2.048 / (1 << 15); //2^15
}

void Ads1115::setInputPin(int pin) {
    int16_t ret = readReg16(ADS1115_REG_CONF);
    ret = bswap_16(ret); //go from little to big endian
    ret &= ~(7 << 12); //clear bits
    ret |= (4 + (pin & 3)) << 12; //set bits
    ret = bswap_16(ret); //go back from big to little endian
    writeReg16(ADS1115_REG_CONF ,ret);
}

void Ads1115::setContinuousMode(bool on) {
    writeBitReg16(ADS1115_REG_CONF, 8, !on);
}

void Ads1115::setSampleRate(int rate) {
    int16_t ret = readReg16(ADS1115_REG_CONF);
    ret = bswap_16(ret); //go from little to big endian
    ret &= ~(7 << 5); //clear bits
    ret |= (rate & 3) << 5; //set bits
    ret = bswap_16(ret); //go back from big to little endian
    writeReg16(ADS1115_REG_CONF ,ret);
}
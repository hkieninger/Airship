#include <pigpiod_if2.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <byteswap.h>
#include <stdint.h>

#include "i2c_dev.h"
#include "gpio_exception.h"

/*
 * wrapper of i2c from pigpio
 * you may want to extend it with the other i2c functions from pigpio
 */

I2CDev::I2CDev(int addr, int bus) {
	if(GpioDevice::gpioHandle < 0)
        throw GPIOException("GPIO wasn't initialised.");
	handle = i2c_open(GpioDevice::gpioHandle, bus, addr, 0);
	if(handle < 0)
		throw I2CException("opening i2c device: " + std::string(strerror(errno)));
}

I2CDev::~I2CDev() {
	i2c_close(GpioDevice::gpioHandle, handle);
}

void I2CDev::writeBitReg8(int reg, int bit, bool value) {
	uint8_t ret = readReg8(reg);
	ret &= ~(0x01 << bit); //clear bit
	ret |= value << bit; //set bit
	writeReg8(reg, ret);
}

bool I2CDev::readBitReg8(int reg, int bit) {
	return (readReg8(reg) >> bit) & 0x01;
}

void I2CDev::writeBitReg16(int reg, int bit, bool value) {
	uint16_t ret = readReg16(reg);
	ret = bswap_16(ret); //convert from little to big endian
	ret &= ~(0x01 << bit); //clear bit
	ret |= value << bit; //set bit
	ret = bswap_16(ret); //convert back from big endian to little endian
	writeReg16(reg, ret);
}

bool I2CDev::readBitReg16(int reg, int bit) {
	uint16_t ret = readReg16(reg);
	ret = bswap_16(ret);
	return (ret >> bit) & 0x01;
}

void I2CDev::writeReg8(int reg, uint8_t value) {
	if(i2c_write_byte_data(GpioDevice::gpioHandle, handle, reg, value) < 0)
		throw I2CException("writing register 8bit of i2c device: " + std::string(strerror(errno)));
}

uint8_t I2CDev::readReg8(int reg) {
	int ret = i2c_read_byte_data(GpioDevice::gpioHandle, handle, reg);
	if(ret < 0)
		throw I2CException("reading register 8bit of i2c device: " + std::string(strerror(errno)));
	return ret;
}

void I2CDev::writeReg16(int reg, uint16_t value) {
	if(i2c_write_word_data(GpioDevice::gpioHandle, handle, reg, value) < 0)
		throw I2CException("writing register 16bit of i2c device: " + std::string(strerror(errno)));
}

uint16_t I2CDev::readReg16(int reg) {
	int ret = i2c_read_word_data(GpioDevice::gpioHandle, handle, reg);
	if(ret < 0)
		throw I2CException("reading register 16bit of i2c device: " + std::string(strerror(errno)));
	return ret;
}
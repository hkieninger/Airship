#include "i2c_dev.h"
#include "gpio_exception.h"

#include <wiringPiI2C.h>
#include <errno.h>
#include <string.h>
#include <byteswap.h>
#include <unistd.h>

/*
 * implements straight read and write (wrapper function of wiringPi)
 */

I2CDev::I2CDev(int addr) {
	fd = wiringPiI2CSetup(addr);
	if(fd < 0)
		throw I2CException("opening i2c device: " + std::string(strerror(errno)));
}

I2CDev::~I2CDev() {
	close(fd);
}

void I2CDev::writeBitReg8(int reg, int bit, bool value) {
	int ret = readReg8(reg);
	ret &= ~(0x01 << bit); //clear bit
	ret |= value << bit; //set bit
	writeReg8(reg, ret);
}

bool I2CDev::readBitReg8(int reg, int bit) {
	return (readReg8(reg) >> bit) & 0x01;
}

void I2CDev::writeReg8(int reg, int value) {
	if(wiringPiI2CWriteReg8(fd, reg, value) < 0)
		throw I2CException("writing register 8bit of i2c device: " + std::string(strerror(errno)));
}

int I2CDev::readReg8(int reg) {
	int ret = wiringPiI2CReadReg8(fd, reg);
	if(ret < 0)
		throw I2CException("reading register 8bit of i2c device: " + std::string(strerror(errno)));
	return ret;
}

void I2CDev::writeReg16(int reg, int value) {
	value = bswap_16(value); //rpi works with little endian and i2c with big endian
	if(wiringPiI2CWriteReg16(fd, reg, value) < 0)
		throw I2CException("writing register 16bit of i2c device: " + std::string(strerror(errno)));
}

int I2CDev::readReg16(int reg) {
	int ret = wiringPiI2CReadReg16(fd, reg);
	if(ret < 0)
		throw I2CException("reading register 16bit of i2c device: " + std::string(strerror(errno)));
	//printf("read register: %d\n", ret);
	return ret;
	//return bswap_16(ret); //rpi works with little endian and i2c with big endian
}

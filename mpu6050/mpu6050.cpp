#include <unistd.h>
#include <stdint.h>
#include <byteswap.h>
#include <wiringPi.h>
#include <string.h>
#include <errno.h>
#include <string>
#include <functional>
using namespace std::placeholders;

#include "mpu6050.h"
#include "../interfaces/observable.h"
#include "../gpio/i2c_dev.h"
#include "../gpio/gpio_exception.h"

void assembleInterruptHandler(void (Mpu6050::*func)(), Mpu6050 &object) {
	(object.*func())();
}

Mpu6050::Mpu6050(int interruptpin, int addr) : I2CDev(addr) {
	reset();
	if(interruptpin >= 0) {
		if(wiringPiISR(interruptpin, INT_EDGE_RISING, assembleInterruptHandler(&Mpu6050::interruptHandler, this)) < 0)
			throw GPIOException("setup interrupt handler: " + std::string(strerror(errno)));
		setInterrupts(true);
	}
	setCycleFreq(MPU6050_1_25_HZ);
	setCycleMode(true);
	setSleepMode(false);
}

Mpu6050::~Mpu6050() {
	writeReg8(MPU6050_REG_PWR_MGMT1, 0x40); //set sleep bit
}

void Mpu6050::reset() {
	writeReg8(MPU6050_REG_PWR_MGMT1, 0x80);
	usleep(1000); //let mpu6050 complete reset
}

void Mpu6050::setInterrupts(bool on) {
	writeBitReg8(MPU6050_REG_INT_ENABLE, 0, on);
}

void Mpu6050::setSleepMode(bool on) {
	writeBitReg8(MPU6050_REG_PWR_MGMT1, 6, on);
}

void Mpu6050::setCycleMode(bool on) {
	writeBitReg8(MPU6050_REG_PWR_MGMT1, 5, on);
}

void Mpu6050::setCycleFreq(int freq) {
	writeBitReg8(MPU6050_REG_PWR_MGMT2, 7, freq & 0x02);
	writeBitReg8(MPU6050_REG_PWR_MGMT2, 6, freq & 0x01);
}

float Mpu6050::getTemperature() {
	int16_t val = 0xFFFF & readReg16(MPU6050_REG_TEMP);
	val = bswap_16(val);
	return val / 340.0f + 36.53f; //formula from datasheet
}

float Mpu6050::getAccel(int reg) {
	int16_t val = 0xFFFF & readReg16(reg);
	val = bswap_16(val);
	return val / 16384.0f * 9.80665f; //formula from datasheet, only valid for AFS_SEL equal 0
}

float Mpu6050::getGyro(int reg) {
	int16_t val = 0xFFFF & readReg16(reg);
	val = bswap_16(val);
	return val / 131.0f; //formula from datasheet, only valid for FS_SEL equal 0
}

float *Mpu6050::getAccel(float *array) {
	//could be optimized by using automatic address incrementation by mpu6050
	array[0] = getAccelX();
	array[1] = getAccelY();
	array[2] = getAccelZ();
	return array;
}

float Mpu6050::getAccelX() {
	return getAccel(MPU6050_REG_AC_X);
}

float Mpu6050::getAccelY() {
	return getAccel(MPU6050_REG_AC_Y);
}

float Mpu6050::getAccelZ() {
	return getAccel(MPU6050_REG_AC_Z);
}

float *Mpu6050::getGyro(float *array) {
	//could be optimized by using automatic address incrementation by mpu6050
	array[0] = getGyroX();
	array[1] = getGyroY();
	array[2] = getGyroZ();
	return array;
}

float Mpu6050::getGyroX() {
	return getGyro(MPU6050_REG_GY_X);
}

float Mpu6050::getGyroY() {
	return getGyro(MPU6050_REG_GY_Y);
}

float Mpu6050::getGyroZ() {
	return getGyro(MPU6050_REG_GY_Z);
}

void Mpu6050::interruptHandler() {
	if(readBitReg8(MPU6050_REG_INT_STATUS, 0) == 1) {
		getAccel(latestData.accel);
		getGyro(latestData.gyro);
		notifyObservers(&latestData); //update methods shouldn't wait, block or sleep, nor do too much processing
	}
	//else interrupt gets ignored
}

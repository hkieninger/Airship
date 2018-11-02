#include <unistd.h>
#include <stdint.h>
#include <stdexcept>

#include "qmc5883l.h"
#include "../gpio/i2c_dev.h"

Qmc5883l::Qmc5883l() : I2CDev(QMC5883L_I2C_ADDR) {
	reset();
	writeReg8(QMC5883L_REG_CTRL1, 0x01); //runs with 10Hz (continuous mode), range 2 Gauss
	writeReg8(0x0B, 0x01); //set/reset period, value according to recommendation of datasheet
	range = QMC5883L_2_GAUSS;
	usleep(100 * 1000);  //time for first measurement
}

Qmc5883l::~Qmc5883l() {
	writeReg8(QMC5883L_REG_CTRL1, 0x00); //set standby mode to reduce power consumption
}

void Qmc5883l::reset() {
	writeReg8(QMC5883L_REG_CTRL2, 0x80);
	usleep(350);
}

void Qmc5883l::setRange(int range) {
	writeBitReg8(QMC5883L_REG_CTRL1, 4, range);
	this->range = range;
}

bool Qmc5883l::dataReady() {
	return readBitReg8(QMC5883L_REG_STAT, 0);
}

void Qmc5883l::setStandby() {
	writeBitReg8(QMC5883L_REG_CTRL1, 0, 0);
}

void Qmc5883l::setContinuous(int freq) {
	writeBitReg8(QMC5883L_REG_CTRL1, 2, freq & 0x01);
	writeBitReg8(QMC5883L_REG_CTRL1, 3, freq & 0x02);
	writeBitReg8(QMC5883L_REG_CTRL1, 0, 1);
}

float *Qmc5883l::getMag(float *array) {
	//could be improved by using automatic address incrementation of the qmc5883l
	array[0] = getMagX();
	array[1] = getMagY();
	array[2] = getMagZ();
	return array;
}

//max speed 200Hz -> 5ms pause
float Qmc5883l::getMag(int reg) {
	if(readBitReg8(QMC5883L_REG_STAT, 1)) //check overflow flag
		throw std::range_error("magnetic field exceeds range");
	int16_t val = readReg16(reg);
	if(range == QMC5883L_2_GAUSS)
		return val / 12000.0f;
	else
		return val / 3000.0f;
}

float Qmc5883l::getMagX() {
	return getMag(QMC5883L_REG_DX);
}

float Qmc5883l::getMagY() {
	return getMag(QMC5883L_REG_DY);
}

float Qmc5883l::getMagZ() {
	return getMag(QMC5883L_REG_DZ);
}

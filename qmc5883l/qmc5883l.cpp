#include "qmc5883l.h"
#include "../i2c_dev.h"

#include <unistd.h>
#include <stdint.h>

Qmc5883l::Qmc5883l() : I2CDev(QMC5883L_I2C_ADDR) {
	reset();
	writeReg8(QMC5883L_REG_CTRL1, 0x01); //runs with 10Hz, set mode and range
}

Qmc5883l::~Qmc5883l() {
	writeReg8(QMC5883L_REG_CTRL1, 0x00); //set standby mode to reduce power consumption
}

void Qmc5883l::reset() {
	writeReg8(QMC5883L_REG_CTRL2, 0x80);
	usleep(350);
}

//max speed 200Hz -> 5ms pause
float Qmc5883l::getMag(int reg) {
	int16_t val = 0xFFFF & readReg16(reg);
	return val / 12000.0f;
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

#include <stdint.h>
#include <unistd.h>

#include "../gpio/i2c_dev.h"
#include "bmp280.h"

Bmp280::Bmp280(int addr) : I2CDev(addr) {
    reset();
    getCallibParam();
    writeReg8(BMP280_REG_CONFIG, 0x10); //standby 0.5 ms, filter coefficient 16
    writeReg8(BMP280_REG_CTRL_MEAS, 0x57); //oversample temperature 2x, oversample pressure 16x, normal mode
    usleep(43200); //sleep 43.2 ms, maximum time for a measurement, to take a first measurement
}

Bmp280::~Bmp280() {
    setPowerMode(BMP280_SLEEP_MODE);
}

void Bmp280::reset() {
    writeReg8(BMP280_REG_RESET, 0xB6);
    usleep(2000); //let complete reset
}

void Bmp280::setPowerMode(uint8_t mode) {
    uint8_t reg = readReg8(BMP280_REG_CTRL_MEAS);
    reg &= ~3; //clear bits
    reg |= mode & 3; //set bits
    writeReg8(BMP280_REG_CTRL_MEAS, reg);
}

void Bmp280::setTemperatureOversampling(uint8_t oversampling) {
    uint8_t reg = readReg8(BMP280_REG_CTRL_MEAS);
    reg &= ~(7 << 5); //clear bits
    reg |= (oversampling & 7) << 5; //set bits
    writeReg8(BMP280_REG_CTRL_MEAS, reg);
}

void Bmp280::setPressureOversampling(uint8_t oversampling) {
    uint8_t reg = readReg8(BMP280_REG_CTRL_MEAS);
    reg &= ~(7 << 2); //clear bits
    reg |= (oversampling & 7) << 2; //set bits
    writeReg8(BMP280_REG_CTRL_MEAS, reg);
}

void Bmp280::setStandbyTime(uint8_t time) {
    uint8_t reg = readReg8(BMP280_REG_CONFIG);
    reg &= ~(7 << 5); //clear bits
    reg |= (time & 7) << 5; //set bits
    writeReg8(BMP280_REG_CONFIG, reg);
}

void Bmp280::setFilter(uint8_t coefficient) {
    uint8_t reg = readReg8(BMP280_REG_CONFIG);
    reg &= ~(7 << 2); //clear bits
    reg |= (coefficient & 7) << 2; //set bits
    writeReg8(BMP280_REG_CONFIG, reg);
}

double Bmp280::getTemperature() {
    //normally should use burst read to avoid mixing up of bytes from different measurements
    uint32_t msb = readReg8(BMP280_REG_TEMP_MSB);
    uint32_t lsb = readReg8(BMP280_REG_TEMP_LSB);
    uint32_t xlsb = readReg8(BMP280_REG_TEMP_XLSB);
    uint32_t value = msb << 12 | lsb << 4 | xlsb >> 4;

    double var1 = (value / 16384.0 - dig_t1 / 1024.0) * dig_t2;
    double var2 = (value / 131072.0 - dig_t1 / 8192.0) * (value / 131072.0 - dig_t1 / 8192.0) * dig_t3;
	t_fine = (int32_t) (var1 + var2);
	return (var1 + var2) / 5120.0;
}

double Bmp280::getPressure() {
    //normally should use burst read to avoid mixing up of bytes from different measurements
    uint32_t msb = readReg8(BMP280_REG_PRES_MSB);
    uint32_t lsb = readReg8(BMP280_REG_PRES_LSB);
    uint32_t xlsb = readReg8(BMP280_REG_PRES_XLSB);
    uint32_t value = msb << 12 | lsb << 4 | xlsb >> 4;

    double var1 = t_fine / 2.0 - 64000.0;
    double var2 = var1 * var1 * dig_p6 / 32768.0;
    var2 = var2 + var1 * dig_p5 * 2.0;
    var2 = var2 / 4.0 + dig_p4 * 65536.0;
    var1 = (dig_p3 * var1 * var1 / 524288.0 + dig_p2 * var1) / 524288.0;
    var1 = (1.0 + var1 / 32768.0) * dig_p1;
    double pressure = 1048576.0 -  value;
    if (var1 != 0) {
        pressure = (pressure - var2 / 4096.0) * 6250.0 / var1;
        var1 = dig_p9 * pressure * pressure / 2147483648.0;
        var2 = pressure * dig_p8 / 32768.0;
        pressure = pressure + (var1 + var2 + dig_p7) / 16.0;
    } else {
        pressure = 0;
    }
    return pressure;
}

void Bmp280::getCallibParam() {
    dig_t1 = readReg8(BMP280_REG_T1_LSB) | readReg8(BMP280_REG_T1_MSB) << 8;
	dig_t2 = readReg8(BMP280_REG_T2_LSB) | readReg8(BMP280_REG_T2_MSB) << 8;
	dig_t3 = readReg8(BMP280_REG_T3_LSB) | readReg8(BMP280_REG_T3_MSB) << 8;
	dig_p1 = readReg8(BMP280_REG_P1_LSB) | readReg8(BMP280_REG_P1_MSB) << 8;
	dig_p2 = readReg8(BMP280_REG_P2_LSB) | readReg8(BMP280_REG_P2_MSB) << 8;
	dig_p3 = readReg8(BMP280_REG_P3_LSB) | readReg8(BMP280_REG_P3_MSB) << 8;
	dig_p4 = readReg8(BMP280_REG_P4_LSB) | readReg8(BMP280_REG_P4_MSB) << 8;
	dig_p5 = readReg8(BMP280_REG_P5_LSB) | readReg8(BMP280_REG_P5_MSB) << 8;
	dig_p6 = readReg8(BMP280_REG_P6_LSB) | readReg8(BMP280_REG_P6_MSB) << 8;
	dig_p7 = readReg8(BMP280_REG_P7_LSB) | readReg8(BMP280_REG_P7_MSB) << 8;
	dig_p8 = readReg8(BMP280_REG_P8_LSB) | readReg8(BMP280_REG_P8_MSB) << 8;
	dig_p9 = readReg8(BMP280_REG_P9_LSB) | readReg8(BMP280_REG_P9_MSB) << 8;
}
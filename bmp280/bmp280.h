#ifndef BMP280_H
#define BMP280_H 1

#include <stdint.h>

#include "../gpio/i2c_dev.h"

/*
 * possible improvements:
 * -allow the usage of spi
 * -use the status register
 */

/* 
 * i2c address of the bmp280
 * addr0 is when sdo is pulled to GND
 * addr1 is when sdo is connected to VCC
 * to enable i2c csb must be connected to VCC
 */
#define BMP280_I2C_ADDR0 0x76
#define BMP280_I2C_ADDR1 0x77

/* registers */
#define BMP280_REG_TEMP_XLSB 0xFC
#define BMP280_REG_TEMP_LSB 0xFB
#define BMP280_REG_TEMP_MSB 0xFA
#define BMP280_REG_PRES_XLSB 0xF9
#define BMP280_REG_PRES_LSB 0xF8
#define BMP280_REG_PRES_MSB 0xF7
#define BMP280_REG_CONFIG 0xF5
#define BMP280_REG_CTRL_MEAS 0xF4
#define BMP280_REG_STATUS 0xF3
#define BMP280_REG_RESET 0xE0
#define BMP280_REG_ID 0xD0

/* callibration registers */
#define BMP280_REG_T1_LSB 0x88
#define BMP280_REG_T1_MSB 0x89
#define BMP280_REG_T2_LSB 0x8A
#define BMP280_REG_T2_MSB 0x8B
#define BMP280_REG_T3_LSB 0x8C
#define BMP280_REG_T3_MSB 0x8D
#define BMP280_REG_P1_LSB 0x8E
#define BMP280_REG_P1_MSB 0x8F
#define BMP280_REG_P2_LSB 0x90
#define BMP280_REG_P2_MSB 0x91
#define BMP280_REG_P3_LSB 0x92
#define BMP280_REG_P3_MSB 0x93
#define BMP280_REG_P4_LSB 0x94
#define BMP280_REG_P4_MSB 0x95
#define BMP280_REG_P5_LSB 0x96
#define BMP280_REG_P5_MSB 0x97
#define BMP280_REG_P6_LSB 0x98
#define BMP280_REG_P6_MSB 0x99
#define BMP280_REG_P7_LSB 0x9A
#define BMP280_REG_P7_MSB 0x9B
#define BMP280_REG_P8_LSB 0x9C
#define BMP280_REG_P8_MSB 0x9D
#define BMP280_REG_P9_LSB 0x9E
#define BMP280_REG_P9_MSB 0x9F

/*
 * constants for setPowerMode()
 */
#define BMP280_SLEEP_MODE 0x00
#define BMP280_FORCED_MODE 0x01
#define BMP280_NORMAL_MODE 0x03

/*
 * constants for setTemperatureOversampling() and setPressureOversampling()
 */
#define BMP280_MEASUREMENT_OFF 0x00
#define BMP280_OVERSAMPLING_1 0x01
#define BMP280_OVERSAMPLING_2 0x02
#define BMP280_OVERSAMPLING_4 0x03
#define BMP280_OVERSAMPLING_8 0x04
#define BMP280_OVERSAMPLING_16 0x05

/*
 * constants for setStanbyTime()
 */
#define BMP280_0_5_MS 0x00
#define BMP280_62_5_MS 0x01
#define BMP280_125_MS 0x02
#define BMP280_250_MS 0x03
#define BMP280_500_MS 0x04
#define BMP280_1000_MS 0x05
#define BMP280_2000_MS 0x06
#define BMP280_4000_MS 0x07

/* 
 * constants for setFilter()
 */
#define BMP280_FILTER_OFF 0x00
#define BMP280_FILTER_COEFF_2 0x01
#define BMP280_FILTER_COEFF_4 0x02
#define BMP280_FILTER_COEFF_8 0x03
#define BMP280_FILTER_COEFF_16 0x04

/*
 * represents a bmp280
 * an ic which can measure pressure of the air and temperature
 * it is either controlled with i2c or spi (until now the library only supports i2c)
 * the ic works big endian
 */
class Bmp280: public I2CDev {
    /*
     * callibration parameters
     */
    uint16_t dig_t1;
	int16_t dig_t2;
	int16_t dig_t3;
	uint16_t dig_p1;
	int16_t dig_p2;
	int16_t dig_p3;
	int16_t dig_p4;
	int16_t dig_p5;
	int16_t dig_p6;
	int16_t dig_p7;
	int16_t dig_p8;
	int16_t dig_p9;
	int32_t t_fine;

    /*
     * reads the callibration parameters
     */
    void getCallibParam();
public:
    /*
     * enables the filter and sets the bmp280 to ultra high resolution
     * the device is set to normal mode with a standby time of 0.5ms, which result in a measurement rate of ~26.32 Hz
     */
    Bmp280(int addr = BMP280_I2C_ADDR0);

    /*
     * sets the bmp280 to sleep mode
     */
    ~Bmp280();

    /*
     * software reset
     * loads default values into registers
     * after software reset device will be in sleep mode
     */
    void reset();

    /*
     * sets the power mode
     * pass one of the constants defined above
     * after forced mode you should sleep some ms (see datasheet for exact duration) before you readout the result
     */
    void setPowerMode(uint8_t mode);

    /*
     * configures the bmp280
     * pass one of the constants defined above
     * you should put the bmp280 in sleep mode before making the configuration
     */
    void setTemperatureOversampling(uint8_t oversampling);
    void setPressureOversampling(uint8_t oversampling);
    void setStandbyTime(uint8_t time);
    void setFilter(uint8_t coefficient);

    /*
     * returns the temperature in °C
     */
    double getTemperature();

    /*
     * call getTemperature() first to initialise/update the callibration parameter
     * returns the pressure in Pa
     */
    double getPressure();
};

#endif /* BMP280_H */
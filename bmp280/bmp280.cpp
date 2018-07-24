#include <stdint.h>
#include <unistd.h>
#include <stddef.h>

#include "../gpio/gpio_exception.h"
#include "../gpio/i2c_dev.h"
#include "bmp280_api/bmp280_defs.h"
#include "bmp280_api/bmp280.h"
#include "bmp280.h"

Bmp280 *Bmp280::bmp0 = NULL;
Bmp280 *Bmp280::bmp1 = NULL;

int8_t Bmp280::i2cWrite(uint8_t dev_id, uint8_t reg_addr, uint8_t *data, uint16_t len) {
    Bmp280 *bmp;
    if(dev_id == BMP280_I2C_ADDR0)
        bmp = Bmp280::bmp0;
    else
        bmp = Bmp280::bmp1;
    try {
        bmp->writeI2C(&reg_addr, 1);
        bmp->writeI2C(data, len);
    } catch(const I2CException& e) {
        return -1;
    }
    return 0;
}

int8_t Bmp280::i2cRead(uint8_t dev_id, uint8_t reg_addr, uint8_t *data, uint16_t len) {
    Bmp280 *bmp;
    if(dev_id == BMP280_I2C_ADDR0)
        bmp = Bmp280::bmp0;
    else
        bmp = Bmp280::bmp1;
    try {
        bmp->writeI2C(&reg_addr, 1);
        bmp->readI2C(data, len);
    } catch(const I2CException& e) {
        return -1;
    }
    return 0;
}

void Bmp280::delayMs(uint32_t period) {
    usleep(period * 1000);
}

Bmp280::Bmp280(int addr) : I2CDev(addr) {
    if(addr == BMP280_I2C_ADDR0)
        Bmp280::bmp0 = this;
    else
        Bmp280::bmp1 = this;

    /* Sensor interface over I2C with primary slave address  */
    bmp.dev_id = addr;
    bmp.intf = BMP280_I2C_INTF;
    bmp.read = &Bmp280::i2cRead;
    bmp.write = &Bmp280::i2cWrite;
    bmp.delay_ms = &Bmp280::delayMs;

    int8_t rslt = bmp280_init(&bmp); //reset is inklusive
    if (rslt != BMP280_OK)
        throw I2CException("bmp280 initialisation failed");

    configure(BMP280_OS_2X, BMP280_OS_16X, BMP280_ODR_0_5_MS, BMP280_FILTER_COEFF_4);
}

Bmp280::~Bmp280() {
    setPowerMode(BMP280_SLEEP_MODE);
}

void Bmp280::reset() {
    if(bmp280_soft_reset(&bmp) != BMP280_OK)
        throw I2CException("bmp280 reset failed");
}

void Bmp280::setPowerMode(uint8_t mode) {
    int8_t rslt = bmp280_set_power_mode(mode, &bmp);
    if(rslt != BMP280_OK)
        throw I2CException("bmp280 set power mode failed");
}

void Bmp280::configure(uint8_t os_temp, uint8_t os_pres, uint8_t odr, uint8_t filter) {
    uint8_t mode;
    if(bmp280_get_power_mode(&mode, &bmp) != BMP280_OK)
        throw I2CException("bmp280 get power mode failed");

    struct bmp280_config conf;
    /* Always read the current settings before writing, especially when
    * all the configuration is not modified 
    */
    int8_t rslt = bmp280_get_config(&conf, &bmp);
    if(rslt != BMP280_OK)
        throw I2CException("bmp280 get configuration failed");

    conf.filter = filter;
    conf.os_pres = os_pres;
    conf.os_temp = os_temp;
    conf.odr = odr; //look at datasheet section 3.8.2 for resulting freqs

    rslt = bmp280_set_config(&conf, &bmp);
    if(rslt != BMP280_OK)
        throw I2CException("bmp280 set configuration failed");

    usleep(2 * 1000);
    
    /* Always set the power mode after setting the configuration */
    setPowerMode(BMP280_NORMAL_MODE);
}

double *Bmp280::getMeasurement(double *data) {
    struct bmp280_uncomp_data ucomp_data;
    int8_t rslt = bmp280_get_uncomp_data(&ucomp_data, &bmp);
    if(rslt != BMP280_OK)
        throw I2CException("bmp280 get measurement failed");
    data[0] = bmp280_comp_temp_double(ucomp_data.uncomp_temp, &bmp);
    data[1] = bmp280_comp_pres_double(ucomp_data.uncomp_press, &bmp);
    return data;
}

uint8_t Bmp280::calcMeasurementTime() {
    return bmp280_compute_meas_time(&bmp);
}
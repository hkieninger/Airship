/*
 * testing bmp280 directly with the bmp280 API from Bosch
 */

#include "../bmp280/bmp280_api/bmp280_defs.h"
#include "../bmp280/bmp280_api/bmp280.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <wiringPiI2C.h>

int fd;

int8_t i2c_write(uint8_t dev_id, uint8_t reg_addr, uint8_t *data, uint16_t len) {
    for(int i = 0; i < len; i++) {
        if(wiringPiI2CWriteReg8(fd, reg_addr + i, data[i]) < 0)
            return -1;
    }
    return 0;
}

int8_t i2c_read(uint8_t dev_id, uint8_t reg_addr, uint8_t *data, uint16_t len) {
    for(int i = 0; i < len; i++) {
        int reg = wiringPiI2CReadReg8(fd, reg_addr + i);
        if(reg < 0)
            return -1;
        data[i] = (uint8_t) reg;
    }
    return 0;
}

void delay_ms(uint32_t period) {
    usleep(period * 1000);
}

int main() {
    fd = wiringPiI2CSetup(BMP280_I2C_ADDR_PRIM);
    if(fd < 0) {
        perror("wiringPiI2CSetupd");
        return EXIT_FAILURE;
    }
    printf("fd: %d\n", fd);


    /*
     * ========================================================================
     * initialisation
     * ========================================================================
     */
    printf("make initialisation\n");
    int8_t rslt;
    struct bmp280_dev bmp;
    /* Sensor interface over I2C with primary slave address  */
    bmp.dev_id = BMP280_I2C_ADDR_PRIM;
    bmp.intf = BMP280_I2C_INTF;
    bmp.read = &i2c_read;
    bmp.write = &i2c_write;
    bmp.delay_ms = &delay_ms;

    rslt = bmp280_init(&bmp);

    if (rslt == BMP280_OK) {
        /* Sensor chip ID will be printed if initialization was successful */
        printf("Device found with chip id 0x%x\n", bmp.chip_id);
    } else {
        return EXIT_FAILURE;
    }

    /*
     * ========================================================================
     * configuration
     * ========================================================================
     */
    printf("make configuration\n");
    struct bmp280_config conf;

    /* Always read the current settings before writing, especially when
    * all the configuration is not modified 
    */
    rslt = bmp280_get_config(&conf, &bmp);
    /* Check if rslt == BMP280_OK, if not, then handle accordingly */

    /* Overwrite the desired settings */
    conf.filter = BMP280_FILTER_COEFF_2;
    conf.os_pres = BMP280_OS_16X;
    conf.os_temp = BMP280_OS_4X;
    conf.odr = BMP280_ODR_1000_MS;

    rslt = bmp280_set_config(&conf, &bmp);
    /* Check if rslt == BMP280_OK, if not, then handle accordingly */

    /* Always set the power mode after setting the configuration */
    rslt = bmp280_set_power_mode(BMP280_NORMAL_MODE, &bmp);
    /* Check if rslt == BMP280_OK, if not, then handle accordingly */

    /*
     * ========================================================================
     * read out
     * ========================================================================
     */
    printf("make read out\n");
    struct bmp280_uncomp_data ucomp_data;
    uint8_t meas_dur = bmp280_compute_meas_time(&bmp);

    printf("Measurement duration: %dms\r\n", meas_dur);

    /* Loop to read out 10 samples of data */ 
    for (uint8_t i = 0; (i < 10) && (rslt == BMP280_OK); i++) {
        bmp.delay_ms(meas_dur); /* Measurement time */

        rslt = bmp280_get_uncomp_data(&ucomp_data, &bmp);
        /* Check if rslt == BMP280_OK, if not, then handle accordingly */

        int32_t temp32 = bmp280_comp_temp_32bit(ucomp_data.uncomp_temp, &bmp);
        uint32_t pres32 = bmp280_comp_pres_32bit(ucomp_data.uncomp_press, &bmp);
        uint32_t pres64 = bmp280_comp_pres_64bit(ucomp_data.uncomp_press, &bmp);
        double temp = bmp280_comp_temp_double(ucomp_data.uncomp_temp, &bmp);
        double pres = bmp280_comp_pres_double(ucomp_data.uncomp_press, &bmp);

        printf("UT: %d, UP: %d, T32: %d, P32: %d, P64: %d, P64N: %d, T: %f, P: %f\r\n", \
        ucomp_data.uncomp_temp, ucomp_data.uncomp_press, temp32, \
        pres32, pres64, pres64 / 256, temp, pres);

        bmp.delay_ms(1000); /* Sleep time between measurements = BMP280_ODR_1000_MS */
    }


    close(fd);
    return EXIT_SUCCESS;
}
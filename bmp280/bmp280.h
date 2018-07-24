#ifndef BMP280_H
#define BMP280_H 1

#include <stdint.h>

#include "../gpio/i2c_dev.h"
#include "bmp280_api/bmp280_defs.h"
#include "bmp280_api/bmp280.h"

/*
 * possible improvements:
 * -allow the usage of spi
 */

/* 
 * i2c address of the bmp280
 * addr0 is when sdo is pulled to GND
 * addr1 is when sdo is connected to VCC
 * to enable i2c csb must be connected to VCC
 */
#define BMP280_I2C_ADDR0 BMP280_I2C_ADDR_PRIM
#define BMP280_I2C_ADDR1 BMP280_I2C_ADDR_SEC

/*
 * represents a bmp280
 * an ic either controlled with i2c or spi (until now the library only supports i2c)
 * the class uses internally the bmp280 API from Bosch the manufacturer
 */
class Bmp280: public I2CDev {
    /*
     * the bmp280 API from Bosch needs a c function pointer to the i2c read and write functions
     * because it's difficult to get a c function pointer to c++ member function, we realise this with a static function
     */
    static Bmp280 *bmp0, *bmp1;
    static int8_t i2cWrite(uint8_t dev_id, uint8_t reg_addr, uint8_t *data, uint16_t len);
    static int8_t i2cRead(uint8_t dev_id, uint8_t reg_addr, uint8_t *data, uint16_t len);
    static void delayMs(uint32_t period);

    //bmp structure from the bmp280 API
    struct bmp280_dev bmp;
public:
    /*
     * initialises the bmp280_dev structure of the API
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
     * pass one of the following constants defined in bmp280_defs.h of the API:
     * BMP280_SLEEP_MODE, BMP280_NORMAL_MODE, BMP280_FORCED_MODE
     */
    void setPowerMode(uint8_t mode);

    /*
     * configures the bmp280
     * pass the constants defined in bmp280_defs.h of the bmp280 API
     */
    void configure(uint8_t os_temp, uint8_t os_pres, uint8_t odr, uint8_t filter);

    /*
     * reads the measured data from the registers and compensates it
     * @data: double array with the size 2, 
     * in the first position the temperature will be written in °C and in the second one the pressure in Pa
     * @return: data
     */
    double *getMeasurement(double *data);

    /*
     * @return: the time in ms needed for a measurement with the current configuration
     */
    uint8_t calcMeasurementTime();
};

#endif /* BMP280_H */
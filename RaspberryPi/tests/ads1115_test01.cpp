/*
 * testing the ads1115 without the ads1115 object
 * reading the conf register to check wether its big- or little endian and how to access the registers
 */

#include <stdio.h>
#include <stdlib.h>

#include "../gpio/i2c_dev.h"
#include "../ads1115/ads1115.h"

int main() {
    I2CDev dev(ADS1115_I2C_ADDR0);
    uint16_t reg = dev.readReg16(ADS1115_REG_CONF);
    printf("%x\n", reg);
    return EXIT_SUCCESS;
}
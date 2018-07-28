#include "../gpio/i2c_dev.h"
#include "../qmc5883l/qmc5883l.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>

int main() {
    I2CDev dev(QMC5883L_I2C_ADDR);
    dev.writeReg16(QMC5883L_REG_CTRL2, 0x80); //reset
    usleep(500 * 1000);
    dev.writeReg16(QMC5883L_REG_CTRL1, 0x01); //continuous mode
    usleep(500 * 1000);
    for(int i = 0; i < 10; i++) {
        int16_t x = dev.readReg16(QMC5883L_REG_DX);
        int16_t y = dev.readReg16(QMC5883L_REG_DY);
        int16_t z = dev.readReg16(QMC5883L_REG_DZ);
        printf("(x, y, z): (%d, %d, %d)\n", x, y, z);
        usleep(1000 * 1000);
    }
    return EXIT_SUCCsESS;
}
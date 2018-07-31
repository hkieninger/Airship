/*
 * tests the bmp280 library in normal mode
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "../bmp280/bmp280.h"

int main() {
    Bmp280 bmp;
    printf("conf reg: %x\n", bmp.readReg8(BMP280_REG_CONFIG));
    printf("ctrl meas reg: %x\n", bmp.readReg8(BMP280_REG_CTRL_MEAS));
    for(int i = 0; i < 10; i++) {
        printf("Temperature is %f °C\n", bmp.getTemperature());
        printf("Pressure is %f Pa\n", bmp.getPressure());
        usleep(1000 * 1000);
    }
}
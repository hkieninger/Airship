/*
 * tests the bmp280 library in normal mode
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "../bmp280/bmp280.h"

int main() {
    Bmp280 bmp;
    double data[2];
    /*bmp.reset();
    bmp.writeReg8(0xF4, 0x54);
    bmp.writeReg8(0xF5, 0x1C);
    usleep(2 * 1000);
    printf("%x\n", bmp.readReg8(0xF4));
    printf("%x\n", bmp.readReg8(0xF5));*/
    //bmp.configure(BMP280_OS_4X, BMP280_OS_16X, BMP280_ODR_0_5_MS, BMP280_FILTER_COEFF_8);
    //bmp.setPowerMode(BMP280_SLEEP_MODE);
    for(int i = 0; i < 100; i++) {
        bmp.getMeasurement(data);
        //printf("Temperature is %.2f °C\n", data[0]);
        printf("Pressure is %f Pa\n", data[1]);
        usleep(1000 * 1000);
    }
}
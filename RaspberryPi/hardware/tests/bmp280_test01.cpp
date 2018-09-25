/*
 * test different functions of the bmp280 library
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pigpiod_if2.h>

#include "../bmp280/bmp280.h"

int main() {
    atexit(gpioTerminate);
    if(gpioInitialise() == PI_INIT_FAILED)
        return 1;
    Bmp280 bmp;

    //test forced mode
    printf("test forced mode\n");
    bmp.setPowerMode(BMP280_FORCED_MODE);
    usleep(43200); //time for a measurement
    printf("Temperature is %f °C\n", bmp.getTemperature());
    printf("Pressure is %f Pa\n", bmp.getPressure());
    usleep(1000 * 1000);
    printf("Temperature is %f °C (should be same as before)\n", bmp.getTemperature());
    printf("Pressure is %f Pa (should be same as before)\n", bmp.getPressure());
    bmp.setPowerMode(BMP280_FORCED_MODE);
    usleep(43200); //time for a measurement
    printf("Temperature is %f °C (should be different)\n", bmp.getTemperature());
    printf("Pressure is %f Pa (should be different)\n", bmp.getPressure());

    //test normal mode
    printf("\ntest normal mode\n");
    bmp.setPowerMode(BMP280_NORMAL_MODE);
    for(int i = 0; i < 5; i++) {
        printf("Temperature is %f °C\n", bmp.getTemperature());
        printf("Pressure is %f Pa\n", bmp.getPressure());
        usleep(1000 * 1000);
    }

    //test sleep mode
    bmp.setPowerMode(BMP280_SLEEP_MODE);
    printf("\ntest sleep mode\n");
    for(int i = 0; i < 5; i++) {
        printf("Temperature is %f °C (should always be the same)\n", bmp.getTemperature());
        printf("Pressure is %f Pa (should always be the same)\n", bmp.getPressure());
        usleep(1000 * 1000);
    }

    //test configuration
    printf("\ntest configuration\n");
    printf("before: conf reg: %x\n", bmp.readReg8(BMP280_REG_CONFIG));
    printf("before: ctrl meas reg: %x\n", bmp.readReg8(BMP280_REG_CTRL_MEAS));
    bmp.setPowerMode(BMP280_SLEEP_MODE);
    bmp.setTemperatureOversampling(BMP280_MEASUREMENT_OFF);
    bmp.setPressureOversampling(BMP280_OVERSAMPLING_1);
    bmp.setStandbyTime(BMP280_2000_MS);
    bmp.setFilter(BMP280_FILTER_OFF);
    bmp.setPowerMode(BMP280_NORMAL_MODE);
    printf("after: conf reg: %x\n", bmp.readReg8(BMP280_REG_CONFIG));
    printf("after: ctrl meas reg: %x\n", bmp.readReg8(BMP280_REG_CTRL_MEAS));
    for(int i = 0; i < 10; i++) {
        printf("Temperature is %f °C (should be always the same and nonesense because its off)\n", bmp.getTemperature());
        printf("Pressure is %f Pa (every second should be the same)\n", bmp.getPressure());
        usleep(1000 * 1000);
    }

    //test reset
    printf("\ntest reset\n");
    bmp.reset();
    for(int i = 0; i < 3; i++) {
        printf("Temperature is %f °C (should correlate with default settings\n", bmp.getTemperature());
        printf("Pressure is %f Pa (should correlate with default settings)\n", bmp.getPressure());
        usleep(1000 * 1000);
    }
    return EXIT_SUCCESS;
}
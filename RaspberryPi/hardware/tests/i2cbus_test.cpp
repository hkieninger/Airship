#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pigpio.h>

#include "../bmp280/bmp280.h"
#include "../mpu6050/mpu6050.h"
#include "../ads1115/ads1115.h"

int main() {
    atexit(gpioTerminate);
    if(gpioInitialise() == PI_INIT_FAILED)
        return 1;
    Bmp280 bmp280;
    Mpu6050 mpu6050;
    Ads1115 ads1115;
    //wait to take the first measurements
    sleep(1);
    printf("temperature bmp280: %f °C\n", bmp280.getTemperature());
    printf("temperature mpu6050: %f °C\n", mpu6050.getTemperature());
    printf("pressure bmp280: %f Pa\n", bmp280.getPressure());
    float accel[3];
    float gyro[3];
    mpu6050.getAccel(accel);
    mpu6050.getGyro(gyro);
    printf("accel mpu6050 (x/y/z): (%f/%f/%f) m/s^2\n", accel[0], accel[1], accel[2]);
    printf("gyro mpu6050 (x/y/z): (%f/%f/%f) °/s\n", gyro[0], gyro[1], gyro[2]);
    printf("voltage ads1115: %f V\n", ads1115.getSingleShot());
    return 0;
}
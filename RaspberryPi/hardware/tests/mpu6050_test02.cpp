/*
 * tests setCycleMode(), setCycleFreq(), setSleepMode() of the Mpu6050 library
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pigpiod_if2.h>

#include "../mpu6050/mpu6050.h"

Mpu6050 *mpu;
float accel[3], gyro[3];

int main() {
    atexit(gpioTerminate);
    if(gpioInitialise() == PI_INIT_FAILED)
        return 1;
    printf("test begins: \n\n");
    printf("measure in cycle mode with a freq of 1.25Hz for 10 seconds\n\n");
    mpu = new Mpu6050();
    mpu->setCycleFreq(MPU6050_1_25_HZ);
	mpu->setCycleMode(true);
    for(int i = 0; i < 10; i++) {
        mpu->getAccel(accel);
        printf("accel x %.2f m/s^2\n", accel[0]);
        printf("accel y %.2f m/s^2\n", accel[1]);
        printf("accel z %.2f m/s^2\n\n", accel[2]);
        usleep(1000 * 1000);
    }
    mpu->setSleepMode(true);
    printf("went to sleep mode for 10 seconds\n\n");
    for(int i = 0; i < 10; i++) {
        mpu->getAccel(accel);
        printf("accel x %.2f m/s^2\n", accel[0]);
        printf("accel y %.2f m/s^2\n", accel[1]);
        printf("accel z %.2f m/s^2\n\n", accel[2]);
        usleep(2000 * 1000);
    }
    mpu->setCycleFreq(MPU6050_5_HZ);
    printf("measure in cycle mode with a freq of 5 Hz for 3 seconds\n\n");
    mpu->setSleepMode(false);
    for(int i = 0; i < 10; i++) {
        mpu->getAccel(accel);
        printf("accel x %.2f m/s^2\n", accel[0]);
        printf("accel y %.2f m/s^2\n", accel[1]);
        printf("accel z %.2f m/s^2\n\n", accel[2]);
        usleep(20 * 1000);
    }
    delete mpu;
    printf("test terminated\n");
    return EXIT_SUCCESS;
}
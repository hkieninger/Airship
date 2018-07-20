/*
 * tests the interrupts, setCycleMode(), setCycleFreq(), setSleepMode() of the Mpu6050 library
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <wiringPi.h>

#include "../mpu6050/mpu6050.h"

#define INTERRUPT_PIN 7

Mpu6050 *mpu;
float accel[3], gyro[3];

/*
 * normally should only set flag from interrupt routine
 * readout device from interrupt  routine is normally a no-go (resource synchronization)
 */
void mpu6050DataReady(void) {
    mpu->getAccel(accel);
    printf("accel x %.2f m/s^2\n", accel[0]);
    printf("accel y %.2f m/s^2\n", accel[1]);
    printf("accel z %.2f m/s^2\n\n", accel[2]);
}

int main() {
    wiringPiSetup();
    printf("test begins: \n\n");
    printf("measure in cycle mode with a freq of 1.25Hz for 10 seconds\n\n");
    mpu = new Mpu6050(INTERRUPT_PIN, mpu6050DataReady);
    mpu->setCycleFreq(MPU6050_1_25_HZ);
	mpu->setCycleMode(true);
    sleep(10);
    mpu->setSleepMode(true);
    printf("went to sleep mode for 10 seconds\n\n");
    sleep(10);
    mpu->setCycleFreq(MPU6050_5_HZ);
    printf("measure in cycle mode with a freq of 5 Hz for 3 seconds\n\n");
    mpu->setSleepMode(false);
    sleep(3);
    delete mpu;
    printf("test terminated\n");
    return EXIT_SUCCESS;
}
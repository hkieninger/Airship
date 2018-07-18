#include <stdio.h>
#include <unistd.h>
#include <wiringPi.h>

#include "../mpu6050/mpu6050.h"

#define INTERRUPT_PIN 7

Mpu6050 *mpu;
float accel[3], gyro[3];

void mpu6050DataReady(void) {
    mpu->getAccel(accel);
    mpu->getGyro(gyro);
    printf("accel x %.2f\n", accel[0]);
    printf("accel y %.2f\n", accel[1]);
    printf("accel z %.2f\n", accel[2]);
    printf("gyro x %.2f\n", gyro[0]);
    printf("gyro y %.2f\n", gyro[1]);
    printf("gyro z %.2f\n", gyro[2]);
}

int main() {
    wiringPiSetup();
    mpu = new Mpu6050(INTERRUPT_PIN, mpu6050DataReady);
    sleep(30);
    delete mpu;
    return 0;
}
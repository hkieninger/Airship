/*
 * tests temperature, gyro and accelerometer measurements of the Mpu6050 library
 */

#include "../mpu6050/mpu6050.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(void) {
	Mpu6050 mpu;
	for(int i = 0; i < 10; i++) {
		float temp = mpu.getTemperature();
		float accel[3], gyro[3];
		mpu.getAccel(accel);
		mpu.getGyro(gyro);

		printf("temperature %.2f °C\n", temp);

		printf("accel x %.2f m/s^2\n", accel[0]);
    	printf("accel y %.2f m/s^2\n", accel[1]);
    	printf("accel z %.2f m/s^2\n\n", accel[2]);

		printf("gyro x %.2f °/s\n", gyro[0]);
    	printf("gyro y %.2f °/s\n", gyro[1]);
    	printf("gyro z %.2f °/s\n\n", gyro[2]);

		usleep(500 * 1000);
	}
	return EXIT_SUCCESS;
}

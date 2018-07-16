#include "../mpu6050.h"
#include "../../gpio/gpio_exception.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

//test close, with signal catching

#define MPS 100

int main(void) {
	Mpu6050 mpu;
	while(true) {
		float temp = 0, gx = 0, gy = 0, gz = 0, ax = 0, ay = 0, az = 0;
		for(int i = 0; i < MPS; i++) {
			temp += mpu.getTemperature();
			gx += mpu.getGyroX();
			gy += mpu.getGyroY();
			gz += mpu.getGyroZ();
			ax += mpu.getAccelX();
			ay += mpu.getAccelY();
			az += mpu.getAccelZ();
			usleep(1000 * 1000 / MPS);
		}
		temp /= MPS;
		gx /= MPS;
		gy /= MPS;
		gz /= MPS;
		ax /= MPS;
		ay /= MPS;
		az /= MPS;
		printf("----------------------------------------------------\n");
		printf("temperature:\t%+.2f%s", temp, "\t°C\n");
		printf("gyro x:     \t%+.2f%s", gx, "\t°/s\n");
		printf("gyro y:     \t%+.2f%s", gy, "\t°/s\n");
		printf("gyro z:     \t%+.2f%s", gz, "\t°/s\n");
		printf("accel x:    \t%+.2f%s", ax, "\tm/s^2\n");
		printf("accel y:    \t%+.2f%s", ay, "\tm/s^2\n");
		printf("accel z:    \t%+.2f%s", az, "\tm/s^2\n");
		printf("----------------------------------------------------\n");
	}
	return EXIT_SUCCESS;
}

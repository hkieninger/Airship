/*
 * reads out the measured values from the register and prints them to the screen
 * calculates in addition the azimuth/angle relative to the north
 * from the qmc5883l we bought, none works properly, but the values outputed are constant (I assume a hardware error)
 * therefore the problem could be solved with a table or polynomial with the correct values
 */

#include "../qmc5883l/qmc5883l.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <pigpio.h>

#define BYTE_TO_BINARY_PATTERN "%c%c%c%c%c%c%c%c"
#define BYTE_TO_BINARY(byte)  \
  (byte & 0x80 ? '1' : '0'), \
  (byte & 0x40 ? '1' : '0'), \
  (byte & 0x20 ? '1' : '0'), \
  (byte & 0x10 ? '1' : '0'), \
  (byte & 0x08 ? '1' : '0'), \
  (byte & 0x04 ? '1' : '0'), \
  (byte & 0x02 ? '1' : '0'), \
  (byte & 0x01 ? '1' : '0') 

void printAllReg(Qmc5883l *qmc) {
	for(int reg = 0; reg <= 0x0d; reg += 1) {
		int byte = qmc->readReg8(reg);
		printf("register %x has value " BYTE_TO_BINARY_PATTERN "\n", reg, BYTE_TO_BINARY(byte));
	}
}

int main(void) {
	atexit(gpioTerminate);
	if(gpioInitialise() == PI_INIT_FAILED)
        return 1;
	Qmc5883l qmc;
	//printAllReg(&qmc);
	while(true) {
		float mag[3];
		qmc.getMag(mag);
		printf("----------------------------------------------------\n");
		printf("magnetic field x:\t%+.4f%s", mag[0], "\tGs\n");
		printf("magnetic field y:\t%+.4f%s", mag[1], "\tGs\n");
		printf("magnetic field z:\t%+.4f%s", mag[2], "\tGs\n");
		printf("absolute value is:\t%.4f%s", sqrt(mag[0]*mag[0] + mag[1]*mag[1] + mag[2]*mag[2]), "\tGs\n");
		int angle;
		if(mag[0] == 0) {
			if(mag[1] > 0)
				angle = 90;
			else
				angle = 270;
		} else {
			if(mag[0] > 0)
				angle = (int) (0.5 + atan(mag[1]/mag[0]) * 180 / M_PI);
			else
				angle = (int) (0.5 + atan(mag[1]/mag[0]) * 180 / M_PI) + 180;
			angle += 360;
			angle %= 360;
		}
		printf("angle: \t%d\n", angle);
		printf("----------------------------------------------------\n");
		usleep(1000 * 1000);
	}
	return EXIT_SUCCESS;
}

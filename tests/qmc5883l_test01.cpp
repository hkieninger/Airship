#include "../qmc5883l/qmc5883l.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>

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

//test close with signal catching

int main(void) {
	Qmc5883l qmc;
	qmc.writeReg8(0x0B, 0x01);
	qmc.writeBitReg8(0x0A, 6, true);
	sleep(1);
	printAllReg(&qmc);
	while(true) {
		double x = qmc.getMagX();
		double y = qmc.getMagY();
		double z = qmc.getMagZ();
		printf("----------------------------------------------------\n");
		printf("magnetic field x:\t%+.4f%s", x, "\tGs\n");
		printf("magnetic field y:\t%+.4f%s", y, "\tGs\n");
		printf("magnetic field z:\t%+.4f%s", z, "\tGs\n");
		printf("absolute value is:\t%.4f%s", sqrt(x*x + y*y + z*z), "\tGs\n");
		int angle;
		if(x == 0) {
			if(y > 0)
				angle = 90;
			else
				angle = 270;
		} else {
			if(x > 0)
				angle = (int) (0.5 + atan(y/x) * 180 / M_PI);
			else
				angle = (int) (0.5 + atan(y/x) * 180 / M_PI) + 180;
			angle += 360;
			angle %= 360;
		}
		printf("angle: \t%d\n", angle);
		printf("----------------------------------------------------\n");
		sleep(1);
	}
	return EXIT_SUCCESS;
}

/*
 * tests the functionalities of the qmc5883l 
 */

#include "../gpio/i2c_dev.h"
#include "../qmc5883l/qmc5883l.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <sys/time.h>

int main() {
    Qmc5883l qmc;

    //test dataReady() and setContinuous()
    printf("test 10 Hz\n");
    float mag[3];
    struct timeval start, stop;
    gettimeofday(&start, NULL);
    for(int i = 0; i < 10; i++) {
        while(!qmc.dataReady()) {
            usleep(1000);
        }
        qmc.getMag(mag); //reset flag
    }
    gettimeofday(&stop, NULL);
    printf("took %lu milliseconds (value should be: ~1000)\n", (stop.tv_sec - start.tv_sec) * 1000 + (stop.tv_usec - start.tv_usec) / 1000);

    printf("\ntest 200 Hz\n");
    qmc.setContinuous(QMC5883L_200_HZ);
    gettimeofday(&start, NULL);
    for(int i = 0; i < 10; i++) {
        while(!qmc.dataReady()) {
            usleep(1000);
        }
        qmc.getMag(mag); //reset flag
    }
    gettimeofday(&stop, NULL);
    printf("took %lu milliseconds (value should be: ~50)\n", (stop.tv_usec - start.tv_usec) / 1000);

    //test setStandby()
    printf("\ntest set standby\n");
    qmc.setStandby();
    for(int i = 0; i < 5; i++) {
        qmc.getMag(mag);
        printf("mag x is %f (value should be the same)\n", mag[0]);
        usleep(1000 * 1000);
    }

    //test range
    printf("\ntest set range\n");
    qmc.setContinuous(QMC5883L_10_HZ);
    qmc.setRange(QMC5883L_2_GAUSS);
    for(int i = 0; i < 5; i++) {
        qmc.getMag(mag);
        printf("mag x is %f (range 2 gauss)\n", mag[0]);
        usleep(1000 * 1000);
    }
    qmc.setRange(QMC5883L_8_GAUSS);
    usleep(1000 * 1000);
    for(int i = 0; i < 5; i++) {
        qmc.getMag(mag);
        printf("mag x is %f (range 8 gauss)\n", mag[0]);
        usleep(1000 * 1000);
    }

    //test reset
    printf("\ntest reset\n");
    qmc.reset();
    for(int i = 0; i < 5; i++) {
        qmc.getMag(mag);
        printf("mag x is %f\n", mag[0]);
        usleep(1000 * 1000);
    }

    return EXIT_SUCCESS;
}
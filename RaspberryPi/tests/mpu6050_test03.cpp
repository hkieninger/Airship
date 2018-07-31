/*
 * test the setInterrupts(), setSampleRateDivider() methods of the Mpu6050 library
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <wiringPi.h>

#include "../mpu6050/mpu6050.h"

#define INTERRUPT_PIN 7

int count = 0;

void mpu6050DataReady(void) {
    count++;
}

int main() {
    wiringPiSetup();
    Mpu6050 mpu(INTERRUPT_PIN, mpu6050DataReady);
    sleep(10);
    printf("registered %d interrupts in 10 seconds with a samplerate of 1 kHz\n", count);
    mpu.setInterrupts(false);
    count = 0;
    sleep(10);
    printf("registered %d interrupts (should be 0)\n", count);
    mpu.setSampleRateDivider(255);
    mpu.setInterrupts(true);
    sleep(10);
    printf("registered %d interrupts in 10 seconds with a samplerate of 4 Hz\n", count);
    return EXIT_SUCCESS;
}
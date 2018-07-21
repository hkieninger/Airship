/*
 * test all the functions of ads1115 library
 * pin 0 should be left floating and to pin 2 should be attached a known voltage (e.g. voltage divider with some resistors)
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "../ads1115/ads1115.h"

int main() {
    Ads1115 ads;
    printf("voltage on pin 0 is %.4f V\n", ads.getSingleShot());
    printf("voltage on pin 2 is %.4f V\n", ads.getSingleShot(2));
    printf("measured voltage is %.4f V (should be same as before)\n", ads.getVoltage());
    ads.setInputPin(0);
    ads.setSampleRate(ADS1115_8_SPS);
    printf("\nswitching to continuous mode\n");
    ads.setContinuousMode(true);
    for(int i = 0; i < 10; i++) {
        usleep(62500);
        printf("measured voltage is %f (approximately every second measurement should be exactly identical)\n", ads.getVoltage());
    }
    ads.setSampleRate(ADS1115_860_SPS);
    for(int i = 0; i < 10; i++) {
        usleep(62500);
        printf("measured voltage is %f\n", ads.getVoltage());
    }
    printf("\nreturning to single shot mode\n");
    ads.setContinuousMode(false);
    printf("measured voltage is %.4f V\n", ads.getVoltage());
    printf("measured voltage is %.4f V (should be same as before)\n", ads.getVoltage());
    return EXIT_SUCCESS;
}
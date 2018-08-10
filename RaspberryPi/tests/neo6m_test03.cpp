/*
 * tests powerOff, powerOn, measurement rate and clear, load, save configuration
 */

#define SERIAL_PORT "/dev/ttyAMA0"

#include <stdio.h>
#include <unistd.h>
#include <math.h>
#include <stdlib.h>
#include <pigpio.h>

#include "../neo6m/neo6m.h"

void printGeodeticPos(struct GPSGeodeticPos &pos) {
    printf("Geodetic Pos | timestamp: %d | lon: %f °, lat: %f ° |\n " \
    " h above ellips: %d mm, h above sea: %d mm | horizontal acc: %d mm, vertical acc: %d mm\n",\
    pos.time, pos.lon * pow(10, -7), pos.lat * pow(10, -7), pos.hae, pos.has, pos.haccuracy, pos.vaccuracy);
}

void printStatus(struct GPSFixStatus &status) {
    printf("Status | timestamp: %d ", status.time);
    printf("| gpsFix: ");
    switch(status.gpsFix) {
        case NEO6M_FIX_NO_FIX: printf("NO_FIX "); break;
        case NEO6M_FIX_DEAD_RECKONING_ONLY: printf("DEAD_RECKONING_ONLY "); break;
        case NEO6M_FIX_2D: printf("FIX_2D "); break;
        case NEO6M_FIX_3D: printf("FIX_3D "); break;
        case NEO6M_FIX_DEAD_RECKONING_GPS: printf("DEAD_RECKONING_GPS "); break;
        case NEO6M_FIX_TIME_ONLY_FIX: printf("TIME_ONLY_FIX "); break;
        default:
            fprintf(stderr, "invalid gps fix type: %d\n", status.gpsFix);
            exit(1);
    }
    printf("| satellites: %d ", status.satellites);
    printf("| dgps: %d ", status.dgps);
    printf("| fixValid: %d ", status.fixValid);
    printf("| timeValid: %d ", status.timeValid);
    printf("\n");
}

void printMeasurements(Neo6M &neo, int n, int sleepms) {
    struct GPSFixStatus status;
    struct GPSGeodeticPos gpos;
    for(int i = 0; i < n; i++) {
        //neo->getFixStatus(status);
        neo.getGeodeticPos(gpos);
        //printStatus(status);
        printGeodeticPos(gpos);
        usleep(sleepms * 1000);
        printf("\n");
    }
}

int main() {
    atexit(gpioTerminate);
    if(gpioInitialise() == PI_INIT_FAILED)
        return 1;
    Neo6M neo(SERIAL_PORT);
    printf("measurement rate is 1 Hz, look at timestamp\n");
    printMeasurements(neo, 10, 100);

    neo.setMeasurementRate(200); //will get lost, because of powerOff()
    printf("power off for 5 seconds\n");
    neo.powerOff(5000);
    sleep(6);
    neo.setProtocol(UBX_ONLY);
    printf("measurement rate should be 1 Hz, look at timestamp\n");
    printMeasurements(neo, 10, 100);
    
    neo.setMeasurementRate(200); //ms -> 5Hz
    printf("\npower off for 3 seconds\n");
    neo.saveConfiguration();
    neo.powerOff(0);
    usleep(3 * 1000 * 1000);
    neo.powerOn();
    printf("\npowered on, measurement rate should be 5 Hz, look at timestamp\n");
    printMeasurements(neo, 10, 100);
}
/*
 * tests SBAS, PPP, position, velocity, status
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <math.h>

#include "../neo6m/neo6m.h"

#define SERIAL_PORT "/dev/ttyS0"

Neo6M *neo = NULL;

void cleanup(int sig) {
    if(sig == SIGINT) {
        if(neo != NULL)
            delete neo;
        exit(EXIT_SUCCESS);
    }
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

void printEcefPos(struct GPSEcefPos &pos) {
    printf("Ecef Pos | timestamp: %d | x: %d cm, y: %d cm, z: %d cm | accuracy: %d cm\n", pos.time, pos.x, pos.y, pos.z, pos.accuracy);
}

void printEcefVel(struct GPSEcefVel &vel) {
    printf("Ecef Vel | timestamp: %d | x: %d cm/s, y: %d cm/s, z: %d cm/s | accuracy: %d cm/s\n", vel.time, vel.x, vel.y, vel.z, vel.accuracy);
}

void printGeodeticPos(struct GPSGeodeticPos &pos) {
    printf("Geodetic Pos | timestamp: %d | lon: %f °, lat: %f ° |\n " \
    " h above ellips: %d mm, h above sea: %d mm | horizontal acc: %d mm, vertical acc: %d mm\n",\
    pos.time, pos.lon * pow(10, -7), pos.lat * pow(10, -7), pos.hae, pos.has, pos.haccuracy, pos.vaccuracy);
}

void printGeodeticVel(struct GPSGeodeticVel &vel) {
    printf("Geodetic Vel | timestamp: %d | north: %d cm/s, east: %d cm/s, down: %d cm/s |\n " \
    " speed: %d cm/s, gspeed: %d cm/s | heading: %f °| speed acc: %d cm/s, heading acc: %f °\n",\
    vel.time, vel.north, vel.east, vel.down, vel.speed, vel.gspeed, vel.heading * pow(10, -5), vel.saccuracy, vel.haccuracy * pow(10, -5));
}

void printMeasurements(int n) {
    struct GPSFixStatus status;
    struct GPSEcefPos epos;
    struct GPSEcefVel evel;
    struct GPSGeodeticPos gpos;
    struct GPSGeodeticVel gvel;
    for(int i = 0; i < n; i++) {
        neo->getFixStatus(status);
        neo->getEcefPos(epos);
        neo->getEcefVel(evel);
        neo->getGeodeticPos(gpos);
        neo->getGeodeticVel(gvel);
        printStatus(status);
        printEcefPos(epos);
        printEcefVel(evel);
        printGeodeticPos(gpos);
        printGeodeticVel(gvel);
        sleep(1);
        printf("\n");
    }
}

int main(void) {
    //signal(SIGINT, cleanup);
    neo = new Neo6M(SERIAL_PORT);
    struct GPSFixStatus status;
    
    printf("wait valid fix\n");
    do {
        sleep(1);
        neo->getFixStatus(status);
    } while(!status.timeValid || !status.fixValid);
    printf("valid fix made\n\n");
    
    printMeasurements(10);

    neo->setSBAS(true);
    printf("\nwait SBAS\n");
    do {
        sleep(1);
        neo->getFixStatus(status);
    } while(!status.dgps);
    printf("SBAS found\n");

    printMeasurements(10);

    neo->setPPP(true);
    printf("\nPPP enabled\n");
    while(true) {
        printMeasurements(1);
    }

    delete neo;

    return EXIT_SUCCESS;
}

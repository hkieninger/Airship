/*
 * tests the NMEA part of the neo6m library
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

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

int main(void) {
    signal(SIGINT, cleanup);
    neo = new Neo6M(SERIAL_PORT);
    neo->setProtocol(UBX_AND_NMEA);
    neo->disableNMEAMessage("GSV");
    neo->disableNMEAMessage("GSA");
    neo->disableNMEAMessage("GLL");
    neo->disableNMEAMessage("VTG");
    neo->disableNMEAMessage("GGA");
    usleep(250 * 1000); //sleep 250ms, it's necessary
    neo->flush();

    printf("should only be RMC and TXT messages\n");
    for(int i = 0; i < 10; i++) {
        printf("%s",neo->readNMEAMessage().c_str());
    }
    
    neo->enableNMEAMessage("GGA");
    printf("\nshould be RMC and GGA and TXT messages\n");
    for(int i = 0; i < 10; i++) {
        printf("%s",neo->readNMEAMessage().c_str());
    }

    delete neo;

    return EXIT_SUCCESS;
}
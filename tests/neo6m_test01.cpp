#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

#include "../neo6m/neo6m.h"
#include "../gpio_exception.h"

#define SERIAL_PORT "/dev/serial0"

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

    neo->setProtocol(NMEA_AND_UBX);

    neo = new Neo6M(SERIAL_PORT);
    neo->disableNMEAMessage("GSV");
    neo->disableNMEAMessage("GSA");
    neo->disableNMEAMessage("GLL");
    neo->disableNMEAMessage("VTG");

    for(int i = 0; i < 10; i++) {
        printf("%s\n",neo->readNMEAMessage().c_str());
    }
    neo->setPowerSaveMode(10 * 1000);
    usleep(10 * 1000 * 1000);
    neo->wakeUp();
    for(int i = 0; i < 10; i++) {
        printf("%s\n",neo->readNMEAMessage().c_str());
    }
    
    return EXIT_SUCCESS;
}
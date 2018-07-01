#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <wiringSerial.h>

#include "neo6m.h"
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
    neo = new Neo6M(SERIAL_PORT);
    neo->disableNMEAMessage("GSV");
    neo->disableNMEAMessage("GSA");
    neo->disableNMEAMessage("GLL");
    neo->disableNMEAMessage("VTG");
    int fd = neo->getFd();
    while(true) {
        char c = serialGetchar(fd);
        if(c != -1)
            printf("%c", c);
    }
    return EXIT_SUCCESS;
}
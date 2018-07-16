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

void readNMEA(int fd, int messages) {
    int count = 0;
    while(count < messages) {
        char c = serialGetchar(fd);
        if(c != -1)
            printf("%c", c);
        if(c == '\n')
            count++;
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
    readNMEA(fd, 10);
    neo->setPowerSaveMode(10 * 1000);
    usleep(10 * 1000 * 1000);
    neo->wakeUp();
    usleep(500 * 1000);
    readNMEA(fd, 40);
    
    return EXIT_SUCCESS;
}
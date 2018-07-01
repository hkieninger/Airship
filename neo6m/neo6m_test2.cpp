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
    neo->reset();
    int fd = neo->getFd();
    char last = 0;
    while(true) {
        char c = serialGetchar(fd);
	if(c == '$') {
	    if(last != '\n')
		printf("\n");
	    printf("%c", c);
	    while(c != '\n') {
		c = serialGetchar(fd);
		if(c != -1)
		    printf("%c", c);
	    }
        neo->pollUBXMessage(0x01, 0x01);
	} else {
	    printf("%.2X ", c);
	}
	last = c;
    }
    return EXIT_SUCCESS;
}

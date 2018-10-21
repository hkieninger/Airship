#include <wiringSerial.h>
#include <stddef.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <string>

#include "gpio_exception.h"
#include "uart_dev.h"

//#define DEBUG 1

/*
 * wrapper functions of wiringPi
 */

UARTDev::UARTDev(const std::string &serialport, int baudrate) {
    fd = serialOpen(serialport.c_str(), baudrate);
    if(fd < 0)
        throw UARTException("opening serial port: " + std::string(strerror(errno)));
}

UARTDev::~UARTDev() {
    serialClose(fd);
}

void UARTDev::flush() {
    serialFlush(fd);
}

int UARTDev::available() {
    int ret = serialDataAvail(fd);
    if(ret < 0)
        throw UARTException("checking for available bytes from UART: " + std::string(strerror(errno)));
    return ret;
}

unsigned char UARTDev::getChar() {
    int c = serialGetchar(fd);
    if(c < 0)
         throw UARTException("reading from UART: " + std::string(strerror(errno)));

    #ifdef DEBUG
    printf("read character: %c, %X\n", c, c);
    #endif

    return c;
}

void UARTDev::putChar(unsigned char c) {
    serialPutchar(fd, c) ;
}

void *UARTDev::readAll(void *buf, size_t count) {
    size_t readed = 0; //grammatically wrong
    while(readed < count) {
        int ret = read(fd, (uint8_t *) buf + readed, count - readed);
        if(ret < 0)
            throw UARTException("reading from UART: " + std::string(strerror(errno)));
        readed += ret;
    }

    #ifdef DEBUG
    printf("read characters:");
    for(size_t i = 0; i < count; i++) {
        printf(" %c, %X |", ((char *) buf)[i], ((char *) buf)[i]);
    }
    printf("\n");
    #endif

    return buf;
}

void UARTDev::writeAll(const void *buf, size_t count) {
    size_t written = 0;
    while(written < count) {
        int ret = write(fd, (uint8_t *) buf + written, count - written);
        if(ret < 0)
            throw UARTException("writing to UART: " + std::string(strerror(errno)));
        written += ret;
    }
}
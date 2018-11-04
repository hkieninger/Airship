#include <wiringSerial.h>
#include <stddef.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <string>

#include "gpio_exception.h"
#include "uart_dev.h"

//DEBUG
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

/*
 * wrapper functions of wiringPi
 */

UARTDev::UARTDev(const std::string &serialport, int baudrate) {
    fd = serialOpen(serialport.c_str(), baudrate);
    if(fd < 0)
        throw UARTException("opening serial port: " + std::string(strerror(errno)));
    
    //DEBUG
    printf("Max. offene Dateien : %d\n",FOPEN_MAX);
    logUart = fopen("uart.txt", "w");
    if(logUart == NULL) {
        perror("failed to create uart logUart.");
        exit(1);
    }
    printf("opened file %p\n", logUart);
}

UARTDev::~UARTDev() {
    serialClose(fd);

    //DEBUG
    printf("closing file %p\n", logUart);
    if(fclose(logUart) != 0) {
        perror("failed to close uart logUart.");
        exit(1);
    }
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

    //DEBUG
    //printf("writing file %p\n", logUart);
    if(fwrite(&c, 1, 1, logUart) != 1) {
        perror("failed to write uart logUart");
        exit(1);
    }

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

    //DEBUG
    //printf("writing file %p\n", logUart);
    if(fwrite(buf, count, 1, logUart) != 1) {
        perror("failed to write uart logUart");
        exit(1);
    }

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
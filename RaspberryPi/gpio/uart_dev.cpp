#include <pigpio.h>
#include <stddef.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <termios.h>
#include <string>

#include "gpio_exception.h"
#include "uart_dev.h"

/*
 * wrapper functions of wiringPi
 */

UARTDev::UARTDev(const std::string &serialport, int baudrate) {
    fd = serOpen(serialport.c_str(), baudrate, 0);
    if(fd < 0)
        throw UARTException("opening serial port: " + std::string(strerror(errno)));
}

UARTDev::~UARTDev() {
    serClose(fd);
}

void UARTDev::flush() {
    tcflush (fd, TCIOFLUSH) ;
}

int UARTDev::available() {
    int ret = serDataAvailable(fd);
    if(ret < 0)
        throw UARTException("checking for available bytes from UART: " + std::string(strerror(errno)));
}

unsigned char UARTDev::getChar() {
    unsigned char c;
    readAll(&c, 1);
    return c;
}

void UARTDev::putChar(unsigned char c) {
    writeAll(&c, 1);
}

void *UARTDev::readAll(void *buf, size_t count) {
    int readed = 0; //grammatically wrong
    while(readed < count) {
        int ret = read(fd, (uint8_t *) buf + readed, count - readed);
        if(ret < 0)
            throw UARTException("reading from UART: " + std::string(strerror(errno)));
        readed += ret;
    }
    return buf;
}

void UARTDev::writeAll(const void *buf, size_t count) {
    int written = 0;
    while(written < count) {
        int ret = write(fd, (uint8_t *) buf + written, count - written);
        if(ret < 0)
            throw UARTException("writing to UART: " + std::string(strerror(errno)));
        written += ret;
    }
}
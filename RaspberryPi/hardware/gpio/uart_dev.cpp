#include <pigpio.h>
#include <stddef.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <string>

#include "gpio_exception.h"
#include "uart_dev.h"

/*
 * wrapper functions of wiringPi
 */

UARTDev::UARTDev(const std::string &serialport, int baudrate) {
    char str[serialport.size() + 1];
    memcpy(str, serialport.c_str(), sizeof(str));
    handle = serOpen(str, baudrate, 0);
    if(handle < 0)
        throw UARTException("opening serial port: " + std::string(strerror(errno)));
}

UARTDev::~UARTDev() {
    serClose(handle);
}

int UARTDev::available() {
    int ret = serDataAvailable(handle);
    if(ret < 0)
        throw UARTException("checking for available bytes from UART: " + std::string(strerror(errno)));
    return ret;
}

unsigned char UARTDev::getChar() {
    int c = serReadByte(handle);
    if(c < 0)
         throw UARTException("reading from UART: " + std::string(strerror(errno)));
    return c;
}

void UARTDev::putChar(unsigned char c) {
    if(serWriteByte(handle, c) != 0)
        throw UARTException("writing to UART: " + std::string(strerror(errno)));
}

void *UARTDev::readAll(void *buf, size_t count) {
    size_t readed = 0; //grammatically wrong
    while(readed < count) {
        int ret = serRead(handle, (char *) buf + readed, count - readed);
        if(ret < 0)
            throw UARTException("reading from UART: " + std::string(strerror(errno)));
        readed += ret;
    }
    return buf;
}

void UARTDev::writeAll(const void *buf, size_t count) {
    size_t written = 0;
    while(written < count) {
        int ret = serWrite(handle, (char *) buf + written, count - written);
        if(ret != 0)
            throw UARTException("writing to UART: " + std::string(strerror(errno)));
        written += ret;
    }
}
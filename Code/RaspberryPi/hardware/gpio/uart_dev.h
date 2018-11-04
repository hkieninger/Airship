#ifndef UART_DEV_H
#define UART_DEV_H 1

#include <stddef.h>
#include <string>

#include <stdio.h> //DEBUG

/*
 * represents a device which communicates over the asynchronous serial interface (uses the UART)
 * functions throw UARTException on errors
 */
class UARTDev {
    FILE *logUart = NULL; //DEBUG
protected:
	/*
	 * file descriptor to the device file
     * if the class doesn't fullfill your need you can manipulate it with termios 
     * and use write() and read()
	 */
	int fd;
public:
	/*
	 * initialises an uart device
	 * opens the file descriptor
     * the serial device is in raw mode and the timeout is set to 10 seconds
	 * @serialport: path to the device file representing the uart
     * @baudrate: of the uart
	 */
	UARTDev(const std::string &serialport, int baudrate);
	
	/*
	 * closes the file descriptor
	 */
	virtual ~UARTDev();

    /* 
     * discards all data received, or waiting to be send down the given device 
     */
    void flush();

    /*
     * @return: the number of bytes available for reading
     */
    int available();

    /*
     * @return: the next byte in the device file
     */
    unsigned char getChar();

    /*
     * writes a byte into the device file
     */
    void putChar(unsigned char c);
    
    /*
     * reads count bytes from fd into buf
     * in contrast to read from <unistd.h> it guarantees that exactly count bytes are read
     * @return: buf
     */
    void *readAll(void *buf, size_t count);

    /*
     * writes count bytes from buf into fd
     * in contrast to write from <unistd.h> it guarantees that exactly count bytes are written
     */
    void writeAll(const void *buf, size_t count);
};

#endif /* UART_DEV_H */
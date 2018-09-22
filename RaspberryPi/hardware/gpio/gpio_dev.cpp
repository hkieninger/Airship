#include <pigpio.h>

#include "gpio_exception.h"
#include "gpio_dev.h"

int GpioDevice::gpioHandle = -1;

void GpioDevice::initialiseGpio() {
    gpioHandle = pigpio_start(NULL, NULL);
    if(gpioHandle < 0)
        throw GPIOException("Failed to initialise gpio.");
}

void GpioDevice::terminateGpio() {
    pigpio_stop(gpioHandle);
    gpioHandle = -1;
}
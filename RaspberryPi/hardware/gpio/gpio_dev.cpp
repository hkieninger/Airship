#include <pigpio.h>

#include "gpio_exception.h"
#include "gpio_dev.h"

void GpioDevice::initialiseGpio() {
    if(gpioInitialise() < 0)
        throw GPIOException("Failed to initialise gpio.");
}

void GpioDevice::terminateGpio() {
    gpioTerminate();
}
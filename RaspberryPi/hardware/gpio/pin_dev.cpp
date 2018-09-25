#include <pigpiod_if2.h>

#include "gpio_exception.h"
#include "pin_dev.h"

PinDevice::PinDevice(int pin) : pin(pin) {
    if(GpioDevice::gpioHandle < 0)
        throw GPIOException("GPIO wasn't initialised.");
}

void PinDevice::setPinMode(unsigned int mode) {
    int ret = set_mode(GpioDevice::gpioHandle, pin, mode);
    if(ret != 0)
        throw GPIOException("Failed to set pin mode: " + std::string(pigpio_error(ret)));
}

int PinDevice::readPin() {
    int ret = gpio_read(GpioDevice::gpioHandle, pin);
    if(ret == PI_BAD_GPIO)
        throw GPIOException("Failed to read pin: " + std::string(pigpio_error(ret)));
    return ret;
}

void PinDevice::writePin(unsigned int level) {
    int ret = gpio_write(GpioDevice::gpioHandle, pin, level);
    if(ret != 0)
        throw GPIOException("Failed to write pin: " + std::string(pigpio_error(ret)));
}

void PinDevice::setPulsewidth(unsigned int microseconds) {
    int ret = set_servo_pulsewidth(GpioDevice::gpioHandle, pin, microseconds);
    if(ret != 0)
        throw GPIOException("Failed to set pulsewidth: "  + std::string(pigpio_error(ret))); 
}
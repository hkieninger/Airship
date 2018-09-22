#include "gpio_exception.h"
#include "pin_dev.h"

void PinDevice::PinDevice(int pin) : pin(pin) {
    if(GpioDevice::gpioHandle < 0)
        throw GPIOException("GPIO wasn't initialised.");
}

void PinDevice::setPinMode(unsigned int mode) {
    if(set_mode(GpioDevice::gpioHandle, pin, mode) != 0)
        throw GPIOException("Failed to set pin mode.");
}

int PinDevice::readPin() {
    int ret = gpio_read(GpioDevice::gpioHandle, pin);
    if(ret == PI_BAD_GPIO)
        throw GPIOException("Failed to read pin.");
    return ret;
}

void PinDevice::writePin(unsigned int level) {
    if(gpio_write(GpioDevice::gpioHandle, pin, level) != 0)
        throw GPIOException("Failed to write pin.");
}

void PinDevice::setPulsewidth(unsigned int microseconds) {
    if(set_servo_pulsewidth(GpioDevice::gpioHandle, pin, width) != 0)
        throw GPIOException("Failed to set pulsewidth."); 
}
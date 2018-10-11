#include "gpio_exception.h"
#include "pin_dev.h"

PinDevice::PinDevice(int pin) : pin(pin) {}

void PinDevice::setPinMode(unsigned int mode) {
    int ret = gpioSetMode(pin, mode);
    if(ret != 0)
        throw GPIOException("Failed to set pin mode: BCM" + pin);
}

int PinDevice::readPin() {
    int ret = gpioRead(pin);
    if(ret == PI_BAD_GPIO)
        throw GPIOException("Failed to read pin: BCM" + pin);
    return ret;
}

void PinDevice::writePin(unsigned int level) {
    int ret = gpioWrite(pin, level);
    if(ret != 0)
        throw GPIOException("Failed to write pin: BCM" + pin);
}

void PinDevice::setPulsewidth(unsigned int microseconds) {
    int ret = gpioServo(pin, microseconds);
    if(ret != 0)
        throw GPIOException("Failed to set pulsewidth: BCM" + pin);
}

void PinDevice::triggerPin(unsigned int pulse_length, unsigned int level) {
    if(gpioTrigger(pin, pulse_length, level) != 0)
        throw GPIOException("Failed to trigger: BCM" + pin);
}
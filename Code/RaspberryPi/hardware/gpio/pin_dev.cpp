#include <string>

#include "gpio_exception.h"
#include "pin_dev.h"

PinDevice::PinDevice(int pin) : pin(pin) {}

void PinDevice::setPinMode(unsigned int mode) {
    int ret = gpioSetMode(pin, mode);
    if(ret != 0)
        throw GPIOException("Failed to set pin mode: BCM" + std::to_string(pin));
}

int PinDevice::readPin() {
    int ret = gpioRead(pin);
    if(ret == PI_BAD_GPIO)
        throw GPIOException("Failed to read pin: BCM" + std::to_string(pin));
    return ret;
}

void PinDevice::writePin(unsigned int level) {
    int ret = gpioWrite(pin, level);
    if(ret != 0)
        throw GPIOException("Failed to write pin: BCM" + std::to_string(pin));
}

void PinDevice::setPulsewidth(unsigned int microseconds) {
    int ret = gpioServo(pin, microseconds);
    if(ret != 0)
        throw GPIOException("Failed to set pulsewidth: BCM" + std::to_string(pin));
}

void PinDevice::triggerPin(unsigned int pulse_length, unsigned int level) {
    if(gpioTrigger(pin, pulse_length, level) != 0)
        throw GPIOException("Failed to trigger: BCM" + std::to_string(pin));
}

void PinDevice::helperFunc(int pin, int level, uint32_t tick, void *interruptInterfaceInstance) {
    InterruptInterface *interface = (InterruptInterface *) interruptInterfaceInstance;
    interface->onInterrupt(pin, level, tick);
}

void PinDevice::registerInterruptFunction(unsigned int edge, interrupt_function function) {
    if(gpioSetISRFunc(pin, edge, 0, function) != 0)
        throw GPIOException("Failed to register interrupt routine: BCM" + std::to_string(pin));
}

void PinDevice::registerInterruptMethod(unsigned int edge, InterruptInterface *interface) {
    if(gpioSetISRFuncEx(pin, edge, 0, &PinDevice::helperFunc, interface) != 0)
        throw GPIOException("Failed to register interrupt routine: BCM" + std::to_string(pin));
}

void PinDevice::unregisterInterrupt() {
    if(gpioSetISRFunc(pin, 0, 0, NULL) != 0)
        throw GPIOException("Failed to unregister interrupt routine: BCM" + std::to_string(pin));
}
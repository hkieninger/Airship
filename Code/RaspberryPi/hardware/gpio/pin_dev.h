#ifndef PIN_DEVICE_H
#define PIN_DEVICE_H 1

#include <pigpio.h>
#include "gpio_dev.h"

/*
 * constants for the pin mode, for setPinMode()
 */
#define PIN_INPUT PI_INPUT
#define PIN_OUTPUT PI_OUTPUT
#define PIN_ALT0 PI_ALT0
#define PIN_ALT1 PI_ALT1
#define PIN_ALT2 PI_ALT2
#define PIN_ALT3 PI_ALT3
#define PIN_ALT4 PI_ALT4
#define PIN_ALT5 PI_ALT5

/*
 * constants for the pin level, for readPin() and writePin()
 */
#define HIGH 1
#define LOW 0

/*
 * constants for edge, for registerInterrupt...()
 */
#define EDGE_RISING RISING_EDGE
#define EDGE_FALLING FALLING_EDGE
#define EDGE_BOTH EITHER_EDGE

typedef void (*interrupt_function) (int pin, int level, uint32_t tick);

class InterruptInterface {
public:
    virtual void onInterrupt(int pin, int level, uint32_t tick) = 0;
};

class PinDevice: private GpioDevice {
    int pin;
    static void helperFunc(int pin, int level, uint32_t tick, void *interruptInterfaceInstance);
public:
    PinDevice(int pin);

    /*
     * sets the pin mode
     * @mode: one of the constants defined above
     */
    void setPinMode(unsigned int mode);

    /*
     * @return: the level on that pin, either HIGH = 1 or LOW = 0
     */
    int readPin();

    /*
     * sets the @level of the pin to HIGH = 1 or LOW = 0
     */
    void writePin(unsigned int level);

    /*
     * set pulses with a width of @microseconds in a frequency of 50Hz
     * useful to control servos and escs
     * @microseconds: the pulsewidth in microseconds, a value between 500 and 2500 or 0
     */
    void setPulsewidth(unsigned int microseconds);

    /*
     * generate a short pulse of @level and switches than back to not @level
     * @pulse_length: the duration of the pulse in microseconds (1us - 100us)
     * @level: HIGH = 1 or LOW = 0
     */
    void triggerPin(unsigned int pulse_length, unsigned int level);

    void registerInterruptFunction(unsigned int edge, interrupt_function function);
    void registerInterruptMethod(unsigned int edge, InterruptInterface *interface);
    void unregisterInterrupt();
};

#endif /* PIN_DEVICE_H */
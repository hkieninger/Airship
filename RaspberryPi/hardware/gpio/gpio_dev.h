#ifndef GPIO_DEV_H
#define GPIO_DEV_H 1

class GpioDevice {
public:
    static void initialiseGpio();
    static void terminateGpio();
};

#endif /* GPIO_DEV_H */
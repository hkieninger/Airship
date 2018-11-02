#ifndef GPIO_DEV_H
#define GPIO_DEV_H 1

#include <signal.h>

class GpioDevice {
public:
    static void initialiseGpio();
    static void terminateGpio();
    static void setSignalHandler(int signum, sighandler_t sighandler);
};

#endif /* GPIO_DEV_H */
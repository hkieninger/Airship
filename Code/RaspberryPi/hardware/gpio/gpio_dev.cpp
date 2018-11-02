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

void GpioDevice::setSignalHandler(int signum, sighandler_t signalhandler) {
    if(gpioSetSignalFunc(signum, signalhandler) != 0)
        throw GPIOException("Failed to set signal handler.");
}

/*void GpioDevice::setSignalHandler(int signum, sighandler_t signalhandler) {
    struct sigaction sigact;
    memset(&sigact, 0, sizeof(sigact));
    sigact.sa_handler = signalhandler;
    sigemptyset (&sigact.sa_mask);
    sigact.sa_flags = 0; //causes system calls to return -1 with errno set to EINTR, SA_RESTART flag would disable this
    if(sigaction (sig, &sigact, NULL) < 0)
        throw std::runtime_error(std::string("failed to set signal handler: ") + strerror(errno));
}*/
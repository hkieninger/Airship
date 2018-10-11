#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <pigpio.h>
#include <string>

#include "thread/interrupted_exception.h"
#include "control/connection.h"
#include "control/control_thread.h"

static void signalHandler(int sig) {
    printf("Signal \" %s \" has occured.\n", strsignal(sig));
}

/*static void setSignalHandler(int sig, sighandler_t signalhandler) {
    struct sigaction sigact;
    memset(&sigact, 0, sizeof(sigact));
    sigact.sa_handler = signalhandler;
    sigemptyset (&sigact.sa_mask);
    sigact.sa_flags = 0; //causes system calls to return -1 with errno set to EINTR, SA_RESTART flag would disable this
    if(sigaction (sig, &sigact, NULL) < 0)
        throw std::runtime_error(std::string("failed to set signal handler: ") + strerror(errno));
}*/

int main() {
    GpioDevice::initialiseGpio();
    //setup signal handling
    if(gpioSetSignalFunc(SIGINT, &signalHandler) != 0) //setSignalHandler(SIGINT, &signalHandler); pigpio is invasive
        return EXIT_FAILURE;
    //thread managing the hardware and controlling the airship
    ControlThread controlThread;
    controlThread.start();
    try {
        controlThread.getConnection().loop();
    } catch(const InterruptedException &e) {
        printf("Programm has been interrupted. Terminating ...\n");
    }
    controlThread.stopRunning();
    controlThread.join();
    GpioDevice::terminateGpio();
    return EXIT_SUCCESS;
}
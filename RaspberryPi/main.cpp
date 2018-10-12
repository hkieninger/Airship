#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>

#include "thread/interrupted_exception.h"
#include "control/connection.h"
#include "control/control_thread.h"

static void signalHandler(int sig) {
    printf("Signal \" %s \" has occured.\n", strsignal(sig));
}

int main() {
    GpioDevice::initialiseGpio();
    //setup signal handling
    GpioDevice::setSignalHandler(SIGINT, signalHandler);

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
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <pthread.h>

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
    //set the name of the main thread for debugging purposes
    pthread_setname_np(pthread_self(), "main");

    //thread managing the hardware and controlling the airship
    ControlThread controlThread;
    controlThread.start();
    //loop managing the connections
    try {
        controlThread.getConnection().loop();
    } catch(const InterruptedException &e) {
        printf("Programm has been interrupted. Terminating ...\n");
    }
    //stop the control thread
    controlThread.stopRunning();
    controlThread.join();
    //release resources
    GpioDevice::terminateGpio();
    return EXIT_SUCCESS;
}
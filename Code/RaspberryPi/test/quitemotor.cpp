#include "../hardware/motor/motor.h"
#include "../control/pin.h"
#include "../hardware/gpio/gpio_dev.h"
#include <unistd.h>

int main() {
    GpioDevice::initialiseGpio();
    Motor ml(LEFT_MOTOR_ESC, LEFT_MOTOR_RELAIS);
    Motor mr(RIGHT_MOTOR_ESC, RIGHT_MOTOR_RELAIS);
    while(1) {
        sleep(60);
    }
    GpioDevice::terminateGpio();
    return 1;
}
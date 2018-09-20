#include <unistd.h>
#include <pigpio.h>

int main() {
    gpioInitialise();
    gpioWrite(17, 1);
    sleep(30);
    gpioWrite(17, 0);
    return 0;
}
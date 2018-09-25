#include <unistd.h>
#include <pigpiod_if2.h>

int main() {
    gpioInitialise();
    gpioWrite(17, 1);
    sleep(30);
    gpioWrite(17, 0);
    return 0;
}
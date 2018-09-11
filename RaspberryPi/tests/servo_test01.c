#include <pigpio.h>
#include <unistd.h>

#define PIN 27
#define ANGLE 90 /* 0 - 180 */

int main() {
	if(gpioInitialise() == PI_INIT_FAILED)
		return 1;
	if(gpioServo(PIN, ANGLE * 2000 / 180 + 500) != 0)
		return 1;
	sleep(10);
	gpioTerminate();
	return 0;
}

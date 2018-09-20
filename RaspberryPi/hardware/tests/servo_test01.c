#include <pigpio.h>
#include <unistd.h>

#define PIN 9 //25, 8
#define ANGLE 90 /* 0 - 180 */

int main() {
	if(gpioInitialise() == PI_INIT_FAILED)
		return 1;
	for(int i = 45; i < 135; i++) {
		if(gpioServo(PIN, i * 2000 / 180 + 500) != 0)
			return 1;
		usleep(500 * 1000);
	}
	gpioTerminate();
	return 0;
}

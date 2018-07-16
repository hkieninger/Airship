#include <stdio.h>
#include <unistd.h>
#include <wiringPi.h>

#include "../interfaces/observer.h"
#include "../mpu6050/mpu6050.h"

#define INTERRUPT_PIN 7

int main() {
    wiringPiSetup();

    class : public Observer {

        void update(/*Observable *observable, */void *data) {
            struct Mpu6050Data *mdata = (struct Mpu6050Data *) data;
            printf("accel x %.2f m/s^2\n", mdata->accel[0]);
            printf("accel y %.2f m/s^2\n", mdata->accel[1]);
            printf("accel z %.2f m/s^2\n", mdata->accel[2]);
            printf("gyro x %.2f m/s^2\n", mdata->gyro[0]);
            printf("gyro y %.2f m/s^2\n", mdata->gyro[1]);
            printf("gyro z %.2f m/s^2\n", mdata->gyro[2]);
        }

    } observer;

    Mpu6050 mpu(INTERRUPT_PIN);
    mpu.addObserver(&observer);

    sleep(10);

    return 0;
}
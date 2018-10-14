#include "../control/thread/jpg_camera_thread.h"
#include <stdio.h>

int main() {
    JpgCameraThread thread("/dev/video1", 320, 240, 0xCCCB);
    thread.start();
    thread.join();
    return 0;
}
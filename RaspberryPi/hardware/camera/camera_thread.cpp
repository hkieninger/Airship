#include "camera_thread.h"

CameraThread::CameraThread() {

}

void CameraThread::run() {
    while(running) {
        int bufIndex = cam.dequeueBuffer();
    }
}
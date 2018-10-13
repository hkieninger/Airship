#include <stdint.h>
#include <unistd.h>

#include "camera_thread.h"

CameraThread::CameraThread(Connection &connection) : 
    cam("/dev/video0", V4L2_PIX_FMT_H264, 640, 480), 
    connection(connection),
    running(true) {}

CameraThread::~CameraThread() {}

void CameraThread::stopRunning() {
    running = false;
}

#include <stdio.h>

void CameraThread::run() {
    struct Paket paket;
    paket.device = Measurement::SENSOR;
    paket.param = Measurement::CAM_BOTTOM;
    while(running) {
        if(connection.isConnected() && !connection.isLost()) {
            printf("sending paket.\n");
            video_buffer *buf = cam.dequeueBuffer();
            paket.data = (uint8_t *) buf->ptr;
            paket.len = buf->info.bytesused;
            connection.sendPaket(paket);
            cam.queueBuffer(buf);
        } else {
            sleep(1);
        }
    }
}
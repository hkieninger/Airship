#include "camera_thread.h"

CameraThread::CameraThread(Connection &connection) : 
    cam("/dev/video0", V4L2_PIX_FMT_H264, 640, 480), 
    connection(connection),
    running(true) {}

void CameraThread::stopRunning() {
    running = false;
}

void CameraThread::run() {
    struct Paket paket;
    paket.device = Measurement::SENSOR;
    paket.param = Measurement::CAM_BOTTOM;
    while(running) {
        video_buffer *buf = cam.dequeueBuffer();
        paket.data = buf->ptr;
        paket.len = buf->info.bytesused;
        connection.sendPaket(paket);
        cam.queueBuffer(buf);
    }
}
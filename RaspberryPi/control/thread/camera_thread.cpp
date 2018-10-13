#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "../../socket/socket.h"
#include "../../socket/socket_exception.h"
#include "../../thread/interrupted_exception.h"
#include "camera_thread.h"

#define ACCEPT_TIMEOUT (1 * 1000)

CameraThread::CameraThread() : 
    server(CAMERA_PORT),
    running(true) {}

CameraThread::~CameraThread() {}

void CameraThread::stopRunning() {
    running = false;
}

void CameraThread::run() {
    try {
        //loop for accepting incoming connections
        while(running) { //exited when interrupted exception is thrown
            try {
                Socket sock = server.acceptConnection(ACCEPT_TIMEOUT);
                Camera cam("/dev/video0", V4L2_PIX_FMT_H264, 640, 480);
                printf("Camera stream: client %s has connected\n", sock.getRemoteIPString().c_str());
                try {
                    //loop for handling connections
                    while(true) { //exited when socket (closed) exception is thrown
                        video_buffer *buf = cam.dequeueBuffer();
                        sock.sendAll(buf->ptr, buf->info.bytesused);
                        cam.queueBuffer(buf);
                    }
                } catch(const SocketClosedException &e) {
                    printf("Camera stream: connection to camera stream has been closed by client.\n");
                } catch(const SocketException &e) {
                    fprintf(stderr, "Camera stream: the following socket exception has occured: %s\n", e.what());
                }
            } catch(const TimeoutException &t) {}
        }
    } catch(const InterruptedException &e) {
        printf("Camera stream: has been interrupted.\n");
    }
}
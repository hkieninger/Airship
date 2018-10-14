#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>

#include "../../socket/socket_exception.h"
#include "../../thread/interrupted_exception.h"
#include "camera_thread.h"

#define ACCEPT_TIMEOUT (1 * 1000)

CameraThread::CameraThread(const std::string &device, uint32_t format, uint32_t width, uint32_t height, uint16_t port) : 
    server(port),
    running(true),
    device(device),
    format(format),
    width(width),
    height(height) {}

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
                Camera cam(device.c_str(), format, width, height); //create every time a new instance that the header is send again for the case format is h264
                printf("Camera stream (port %x): client %s has connected\n", server.getPort(), sock.getRemoteIPString().c_str());
                try {
                    //loop for handling connections
                    while(true) { //exited when socket (closed) exception is thrown
                        video_buffer *buf = cam.dequeueBuffer();
                        sendFrame(sock, buf, cam.getWidth(), cam.getHeight());
                        cam.queueBuffer(buf);
                    }
                } catch(const SocketClosedException &e) {
                    printf("Camera stream (port %x): connection has been closed by client.\n", server.getPort());
                } catch(const SocketException &e) {
                    fprintf(stderr, "Camera stream (port %x): the following socket exception has occured: %s\n", server.getPort(), e.what());
                }
            } catch(const TimeoutException &t) {}
        }
    } catch(const InterruptedException &e) {
        printf("Camera stream (port %x): has been interrupted.\n", server.getPort());
    }
}

void CameraThread::sendFrame(Socket &sock, video_buffer *buf, uint32_t width, uint32_t height) {
    sock.sendAll(buf->ptr, buf->info.bytesused);
}
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "../hardware/camera/camera.h"
#include "../socket/server_socket.h"
#include "../socket/socket.h"
#include "../socket/socket_exception.h"

static uint64_t micros() {
    struct timeval tv;
    if(gettimeofday(&tv, NULL) < 0)
        exit(1);
    return 1000 * 1000 * tv.tv_sec + tv.tv_usec;
}

int main() {
    ServerSocket server(0xCCCC);
    int64_t bytes = 0;
    int64_t frames = 0;
    uint64_t start = micros();
    //loop for accepting incoming connections
    while(true) { //exited when interrupted exception is thrown
        Camera cam("/dev/video0", V4L2_PIX_FMT_H264, 640, 480);
        Socket sock = server.acceptConnection();
        printf("client %s has connected\n", sock.getRemoteIPString().c_str());
        try {
            //loop for handling connections
            while(true) { //exited when socket (closed) exception is thrown
                video_buffer *buf = cam.dequeueBuffer();
                sock.sendAll(buf->ptr, buf->info.bytesused);
                bytes += buf->info.bytesused;
                frames++;
                cam.queueBuffer(buf);
                if(micros() - start > 3 * 1000 * 1000) {
                    uint64_t t = micros() - start;
                    t /= 1000;
                    printf("frames: %lld, bytes: %lld, millis: %llu\n", frames, bytes, t);
                    printf("fps: %lld, byterate: %lld\n", frames * 1000 / t, bytes * 1000 / t);
                    frames = 0;
                    bytes = 0;
                    start = micros();
                }
            }
        } catch(const SocketClosedException &e) {
            printf("Connection has been closed by client.\n");
        } catch(const SocketException &e) {
            fprintf(stderr, "The following socket exception has occured: %s\n", e.what());
        }
    }
    return 0;
}
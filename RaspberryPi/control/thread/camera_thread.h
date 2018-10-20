#ifndef CAMERA_THREAD_H
#define CAMERA_THREAD_H 1

#include <string>
#include <stdint.h>

#include "../../thread/thread.h"
#include "../../hardware/camera/camera.h"
#include "../../socket/server_socket.h"
#include "../../socket/socket.h"
#include "../connection.h"

class CameraThread: public Thread {
    ServerSocket server;
    bool running, pausing;
    const std::string device;
    const uint32_t format, width, height;
    Connection &connection;
public:
    CameraThread(const std::string &device, uint32_t format, uint32_t width, uint32_t height, uint16_t port, Connection &connection);
    virtual ~CameraThread();
    void stopRunning();
    void setPausing(bool on);
    virtual void run();
    virtual void sendFrame(Socket &sock, video_buffer *buf, uint32_t width, uint32_t height);
};

#endif /* CAMERA_THREAD_H */
#ifndef CAMERA_THREAD_H
#define CAMERA_THREAD_H 1

#include <string>

#include "../../thread/thread.h"
#include "../../hardware/camera/camera.h"
#include "../../socket/server_socket.h"

#define CAMERA_PORT 0xCCCD

class CameraThread: public Thread {
    ServerSocket server;
    bool running;
public:
    CameraThread();
    virtual ~CameraThread();
    void stopRunning();
    virtual void run();
};

#endif /* CAMERA_THREAD_H */
#ifndef CAMERA_THREAD_H
#define CAMERA_THREAD_H 1

#include <string>

#include "../connection.h"
#include "../../thread/thread.h"
#include "../../hardware/camera/camera.h"

class CameraThread: public Thread {
    Camera cam;
    Connection &connection;
    bool running;
public:
    CameraThread(Connection &connection);
    void stopRunning();
    virtual void run();
};

#endif /* CAMERA_THREAD_H */
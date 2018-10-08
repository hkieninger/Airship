#ifndef CAMERA_THREAD_H
#define CAMERA_THREAD_H 1

#include "../../thread/thread.h"

class CameraThread: public Thread {
    Camera &cam;
    Connection &connection;
public:
    CameraThread(const std::string &camera);
    virtual void run();
};

#endif /* CAMERA_THREAD_H */
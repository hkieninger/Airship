#ifndef JPG_CAMERA_THREAD_H
#define JPG_CAMERA_THREAD_H 1

#include "camera_thread.h"

class JpgCameraThread: public CameraThread {
    unsigned char *jpeg_buffer;
    unsigned long jpeg_buffer_size;
public:
    JpgCameraThread(const std::string &device, uint32_t width, uint32_t height, uint16_t port, Connection &connection);
    virtual ~JpgCameraThread();
    virtual void sendFrame(Socket &sock, video_buffer *buf, uint32_t width, uint32_t height);
};

#endif /* JPG_CAMERA_THREAD_H */
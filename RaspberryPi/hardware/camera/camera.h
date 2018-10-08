#ifndef CAMERA_H
#define CAMERA_H

#include <string>
#include <deque>

#include <stdint.h>
#include <linux/videodev2.h>

#define DEFAULT_DEVICE "/dev/video0"
#define DEFAULT_WIDTH 640
#define DEFAULT_HEIGHT 480
#define DEFAULT_FORMAT V4L2_PIX_FMT_YUYV

#define BUFFER_COUNT 2

class Camera {
    int fd;

    int buffer_count;
    void *buffer[BUFFER_COUNT];
    struct v4l2_buffer bufferinfo[BUFFER_COUNT];
    std::deque<int> buffer_order;

    const std::string device_file;
    uint32_t width, height, format;

    void munmapBuffers();
public:
    /*
     * you may want to check which formats are supported by your camera with 
     * $ v4l2-ctl -d /dev/video0 --list-formats-ext
     * @device: the device node in the filesystem
     * @format: the format of the frames, one of the constants from <linux/videodev2.h>
     * @width: the next fitting width will be used
     * @height: the next fitting height will be used
     */
    Camera(const std::string &device = DEFAULT_DEVICE, 
        uint32_t format = DEFAULT_FORMAT, 
        uint32_t width = DEFAULT_WIDTH, uint32_t height = DEFAULT_HEIGHT);

    /*
     * realeases all the resources (buffers, filedescriptor)
     */
    ~Camera();

    /*
     * blocks until the next buffer in the queue is filled
     * @return the index of the filled buffer
     */
    int dequeueBuffer();
    
    /*
     * @buffer: the index of the buffer to be queued, must be dequeued beforehand with dequeueBuffer()
     */
    void queueBuffer(int buf);

    /*
     * getters for the private fields
     */

    void *getBuffer(int buf) {
        return buffer[buf];
    }

    uint32_t getBufferCount() {
        return buffer_count;
    }

    uint32_t getBufferLength() {
        return bufferinfo[0].length;
    }

    const std::string &getDeviceFile() {
        return device_file;
    }

    uint32_t getWidth() {
        return width;
    }

    uint32_t getHeight() {
        return height;
    }

    uint32_t getFormat() {
        return format;
    }
};

#endif /* CAMERA_H */
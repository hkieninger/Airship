#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <string.h>
#include <errno.h>
#include <stdexcept>
#include <stdio.h>
#include <stdlib.h>

#include "camera.h"

/*
 * the implementation of the class is based on following tutorial
 * http://jwhsmith.net/2014/12/capturing-a-webcam-stream-using-v4l2/
 * and following example
 * https://github.com/pyrovski/h264-v4l2-capture/blob/master/capture.c
 */

void Camera::munmapBuffers() {
    for(int i = 0; i < buffer_count; i++) {
        if(munmap(buffer[i], buffer_length[i]) < 0)
            fprintf(stderr, "camera %s: failed to unmap buffer: %s\n", device_file.c_str(), strerror(errno));
    }
}

Camera::Camera(const std::string &device, uint32_t format, uint32_t width, uint32_t height) : 
    device_file(device), buffer_count(0) {
    try {
        //open device file
        fd = open(device.c_str(), O_RDWR);
        if(fd < 0)
            throw std::runtime_error("failed to open camera device: " + std::string(strerror(errno)));
        //query capabilities
        struct v4l2_capability cap;
        memset(&cap, 0, sizeof(cap));
        if(ioctl(fd, VIDIOC_QUERYCAP, &cap) < 0)
            throw std::runtime_error("failed to query device capabilities: " + std::string(strerror(errno)));
        if(!(cap.capabilities & V4L2_CAP_VIDEO_CAPTURE))
            throw std::runtime_error("device " + device + " doesn't support video capture");
        if(!(cap.capabilities & V4L2_CAP_STREAMING))
            throw std::runtime_error("device " + device + " doesn't support streaming");
        //set format and resolution
        struct v4l2_format fmt;
        memset(&fmt, 0, sizeof(fmt));
        fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        fmt.fmt.pix.pixelformat = format;
        fmt.fmt.pix.width = width;
        fmt.fmt.pix.height = height;
        fmt.fmt.pix.field = V4L2_FIELD_INTERLACED;
        if(ioctl(fd, VIDIOC_S_FMT, &fmt) < 0)
            throw std::runtime_error("failed to set format: " +  std::string(strerror(errno)));
        if(fmt.fmt.pix.pixelformat != format)
            throw std::runtime_error("device" + device + "doesn't support format");
        this->format = format;
        this->width = fmt.fmt.pix.width; //get the effective width
        this->height = fmt.fmt.pix.height; //get the effective height
        //request buffers
        struct v4l2_requestbuffers bufrequest;
        memset(&bufrequest, 0, sizeof(bufrequest));
        bufrequest.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        bufrequest.memory = V4L2_MEMORY_MMAP;
        bufrequest.count = BUFFER_COUNT;
        if(ioctl(fd, VIDIOC_REQBUFS, &bufrequest) < 0)
            throw std::runtime_error("failed to request buffers: " +  std::string(strerror(errno)));
        //loop over the buffers
        for(unsigned int i = 0; i < bufrequest.count; i++) {
            //query buffer properties
            struct v4l2_buffer info;
            memset(&info, 0, sizeof(info));
            info.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
            info.memory = V4L2_MEMORY_MMAP;
            info.index = i;
            if(ioctl(fd, VIDIOC_QUERYBUF, &info) < 0)
                throw std::runtime_error("failed to request buffers: " +  std::string(strerror(errno)));
            buffer_length[i] = info.length;
            //allocate the buffer
            buffer[i] = mmap(NULL, buffer_length[i], PROT_READ | PROT_WRITE, MAP_SHARED, fd, info.m.offset);
            if(buffer[i] == MAP_FAILED)
                throw std::runtime_error("failed to map buffer: " + std::string(strerror(errno)));
            memset(buffer[i], 0, buffer_length[i]);
            buffer_count++;
        }
        //queue the buffers
        for(unsigned int i = 0; i < buffer_count; i++) {
            struct v4l2_buffer info;
            memset(&info, 0, sizeof(info));
			info.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
			info.memory = V4L2_MEMORY_MMAP;
			info.index = i;
            if(ioctl(fd, VIDIOC_QBUF, &info) < 0)
                throw std::runtime_error("failed to queue buffer: " + std::string(strerror(errno)));
        }
        //activate streaming
	    enum v4l2_buf_type type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	    if(ioctl(fd, VIDIOC_STREAMON, &type) < 0)
		    throw std::runtime_error("failed to activate streaming: " + std::string(strerror(errno)));
    } catch(const std::runtime_error &e) {
        //free the buffers
        munmapBuffers();
        //close the file descriptor
        if(close(fd) < 0)
            fprintf(stderr, "camera %s: failed to close file descriptor: %s\n", device_file.c_str(), strerror(errno));
        throw;
    }
}

Camera::~Camera() {
    //deactivate streaming
	enum v4l2_buf_type type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	if(ioctl(fd, VIDIOC_STREAMOFF, &type) < 0)
        fprintf(stderr, "camera %s: failed to deactivate streaming: %s\n", device_file.c_str(), strerror(errno));
    //free the buffers
    munmapBuffers();
    //close the file descriptor
    if(close(fd) < 0)
        fprintf(stderr, "camera %s: failed to close file descriptor: %s\n", device_file.c_str(), strerror(errno));
}

video_buffer *Camera::dequeueBuffer() {
    struct video_buffer *buf = (video_buffer *) malloc(sizeof(struct video_buffer));
    if(buf == NULL)
        throw std::runtime_error("failed to allocate memory: " + std::string(strerror(errno)));
    memset(&buf->info, 0, sizeof(buf->info));
    buf->info.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    buf->info.memory = V4L2_MEMORY_MMAP;
    //the buffer is waiting in the outgoing queue.
	if(ioctl(fd, VIDIOC_DQBUF, &buf->info) < 0)
		throw std::runtime_error("failed to dequeue buffer: " + std::string(strerror(errno)));
    buf->ptr = buffer[buf->info.index];
    return buf;
}

void Camera::queueBuffer(video_buffer *buf) {
    //put the buffer in the incoming queue.
	if(ioctl(fd, VIDIOC_QBUF, &buf->info) < 0)
		throw std::runtime_error("failed to queue buffer: " + std::string(strerror(errno)));
    free(buf);
}
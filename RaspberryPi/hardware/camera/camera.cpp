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

#include "camera.h"

/*
 * the implementation of the class is based on following tutorial
 * http://jwhsmith.net/2014/12/capturing-a-webcam-stream-using-v4l2/
 */

void Camera::munmapBuffers() {
    for(int i = 0; i < buffer_count; i++) {
        if(munmap(buffer[i], bufferinfo[i].length) < 0)
            fprintf(stderr, "camera %s: failed to unmap buffer: %s\n", device_file.c_str(), strerror(errno));
    }
}

Camera::Camera(const std::string &device, uint32_t format, uint32_t width, uint32_t height) : device_file(device) {
    buffer_count = 0;
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
        struct v4l2_format format_struct;
        memset(&format_struct, 0, sizeof(format_struct));
        format_struct.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        format_struct.fmt.pix.pixelformat = format;
        format_struct.fmt.pix.width = width;
        format_struct.fmt.pix.height = height;
        if(ioctl(fd, VIDIOC_S_FMT, &format_struct) < 0)
            throw std::runtime_error("failed to set format: " +  std::string(strerror(errno)));
        if(format_struct.fmt.pix.pixelformat != format)
            throw std::runtime_error("device" + device + "doesn't support format");
        this->format = format;
        this->width = format_struct.fmt.pix.width; //get the effective width
        this->height = format_struct.fmt.pix.height; //get the effective height
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
            memset(&bufferinfo[i], 0, sizeof(bufferinfo[i]));
            bufferinfo[i].type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
            bufferinfo[i].memory = V4L2_MEMORY_MMAP;
            bufferinfo[i].index = i;
            if(ioctl(fd, VIDIOC_QUERYBUF, &bufferinfo[i]) < 0)
                throw std::runtime_error("failed to request buffers: " +  std::string(strerror(errno)));
            //allocate the buffer
            buffer[i] = mmap(NULL, bufferinfo[i].length, PROT_READ | PROT_WRITE, MAP_SHARED, fd, bufferinfo[i].m.offset);
            if(buffer[i] == MAP_FAILED)
                throw std::runtime_error("failed to map buffer: " + std::string(strerror(errno)));
            memset(buffer[i], 0, bufferinfo[i].length);
            buffer_count++;
        }
        //activate streaming (may first queue buffers in case it doesn't work)
	    int type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	    if(ioctl(fd, VIDIOC_STREAMON, &type) < 0)
		    throw std::runtime_error("failed to activate streaming: " + std::string(strerror(errno)));
        //queue the buffers
        for(int i = 0; i < buffer_count; i++) {
            queueBuffer(i);
        }
    } catch(const std::runtime_error &e) {
        //close the file descriptor
        if(close(fd) < 0)
            fprintf(stderr, "camera %s: failed to close file descriptor: %s\n", device_file.c_str(), strerror(errno));
        //free the buffers
        munmapBuffers();
        throw;
    }
}

Camera::~Camera() {
    //deactivate streaming
	int type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	if(ioctl(fd, VIDIOC_STREAMOFF, &type) < 0)
        fprintf(stderr, "camera %s: failed to deactivate streaming: %s\n", device_file.c_str(), strerror(errno));
    //close the file descriptor
    if(close(fd) < 0)
        fprintf(stderr, "camera %s: failed to close file descriptor: %s\n", device_file.c_str(), strerror(errno));
    //free the buffers
    munmapBuffers();
}

int Camera::dequeueBuffer() {
    if(buffer_order.empty())
        throw std::runtime_error("no buffer in the outgoing queue");
    int buf = buffer_order.front();
    //the buffer is waiting in the outgoing queue.
	if(ioctl(fd, VIDIOC_DQBUF, &bufferinfo[buf]) < 0)
		throw std::runtime_error("failed to dequeue buffer: " + std::string(strerror(errno)));
    buffer_order.pop_front();
    return buf;
}

void Camera::queueBuffer(int buf) {
    //put the buffer in the incoming queue.
	if(ioctl(fd, VIDIOC_QBUF, &bufferinfo[buf]) < 0)
		throw std::runtime_error("failed to queue buffer: " + std::string(strerror(errno)));
    buffer_order.push_back(buf);
}
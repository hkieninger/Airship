#include "../camera/camera.h"
#include <stdio.h>
#include <linux/videodev2.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int main() {
    Camera cam("/dev/video0", V4L2_PIX_FMT_H264, 640, 480);
    int fd = open("test.h264", O_WRONLY | O_CREAT, 0644);
    if(fd < 0)
        return 1;
    for(int i = 0; i < 300; i++) {
        video_buffer *buf = cam.dequeueBuffer();
        write(fd, buf->ptr, buf->info.bytesused);
        cam.queueBuffer(buf);
    }
    close(fd);
}
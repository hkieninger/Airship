#include "picture.h"
#include "webcam.h"
#include <stdlib.h>

static void yuv2rgb_info(const struct picture_info *yuv_info, struct picture_info *rgb_info) {
	rgb_info->width = yuv_info->width;
	rgb_info->height = yuv_info->height;
	rgb_info->size = yuv_info->size * 3 / 2;
}

int main() {
    webcam cam("/dev/video0", 640, 480);
    
    struct picture_info yuv_info = cam.picture_information();
    struct picture_info info;
    yuv2rgb_info(&yuv_info, &info);
    unsigned char *rgb_buffer = (unsigned char *) malloc(info.size);

    cam.queue_buffer();
    printf("buffer queued");
    cam.dequeue_buffer();
    printf("buffer dequeued");
    
    yuv2rgb(&yuv_info, cam.picture_data(), rgb_buffer);
    write_ppm("test.ppm", &info, rgb_buffer);

    cam.close_webcam();
    free(rgb_buffer);
}

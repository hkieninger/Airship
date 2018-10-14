#include <stdlib.h>
#include <stdio.h>
#include <jpeglib.h>
#include <stdexcept>
#include <byteswap.h>

#include "jpg_camera_thread.h"

/* https://stackoverflow.com/questions/19857766/error-handling-in-libjpeg */
static void jpeg_error_exit (j_common_ptr cinfo){
    char jpeg_last_error_msg[JMSG_LENGTH_MAX];
    (*( cinfo->err->format_message )) (cinfo, jpeg_last_error_msg);
    throw std::runtime_error(jpeg_last_error_msg);
}

/* creates an jpeg picture from a yuyv picture 
   code is based on: 
       -https://github.com/LuaDist/libjpeg/blob/master/example.c
       -jdatadst.c in the libjpeg source code
       -https://gist.github.com/royshil/fa98604b01787172b270
   @width: width of the picture in pixel
   @height: height of the picture in pixel
   @yuyv: the buffer to read the yuyv data from (arrangement as in buffer from V4L2_PIX_FMT_YUYV)
   @jpeg: a pointer to a buffer which is big enough to contain the jpeg picture
   if the buffer is too small or jpeg points to NULL a new buffer, which is big enough will be allocated
   the new buffer (and the buffer which is too small) should be freed with free afterwards
   @size: the size of the jpeg buffer in bytes, if the jpeg buffer points to NULL size should be 0
   @return: the size of the jpeg picture in bytes */
static unsigned long yuyv2jpg(uint32_t width, uint32_t height, unsigned char *yuyv, unsigned char **jpeg, unsigned long size) {
    struct jpeg_compress_struct cinfo;
	struct jpeg_error_mgr jerr;
	
	cinfo.err = jpeg_std_error(&jerr);
	jerr.error_exit = jpeg_error_exit; //throw exception instead of exiting programm
	jpeg_create_compress(&cinfo);
	
	jpeg_mem_dest(&cinfo, jpeg, &size);
	
	cinfo.image_width = width;
	cinfo.image_height = height;
	cinfo.input_components = 3;
	cinfo.in_color_space = JCS_YCbCr; 
	jpeg_set_defaults(&cinfo);
	
	jpeg_start_compress(&cinfo, TRUE);
	
    unsigned char tmprowbuf[cinfo.image_width * 3];
    JSAMPROW row_pointer[1];
    row_pointer[0] = &tmprowbuf[0];
    while(cinfo.next_scanline < cinfo.image_height) {
        unsigned long offset = cinfo.next_scanline * cinfo.image_width * 2;
        for(unsigned long i = 0, j = 0; i < cinfo.image_width * 2; i += 4, j += 6) { //input strides by 4 bytes, output strides by 6 (2 pixels)
            tmprowbuf[j + 0] = yuyv[offset + i + 0]; // Y (unique to this pixel)
            tmprowbuf[j + 1] = yuyv[offset + i + 1]; // U (shared between pixels)
            tmprowbuf[j + 2] = yuyv[offset + i + 3]; // V (shared between pixels)
            tmprowbuf[j + 3] = yuyv[offset + i + 2]; // Y (unique to this pixel)
            tmprowbuf[j + 4] = yuyv[offset + i + 1]; // U (shared between pixels)
            tmprowbuf[j + 5] = yuyv[offset + i + 3]; // V (shared between pixels)
        }
        jpeg_write_scanlines(&cinfo, row_pointer, 1);
    }
	
	jpeg_finish_compress(&cinfo);
	jpeg_destroy_compress(&cinfo);
	
	return size;
}

JpgCameraThread::JpgCameraThread(const std::string &device, uint32_t width, uint32_t height, uint16_t port) : 
    CameraThread(device, V4L2_PIX_FMT_YUYV, width, height, port), jpeg_buffer(NULL), jpeg_buffer_size(0) {}

JpgCameraThread::~JpgCameraThread() {}

void JpgCameraThread::sendFrame(Socket &sock, video_buffer *buf, uint32_t width, uint32_t height) {
    unsigned char *jpegbuf = jpeg_buffer;
    uint32_t size = yuyv2jpg(width, height, (unsigned char *) buf->ptr, &jpegbuf, jpeg_buffer_size);
    //has a new buffer for the jpeg picture been allocated
    if(size > jpeg_buffer_size) {
        free(jpeg_buffer);
        jpeg_buffer = jpegbuf;
        jpeg_buffer_size = size;
    }
    uint32_t sizeBigEndian = bswap_32(size);
    sock.sendAll(&sizeBigEndian, sizeof(uint32_t));
    sock.sendAll(jpegbuf, size);
}
#include <linux/videodev2.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <stdio.h>


/*
    Setting Format is quite simple, basically we use the struct v4l2_format 
    which has properties such as window size and pixel format. This function 
    allows for custom width and height inputs.
*/

int set_format(int fd, int width, int height) {
    struct v4l2_format format = {0};
    format.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    format.fmt.pix.width = width;
    format.fmt.pix.height = height;
    // may need to check here what format it should be
    format.fmt.pix.pixelformat = V4L2_PIX_FMT_YUYV;
    format.fmt.pix.field = V4L2_FIELD_NONE;
    int res = ioctl(fd, VIDIOC_S_FMT, &format);
    if( res == -1) {
        perror("Could not set format");
        exit(1);
    }
    return res;
}

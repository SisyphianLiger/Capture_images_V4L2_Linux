#include <linux/videodev2.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <stdio.h>
#include <errno.h>

/*
    Setting Format is quite simple, basically we use the struct v4l2_format 
    which has properties such as window size and pixel format. This function 
    allows for custom width and height inputs.
*/

/*@
  lemma res_success_or_failure:
    \forall int res; res == 0 || res == -1;
*/
/*@
    requires fd >= INT_MIN && fd <= INT_MAX;
    requires width >= SHRT_MIN && width <= SHRT_MAX;
    requires height >= SHRT_MIN && height <= SHRT_MAX;
    
    behavior success:
        ensures \result == 0;

    behavior failure:
        ensures \result == ENOEXEC;

    disjoint behaviors;
    complete behaviors;
    
*/

int set_format(int fd, unsigned short width, unsigned short height) {
    struct v4l2_format format = {0};
    format.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    //@ assert format.type == 65536; 
    format.fmt.pix.width = width;
    //@ assert format.type == 65536 && format.fmt.pix.width == width;
    format.fmt.pix.height = height;
    //@ assert format.type == 65536 && format.fmt.pix.width == width && format.fmt.pix.height == height;

    // may need to check here what format it should be
    format.fmt.pix.pixelformat = V4L2_PIX_FMT_YUYV;
    //@ assert format.type == 65536 && format.fmt.pix.width == width && format.fmt.pix.height == height && format.fmt.pix.pixelformat == V4L2_PIX_FMT_YUYV;
    format.fmt.pix.field = V4L2_FIELD_NONE;
    //@ assert format.type == 65536 && format.fmt.pix.width == width && format.fmt.pix.height == height && format.fmt.pix.pixelformat == V4L2_PIX_FMT_YUYV && format.fmt.pix.field == V4L2_FIELD_NONE;
    int res = ioctl(fd, VIDIOC_S_FMT, &format);
    //@ assert res == 0 || res == -1 &&format.type == 65536 && format.fmt.pix.width == width && format.fmt.pix.height == height && format.fmt.pix.pixelformat == V4L2_PIX_FMT_YUYV && format.fmt.pix.field == V4L2_FIELD_NONE;
    if( res == -1) {
    //@ assert res == -1 && format.type == 65536 && format.fmt.pix.width == width && format.fmt.pix.height == height && format.fmt.pix.pixelformat == V4L2_PIX_FMT_YUYV && format.fmt.pix.field == V4L2_FIELD_NONE;
        perror("Format unable to be set please check query capabilities and make sure pixel format is correctly specified");
        //@ assert res == -1 &&format.type == 65536 && format.fmt.pix.width == width && format.fmt.pix.height == height && format.fmt.pix.pixelformat == V4L2_PIX_FMT_YUYV && format.fmt.pix.field == V4L2_FIELD_NONE;
        fprintf(stderr, "Error Number: %d\n", ENOEXEC);
        //@ assert res == -1 &&format.type == 65536 && format.fmt.pix.width == width && format.fmt.pix.height == height && format.fmt.pix.pixelformat == V4L2_PIX_FMT_YUYV && format.fmt.pix.field == V4L2_FIELD_NONE;
        return ENOEXEC;
    }
    //@ assert res == 0 && format.type == 65536 && format.fmt.pix.width == width && format.fmt.pix.height == height && format.fmt.pix.pixelformat == V4L2_PIX_FMT_YUYV && format.fmt.pix.field == V4L2_FIELD_NONE;
    return res;
}

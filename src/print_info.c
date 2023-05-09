#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <linux/videodev2.h>
#include "../headers/xioctl.h"



int check_camera_capabilities(int fd){

    struct v4l2_capability caps = {};
    if ( -1 == xioctl(fd, VIDIOC_QUERYCAP, &caps)){
        perror("Querying Capabilities");
        return 1;
    }
  
    printf("Driver Caps: \n"
            " Driver: \"%s\"n"
            " Card: \"%s\"n"
            " Bus: \"%s\"n"
            " Version: %d.%d\n"
            " Capabilities: %08x\n",
            caps.driver,
            caps.card,
            caps.bus_info,
            (caps.version>>16)&&0xff,
            (caps.version>>24)&&0xff,
            caps.capabilities);

    return 0;
}

int check_crop_capabilities(int fd){
    
    struct v4l2_cropcap cropcap = {0};
    cropcap.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    if( -1 == xioctl(fd, VIDIOC_CROPCAP, &cropcap)){
        perror("Querying Cropping capabilities");
        return 1;
    }

    printf("Camera Cropping: \n"
            " Bounds: %dx%d+%d+%d\n"
            " Default: %dx%d+%d+%d\n"
            " Aspect: %d/%d\n",
            cropcap.bounds.width,
            cropcap.bounds.height,
            cropcap.bounds.left,
            cropcap.bounds.top,
            cropcap.defrect.width,
            cropcap.defrect.height,
            cropcap.defrect.left,
            cropcap.defrect.top,
            cropcap.pixelaspect.numerator,
            cropcap.pixelaspect.denominator);
    
    return 0;
}

int check_camera_format(int fd){
    int supports_GRBG10 = 0;
    struct v4l2_fmtdesc fmtdesc = {0};
    fmtdesc.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    
    char fourcc[5] = {0};
    char c, e;

    printf(" FMT : CE Desc\n--------------------------\n");

    while( 0 == xioctl(fd, VIDIOC_ENUM_FMT, &fmtdesc)) {
        strncpy(fourcc, (char *)&fmtdesc.pixelformat, 4);
        if(fmtdesc.pixelformat == V4L2_PIX_FMT_SGRBG10)
            supports_GRBG10 = 1;

        c = fmtdesc.flags & 1 ? 'C' : ' ';
        e = fmtdesc.flags & 2 ? 'E' : ' ';

        printf(" %s: %c%c %s\n", fourcc, c, e, fmtdesc.description);
        fmtdesc.index++;
    }

    if (!supports_GRBG10){
        printf("Doesn't Support GRBG10. \n");
        return 1;
    } 

    struct v4l2_format fmt = {0};
    fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    fmt.fmt.pix.width = 752;
    fmt.fmt.pix.height = 480;
    fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_SGRBG10; 
    fmt.fmt.pix.field = V4L2_FIELD_NONE;

    if ( -1 == xioctl(fd, VIDIOC_S_FMT, &fmt)){
        perror("Setting Pixel Format");
        return 1;
    }

    strncpy(fourcc, (char *)&fmt.fmt.pix.pixelformat, 4);
    printf( "Selected Camera Mode: \n"
            " Width: %d\n"
            " Height: %d\n"
            " PixFmt: %s\n"
            " Field: %d\n",
            fmt.fmt.pix.width,
            fmt.fmt.pix.height,
            fourcc,
            fmt.fmt.pix.field);

    return 0;

}

/* Test for Capabilities and if there is a capture available
   Print out the information in regard to the interface :)
   */
int print_caps(int fd){

    int success = 0;
    success += check_camera_capabilities(fd);
    success += check_crop_capabilities(fd); 
    success += check_camera_format(fd); 
    if (success > 0)
        success = -1;
    return success;
}

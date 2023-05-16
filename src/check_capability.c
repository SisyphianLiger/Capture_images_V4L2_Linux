#include <linux/videodev2.h>
#include <sys/ioctl.h>
#include <stdio.h>
#include <stdlib.h>


/*
 * query_capabilities checks that the file in question has the capability to 
 * take a picture. The most important check here is V4L2_CAP_STREAMING, because
 * if verifyied true, it will be used as the capture device. 
 */

void query_capabilities(int fd) {
    
    struct v4l2_capability capability;

    if( -1 == ioctl(fd, VIDIOC_QUERYCAP, &capability)){
        perror("Query Capabilities");
        exit(EXIT_FAILURE);
    }

    if (!(capability.capabilities & V4L2_CAP_VIDEO_CAPTURE)){
        fprintf(stderr, "Device is no video capture device \\n");
        exit(EXIT_FAILURE);
    }

    if (!(capability.capabilities & V4L2_CAP_READWRITE))
        fprintf(stderr, "Device does not support read i/o \\n");
    
    if (!(capability.capabilities & V4L2_CAP_STREAMING))
        fprintf(stderr, "Device does not support streaming \\n");

}

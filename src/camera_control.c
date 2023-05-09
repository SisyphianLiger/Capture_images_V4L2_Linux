#include <errno.h>
#include <linux/videodev2.h>
#include "../headers/xioctl.h"
#include <string.h>
#include <stdio.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <stdint.h>
#include <unistd.h>


uint8_t * buffer_ptr;

int init_mmap(int fd){
    struct v4l2_requestbuffers request = {0};
    
    request.count = 1;
    request.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    request.memory = V4L2_MEMORY_MMAP;

    if ( -1 == xioctl(fd, VIDIOC_REQBUFS, &request)){
	printf("Errno: %d/n", errno);
        perror("Requesting Buffer");
        return 1;
    }
    
    struct v4l2_buffer buffer = {0};
    buffer.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    buffer.memory = V4L2_MEMORY_MMAP;
    buffer.index = 0;
    
    
    if ( -1 == xioctl(fd, VIDIOC_QBUF, &buffer)){
	printf("Errno: %d/n", errno);
        perror("Querying Buffer");
        return 1;
    }
    void * buffer_start = mmap(NULL, buffer.length, PROT_READ | PROT_WRITE, MAP_SHARED, fd, buffer.m.offset); 
     
    printf("Length: %d\nAddress: %p\n", buffer.length, buffer_ptr);
    printf("Image Length: %d\n", buffer.bytesused);

    buffer_ptr = (uint8_t *) buffer_start;

    return 0;
}

int capture_image(int fd){
    struct v4l2_buffer buffer = {0};
    buffer.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    buffer.memory = V4L2_MEMORY_MMAP;
    buffer.index = 0;

    if ( -1 == xioctl(fd, VIDIOC_QBUF, &buffer)){
	printf("Errno: %d/n", errno);
        perror("Querying Buffer");
        return 1;
    }

    if ( -1 == xioctl(fd, VIDIOC_STREAMON, &buffer.type)){
	printf("Errno: %d/n", errno);
        perror("Start Capture");
        return 1;
    }

    fd_set fds;
    FD_ZERO(&fds);
    FD_SET(fd, &fds);

    struct timeval tv = {0};
    tv.tv_sec = 2;

    int r = select(fd+1, &fds, NULL, NULL, &tv);
    if ( -1 == r) {
	printf("Errno: %d/n", errno);
        perror("Waiting for Frame");
        return 1;
    }


    if ( -1 == xioctl(fd, VIDIOC_DQBUF, &buffer)){
	printf("Errno: %d/n", errno);
        perror("Retrieving Frame");
        return 1;
    }

    int outfd = open("out.img", O_CREAT | O_WRONLY, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH); 
    printf("%d\n", outfd);
    if ( outfd < 0 ) {
	    printf("Errno: %d/n", errno);
	    perror("Opening Output File");
	    return 1;
    }
    printf("Hello World\n"); 
    write(outfd, buffer_ptr, buffer.bytesused);
    close(outfd);

    return 0;

}


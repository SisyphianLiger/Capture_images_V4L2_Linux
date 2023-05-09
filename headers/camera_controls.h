#ifndef CAMERA_CONTROLS_H
#define CAMERA_CONTROLS_H
 

// Libraries`
#include <errno.h>
#include <linux/videodev2.h>
#include "../headers/xioctl.h"
#include <string.h>
#include <stdio.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>

// Important Structs` 


// Functions
int init_mmap(int fd);
int capture_image(int fd);

// Static function for query_capture

#endif  




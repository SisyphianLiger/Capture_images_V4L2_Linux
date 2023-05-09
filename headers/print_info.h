
#ifndef PRINT_INFO_H 
#define PRINT_INFO_H
// Libraries`
#include <errno.h>
#include <sys/ioctl.h>
#include <linux/videodev2.h>
#include "xioctl.h"
// Important Structs


// Functions
int print_caps(int fd);
int check_camera_capabilities(int fd);
int check_crop_capabilities(int fd);
int check_camera_format(int fd);


// Static function for query_capture


#endif  



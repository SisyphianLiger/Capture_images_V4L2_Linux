
#ifndef SET_FORMAT_H
#define SET_FORMAT_H
 
// Libraries`
#include <linux/videodev2.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <stdio.h>

// Important Structs` 

// Functions
int set_format(int fd, int width, int height);
// Static function for query_capture

#endif  



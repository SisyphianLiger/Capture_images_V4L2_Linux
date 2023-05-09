
#ifndef XIOCTL_H 
#define XIOCTL_H
// Libraries`
#include <errno.h>
#include <sys/ioctl.h>


// Important Structs` 


// Functions

// Static function for query_capture
int xioctl(int fd, int request, void * arg);


#endif  


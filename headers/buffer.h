#ifndef BUFFER_H
#define BUFFER_H

// Libraries`
#include <linux/videodev2.h>
#include <sys/ioctl.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/mman.h>

// Important Structs` 

// Functions
int request_buffer(int fd, int count);
int query_buffer(int fd, int index, unsigned char **buffer);
int enqueue_buffer(int fd, int index);
int dequeue_buffer(int fd);
int stop_stream(int fd);
int start_stream(int fd);
// Static function for query_capture

#endif  

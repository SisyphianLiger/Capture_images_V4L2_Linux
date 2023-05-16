#include <linux/videodev2.h>
#include <sys/ioctl.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/mman.h>

/*
 * This function allocates a buffer such that images have a place to go
 * The VIDIOC_REQBUS ioctl does this, but require a struct v4l2_requestbuffers
 * as an argument to do so.
 *
 * Also imporant to note: v4l2_requestbuffers.count needs to be set to 
 * number of memory buffers needed to be allocated.
 *
 * Must set it high enough so that frames won't be dropped due to lack of 
 * queed buffers. The driver decides what minimum number is. 
 *
 * v4l2_requestbuffers.type == V4L2_BUF_TYPE_VIDEO_CAPTURE
 * v4l2_requestbuffers.memory is the streaming method, we use V4L2_MEMORY_MMAP
*/

int request_buffer(int fd, int count){
    
    struct v4l2_requestbuffers request_buffer = {0};
    
    request_buffer.count = count;
    request_buffer.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    request_buffer.memory = V4L2_MEMORY_MMAP;
    
    if ( -1 == ioctl(fd, VIDIOC_REQBUFS, &request_buffer)){
        perror("Requesting Buffer");
        exit(1);
    }
    return request_buffer.count;
}

/*
 * After allocating the buffer we then query the phyical address 
 * to find their phyical addresses and the mmap() them.
 * 
 * mmap() lets a driver allocate buffers
 * 
 * The struct we need here is struct v4l2_buffer 
 * buffer.type == V4L2_BUF_TYPE_VIDEO_CAPTURE
 * buffer.memory == V4L2_MEMORY_MMAP
 * buffer.index == is from range 0 to buffer count 
*/
int query_buffer(int fd, int index, unsigned char **buffer){
    struct v4l2_buffer buffer_alloc = {0};
    
    buffer_alloc.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    buffer_alloc.memory = V4L2_MEMORY_MMAP;
    buffer_alloc.index = index;
    int res = ioctl(fd, VIDIOC_QUERYBUF, &buffer_alloc); 
    
    if ( res == -1 ){
        perror("Could not Query the Buffer");
        return 2;
    }
    
    *buffer = (u_int8_t*)mmap (NULL, buffer_alloc.length, PROT_READ | 
            PROT_WRITE, MAP_SHARED, fd, buffer_alloc.m.offset);
    
    return buffer_alloc.length;
}


/*
 * Before putting data in the buffer, we need to enqueue the buffer.
 * We do this by locking the memory pages so that the cannot be swapped out
 * during useage. Buffers will be locked until they are dequeued and the 
 * device is closed or stream is turned off.
 *
 * VIDIOC_QBUF has same arguments as VIDIOC_QUERYBUF so the same logic applies
 */


int enqueue_buffer(int fd, int index) {
    struct v4l2_buffer buffer_enqued = {0};
    
    buffer_enqued.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    buffer_enqued.memory = V4L2_MEMORY_MMAP;
    buffer_enqued.index = index;
    
    if( -1 == ioctl(fd, VIDIOC_QBUF, &buffer_enqued)){
        perror("Queue Buffer");
        return 1;
    }
    return buffer_enqued.bytesused;
}




/*
 * Once we fill the buffer with video data, they are ready to be dequed :D!
 * VIDIOC_DQBUF == VIDIOC_QBUF will take index number and put it in dequeued
 * mapping
 */

int dequeue_buffer(int fd) {
    struct v4l2_buffer buffer_alloc = {0};
    buffer_alloc.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    buffer_alloc.memory = V4L2_MEMORY_MMAP;
    
    if ( -1 == ioctl(fd, VIDIOC_DQBUF, &buffer_alloc)){
        perror("Dequeue Buffer");
        return 1;
    }
   
    return buffer_alloc.index;
}


/* This function starts the stream! basically just turns on VIDIOC_STREAMON
 * to inform v4l layer that it can start to acquire frames and use the queued
 * buffers to store them :)
 */
int start_stream(int fd){
    unsigned int type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

    if( ioctl(fd, VIDIOC_STREAMON, &type) == -1){
        perror("VIDIOC_STREAMON");
        exit(1);
    }
    return type;
}


/*
 * After the section is dequeued we then need to stop the stream
 */

int stop_stream(int fd) {
    unsigned int type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    
    if(ioctl(fd, VIDIOC_STREAMOFF, &type) == -1){
        perror("VIDIOC_STREAMON");
        exit(1);
    }
    
    return type;
}

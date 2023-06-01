#include <linux/videodev2.h>
#include <sys/ioctl.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/mman.h>
#include <errno.h>
#include <stdint.h>


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


/*@
  lemma ioctl_res_success_or_failure:
    \forall int io_res; io_res == 0 || io_res == -1;
*/

/*@     
    requires fd >= INT_MIN && fd <= INT_MAX;
    requires count >= 0 && count <= 4294967295;

    behavior buffer_successful:
        ensures \result; 

    behavior buffer_failured:
        ensures \result == EINVAL;

    disjoint behaviors;
    complete behaviors;

*/

int request_buffer(int fd, unsigned int count){
    
    struct v4l2_requestbuffers request_buffer = {0};
    request_buffer.count = count;
    //@ assert request_buffer.count == count;
    request_buffer.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    //@ assert request_buffer.count == count && request_buffer.type == V4L2_BUF_TYPE_VIDEO_CAPTURE;
    request_buffer.memory = V4L2_MEMORY_MMAP;
    //@ assert request_buffer.count == count && request_buffer.type == V4L2_BUF_TYPE_VIDEO_CAPTURE && request_buffer.memory == V4L2_MEMORY_MMAP; 
    int io_res = ioctl(fd, VIDIOC_REQBUFS, &request_buffer); 
    //@ assert io_res == 0 || io_res == -1 && request_buffer.count == count && request_buffer.type == V4L2_BUF_TYPE_VIDEO_CAPTURE && request_buffer.memory == V4L2_MEMORY_MMAP;
    if ( -1 == io_res) {
        //@ assert io_res == -1 && request_buffer.count == count && request_buffer.type == V4L2_BUF_TYPE_VIDEO_CAPTURE && request_buffer.memory == V4L2_MEMORY_MMAP;
        perror("Unable to Request a Buffer please make sure buffer is valid input for your camera");
        //@ assert io_res == -1 && request_buffer.count == count && request_buffer.type == V4L2_BUF_TYPE_VIDEO_CAPTURE && request_buffer.memory == V4L2_MEMORY_MMAP;
        fprintf(stderr, "Error Number: %d\n", EINVAL);
        //@ assert io_res == -1 && request_buffer.count == count && request_buffer.type == V4L2_BUF_TYPE_VIDEO_CAPTURE && request_buffer.memory == V4L2_MEMORY_MMAP;
        return EINVAL;
    }

    //@ assert io_res == 0 && request_buffer.count == count && request_buffer.type == V4L2_BUF_TYPE_VIDEO_CAPTURE && request_buffer.memory == V4L2_MEMORY_MMAP;
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


/*@     
    requires fd >= INT_MIN && fd <= INT_MAX;
    requires index >= INT_MIN && index <= INT_MAX;
    requires \valid(*buffer) && \valid(buffer);

    behavior buffer_successful:
        ensures \result; 

    behavior buffer_failured:
        ensures \result == EINVAL;

    disjoint behaviors;
    complete behaviors;

*/
int query_buffer(int fd, int index, unsigned char **buffer){
    struct v4l2_buffer buffer_alloc = {0};
    buffer_alloc.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    //@ assert buffer_alloc.type == V4L2_BUF_TYPE_VIDEO_CAPTURE;
    buffer_alloc.memory = V4L2_MEMORY_MMAP;
    //@ assert buffer_alloc.type == V4L2_BUF_TYPE_VIDEO_CAPTURE && buffer_alloc.memory == V4L2_MEMORY_MMAP;
    buffer_alloc.index = index;
    /*@ assert buffer_alloc.type == V4L2_BUF_TYPE_VIDEO_CAPTURE && 
               buffer_alloc.memory == V4L2_MEMORY_MMAP && 
               buffer_alloc.index == index;
    */ 
    int io_res = ioctl(fd, VIDIOC_QUERYBUF, &buffer_alloc); 
    //@ assert io_res == 0 || io_res == -1 && buffer_alloc.type == V4L2_BUF_TYPE_VIDEO_CAPTURE && buffer_alloc.memory == V4L2_MEMORY_MMAP && buffer_alloc.index == index;
    
    if ( io_res == -1 ){
        //@ assert io_res == -1 && buffer_alloc.type == V4L2_BUF_TYPE_VIDEO_CAPTURE && buffer_alloc.memory == V4L2_MEMORY_MMAP && buffer_alloc.index == index;
        perror("Could not Query the Buffer, Please check if query_capabilities runs successfully ");
        //@ assert io_res == -1 && buffer_alloc.type == V4L2_BUF_TYPE_VIDEO_CAPTURE && buffer_alloc.memory == V4L2_MEMORY_MMAP && buffer_alloc.index == index;
        fprintf(stderr, "Error Number: %d\n", EINVAL);
        //@ assert io_res == -1 && buffer_alloc.type == V4L2_BUF_TYPE_VIDEO_CAPTURE && buffer_alloc.memory == V4L2_MEMORY_MMAP && buffer_alloc.index == index;
        return EINVAL;
    }
    //@ assert io_res == 0  && buffer_alloc.type == V4L2_BUF_TYPE_VIDEO_CAPTURE && buffer_alloc.memory == V4L2_MEMORY_MMAP && buffer_alloc.index == index;
    *buffer = (u_int8_t*) mmap (NULL, buffer_alloc.length, PROT_READ | PROT_WRITE, MAP_SHARED, fd, buffer_alloc.m.offset); 
    //@ assert \valid(buffer) && io_res == 0  && buffer_alloc.type == V4L2_BUF_TYPE_VIDEO_CAPTURE && buffer_alloc.memory == V4L2_MEMORY_MMAP && buffer_alloc.index == index;
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


/*@     
    requires fd >= INT_MIN && fd <= INT_MAX;
    requires index >= INT_MIN && index <= INT_MAX;

    behavior buffer_successful:
        ensures \result; 

    behavior buffer_failured:
        ensures \result == EINVAL;

    disjoint behaviors;
    complete behaviors;

*/
int enqueue_buffer(int fd, int index) {
    struct v4l2_buffer buffer_enqued = {0};
    
    buffer_enqued.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    //@ assert buffer_enqued.type == V4L2_BUF_TYPE_VIDEO_CAPTURE;
    buffer_enqued.memory = V4L2_MEMORY_MMAP;
    //@ assert buffer_enqued.type == V4L2_BUF_TYPE_VIDEO_CAPTURE && buffer_enqued.memory == V4L2_MEMORY_MMAP;
    buffer_enqued.index = index;
    //@ assert buffer_enqued.type == V4L2_BUF_TYPE_VIDEO_CAPTURE && buffer_enqued.memory == V4L2_MEMORY_MMAP && buffer_enqued.index == index;
    
    int io_res = ioctl(fd, VIDIOC_QBUF, &buffer_enqued);
    //@ assert io_res == 0 || io_res == -1 && buffer_enqued.type == V4L2_BUF_TYPE_VIDEO_CAPTURE && buffer_enqued.memory == V4L2_MEMORY_MMAP && buffer_enqued.index == index;
    
    if( -1 == io_res) {
        //@ assert io_res == -1 && buffer_enqued.type == V4L2_BUF_TYPE_VIDEO_CAPTURE && buffer_enqued.memory == V4L2_MEMORY_MMAP && buffer_enqued.index == index;
        perror("Queue Buffer Had an issue setting buffer check input");
        //@ assert io_res == -1 && buffer_enqued.type == V4L2_BUF_TYPE_VIDEO_CAPTURE && buffer_enqued.memory == V4L2_MEMORY_MMAP && buffer_enqued.index == index;
        fprintf(stderr, "Error Number: %d\n", EINVAL);
        //@ assert io_res == -1 && buffer_enqued.type == V4L2_BUF_TYPE_VIDEO_CAPTURE && buffer_enqued.memory == V4L2_MEMORY_MMAP && buffer_enqued.index == index;
        return EINVAL;
    } 
    //@ assert io_res == 0 && buffer_enqued.type == V4L2_BUF_TYPE_VIDEO_CAPTURE && buffer_enqued.memory == V4L2_MEMORY_MMAP && buffer_enqued.index == index;
    return buffer_enqued.bytesused;
}




/*
 * Once we fill the buffer with video data, they are ready to be dequed :D!
 * VIDIOC_DQBUF == VIDIOC_QBUF will take index number and put it in dequeued
 * mapping
 */


/*@     
    requires fd >= INT_MIN && fd <= INT_MAX;

    behavior dequeued_buffer_successful:
        ensures \result; 

    behavior dequeued_buffer_failed:
        ensures \result == EINVAL;

    disjoint behaviors;
    complete behaviors;

*/

int dequeue_buffer(int fd) {
    struct v4l2_buffer buffer_alloc = {0};
    buffer_alloc.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    //@ assert buffer_alloc.type == V4L2_BUF_TYPE_VIDEO_CAPTURE;
    buffer_alloc.memory = V4L2_MEMORY_MMAP;
    //@ assert buffer_alloc.type == V4L2_BUF_TYPE_VIDEO_CAPTURE && buffer_alloc.memory == V4L2_MEMORY_MMAP;
    
    int io_res = ioctl(fd, VIDIOC_DQBUF, &buffer_alloc);
    //@ assert io_res == 0 || io_res == -1 && buffer_alloc.type == V4L2_BUF_TYPE_VIDEO_CAPTURE && buffer_alloc.memory == V4L2_MEMORY_MMAP;
    if ( -1 == io_res) {
        //@ assert io_res == -1 && buffer_alloc.type == V4L2_BUF_TYPE_VIDEO_CAPTURE && buffer_alloc.memory == V4L2_MEMORY_MMAP;
        perror("Unable to dequeue_buffer: Check if buffer has been initialized");
        //@ assert io_res == -1 && buffer_alloc.type == V4L2_BUF_TYPE_VIDEO_CAPTURE && buffer_alloc.memory == V4L2_MEMORY_MMAP;
        fprintf(stderr, "Error Number: %d\n", EINVAL);
        //@ assert io_res == -1 && buffer_alloc.type == V4L2_BUF_TYPE_VIDEO_CAPTURE && buffer_alloc.memory == V4L2_MEMORY_MMAP;
        return EINVAL;
    } 
    //@ assert io_res == 0 && buffer_alloc.type == V4L2_BUF_TYPE_VIDEO_CAPTURE && buffer_alloc.memory == V4L2_MEMORY_MMAP;
    return buffer_alloc.index;
}


/* This function starts the stream! basically just turns on VIDIOC_STREAMON
 * to inform v4l layer that it can start to acquire frames and use the queued
 * buffers to store them :)
 */


/*@     
    requires fd >= INT_MIN && fd <= INT_MAX;

    behavior dequeued_buffer_successful:
        ensures \result; 

    behavior dequeued_buffer_failed:
        ensures \result == EINVAL;

    disjoint behaviors;
    complete behaviors;

*/

int start_stream(int fd){
    unsigned int video_capture_type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    //@ assert video_capture_type == V4L2_BUF_TYPE_VIDEO_CAPTURE;
    int io_res = ioctl(fd, VIDIOC_STREAMON, &video_capture_type );
    //@ assert io_res == 0 || io_res == -1 && video_capture_type == V4L2_BUF_TYPE_VIDEO_CAPTURE;
    if( -1 == io_res ){
        //@ assert io_res == -1 && video_capture_type == V4L2_BUF_TYPE_VIDEO_CAPTURE;
        perror("Check to Make sure camera drivers and device are connected and that streaming is possible");
        //@ assert io_res == -1 && video_capture_type == V4L2_BUF_TYPE_VIDEO_CAPTURE;
        fprintf(stderr, "Error Number: %d\n", EINVAL);
        //@ assert io_res == -1 && video_capture_type == V4L2_BUF_TYPE_VIDEO_CAPTURE;
        return EINVAL;
    }
    //@ assert io_res == 0 && video_capture_type == V4L2_BUF_TYPE_VIDEO_CAPTURE;
    return video_capture_type;
}


/*
 * After the section is dequeued we then need to stop the stream
 */

/*@     
    requires fd >= INT_MIN && fd <= INT_MAX;

    behavior dequeued_buffer_successful:
        ensures \result; 

    behavior dequeued_buffer_failed:
        ensures \result == EINVAL;

    disjoint behaviors;
    complete behaviors;

*/

int stop_stream(int fd) {
    unsigned int video_capture_type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    //@ assert video_capture_type == V4L2_BUF_TYPE_VIDEO_CAPTURE;
    int io_res = ioctl(fd, VIDIOC_STREAMOFF, &video_capture_type );
    //@ assert io_res == 0 || io_res == -1 && video_capture_type == V4L2_BUF_TYPE_VIDEO_CAPTURE; 
    if( -1 == io_res ){
        //@ assert io_res == -1 && video_capture_type == V4L2_BUF_TYPE_VIDEO_CAPTURE;
        perror("Check to Make sure camera drivers and device are connected and that streaming is possible");
        //@ assert io_res == -1 && video_capture_type == V4L2_BUF_TYPE_VIDEO_CAPTURE;
        fprintf(stderr, "Error Number: %d\n", EINVAL);
        //@ assert io_res == -1 && video_capture_type == V4L2_BUF_TYPE_VIDEO_CAPTURE;
        return EINVAL;
    } 
    //@ assert io_res == 0 && video_capture_type == V4L2_BUF_TYPE_VIDEO_CAPTURE;
    return video_capture_type;
}

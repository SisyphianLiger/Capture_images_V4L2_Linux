#include <linux/videodev2.h>
#include <sys/ioctl.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>


/*
 * query_capabilities checks that the file in question has the capability to 
 * take a picture. The most important check here is V4L2_CAP_STREAMING, because
 * if verifyied true, it will be used as the capture device. 
 */


/*@
  lemma ioctl_res_success_or_failure:
    \forall int io_res; io_res == 0 || io_res == -1;
*/

/*@
    requires fd >= INT_MIN && fd <= INT_MAX;
    
    behavior Query_Successful:
    ensures \result == 0;

    behavior Not_Capable_Query:
    ensures \result == ENODEV;

    behavior No_Video_Detected:
    ensures \result == ENOSYS;

    behavior No_Read_Write_IO:
    ensures \result == EIO;

    behavior No_Streaming_Supported:
    ensures \result == ENOSTR;

    disjoint behaviors;
    complete behaviors;
*/

int query_capabilities(int fd) {
    struct v4l2_capability capability;
    //@ assert fd >= INT_MIN && fd <= INT_MAX;
    
    int io_res = ioctl(fd, VIDIOC_QUERYCAP, &capability);
    //@ assert io_res == 0 || io_res == -1 && fd >= INT_MIN && fd <= INT_MAX;
     
    if( -1 == io_res){
        //@ assert io_res == -1 && fd >= INT_MIN && fd <= INT_MAX;
        perror("No Device to Query capabilities found, please check connection");
        //@ assert io_res == -1 && fd >= INT_MIN && fd <= INT_MAX;
        fprintf(stderr, "Error Number: %d\n", ENODEV); 
        //@ assert io_res == -1 && fd >= INT_MIN && fd <= INT_MAX;
        return ENODEV;
    }
    
    int video_capture = capability.capabilities & V4L2_CAP_VIDEO_CAPTURE;
    /*@ assert io_res == 0 && 
               video_capture == 1 || video_capture == 0 && 
               fd >= INT_MIN && fd <= INT_MAX;
     */
    if ( ! video_capture ){
        //@ assert io_res == 0 && video_capture != 1 && fd >= INT_MIN && fd <= INT_MAX;
        perror("Device has no video capture device \\n");
        //@ assert io_res == 0 && video_capture != 1 && fd >= INT_MIN && fd <= INT_MAX; 
        fprintf(stderr, "Error Number: %d\n", ENOSYS);
        //@ assert io_res == 0 && video_capture != 1 && fd >= INT_MIN && fd <= INT_MAX;
        return ENOSYS;
    }

    int video_read_write = capability.capabilities & V4L2_CAP_READWRITE;
    /*@ assert io_res == 0 &&
               fd >= INT_MIN && fd <= INT_MAX &&
               video_capture == 1 &&
               video_read_write == 16777216 || video_read_write != 16777216;
     */
    if ( ! video_read_write ){
    /*@ assert io_res == 0 && video_capture == 1 &&
               video_read_write != 16777216 && 
               fd >= INT_MIN && fd <= INT_MAX; */
        perror("Device does not support read/write i/o \\n");
        /*@ assert io_res == 0 && video_capture == 1 &&
                   video_read_write != 16777216 && 
                   fd >= INT_MIN && fd <= INT_MAX; */
        fprintf(stderr, "Error Number: %d\n", EIO);
        /*@ assert io_res == 0 && video_capture == 1 &&
                   video_read_write != 16777216 && 
                   fd >= INT_MIN && fd <= INT_MAX; */
        return EIO;
    } 

    int video_streaming = capability.capabilities & V4L2_CAP_STREAMING;
    /*@ assert io_res == 0 &&
               video_capture == 1 &&
               video_read_write == 16777216 &&
               video_streaming == 67108864 || video_streaming != 67108864;
     */
    
    if ( ! video_streaming ){
        /*@ assert io_res == 0 && video_capture == 1 &&
                   video_read_write == 16777216 && 
                   video_streaming != 67108864;  
         */
        perror("Device does not support streaming \\n");
        /*@ assert io_res == 0 && video_capture == 1 &&
                   video_read_write == 16777216 && 
                   video_streaming != 67108864;  
         */
        fprintf(stderr, "Error Number: %d\n", ENOSTR);
        /*@ assert io_res == 0 && video_capture == 1 &&
                   video_read_write == 16777216 && 
                   video_streaming != 67108864;  
         */
        return ENOSTR;
    }

    //@ assert io_res == 0 && video_capture == 1 && video_read_write == 16777216 && video_streaming == 67108864;
    return 0;
}

#include "headers/check_capability.h"
#include "headers/set_format.h"
#include "headers/buffer.h"
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>


#define STANDARD_WIDTH 320
#define STANDARD_HEIGHT 240
#define NBUF 3


int main() {
    unsigned char* buffer[NBUF];
    // Check here what video driver we need to open
    // Assuming we open /dev/video0
    int fd = open("/dev/video0", O_RDWR); 
    int size;
    int index;
    int nbufs;
    
    // First let's check that it's possible to find the camera
    query_capabilities(fd);

    // Then we set format, macros made for custom pictures
    set_format(fd, STANDARD_WIDTH, STANDARD_HEIGHT);

    // Here we are requesting the buffer based on NBUF
    nbufs = request_buffer(fd, NBUF); 

    // Here we check that we have set up enough buffer slots otherwise
    // we get an angry driver and need to increase our size
    if(nbufs > NBUF) {
        fprintf(stderr, "Minimum NBUF size MUST be at least %i\n", nbufs);
        exit(1);
    }

    // So we allocate the buffer que!
    for (int i = 0; i < NBUF; i++) {
        // We Assume here that all buffers are of equal sizes
        size = query_buffer(fd, 0, &buffer[0]);
        // Now we allocate it here
        enqueue_buffer(fd, i);
    }
    // All Clear lets start the stream
    start_stream(fd);
    
    fd_set fds;
    FD_ZERO(&fds);
    FD_SET(fd, &fds);

    // Set time exposure to 2 seconds
    struct timeval tv = {0};
    tv.tv_sec = 2;

    // We select a buffer based o
    int r = select(fd+1, &fds, NULL, NULL, &tv);
    
    if (-1 == r) {
        perror("Waiting for Frame");
        exit(1);
    }

    // Now we make sure to dequeue_buffer the buffer
    index = dequeue_buffer(fd);

    // we store the oupute as an integer
    int output_file = open("output.raw", O_RDWR | O_CREAT, 0666);
    // if there is an error we output error message
    fprintf(stderr, "file == %i\n", output_file);
    // And write it here
    write(output_file, buffer[index], size);

    // We end the stream device because we have dequeue_buffer
    stop_stream(fd);
    // Make sure that the output_file is closed
    close(output_file);
    // close video device
    close(fd);

    return 0;
}

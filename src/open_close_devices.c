#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <errno.h>
#include <linux/videodev2.h>





int open_device(void) {
    int fd = open("/dev/video0", O_RDWR);
    if (fd == -1) {
        perror("Opening Video Device");
        return -1;
    }
    return fd;
}


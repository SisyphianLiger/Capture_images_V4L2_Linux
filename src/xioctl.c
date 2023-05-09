#include <errno.h>
#include <sys/ioctl.h>


int xioctl(int fd, int request, void *arg){
    int r;
        do r = ioctl (fd, request, arg);
        while (-1 == r && EINTR == errno);
        return r;
}

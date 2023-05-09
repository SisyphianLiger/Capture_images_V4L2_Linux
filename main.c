#include <stdint.h>
#include "headers/open_close_devices.h"
#include "headers/xioctl.h"
#include "headers/print_info.h"
#include "headers/camera_controls.h"





int main() {
    
    /*
     * Checks that there is a Camera Connection (device connection) in 
     * the video0 folder. 
     * */
    int fd = open_device();

    if(print_caps(fd))
       return 1;

    if(init_mmap(fd))
        return 1;

    if(capture_image(fd))
        return 1;

    close(fd);

    return 0;
}



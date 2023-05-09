#include <stdint.h>
#include "headers/open_close_devices.h"
#include "headers/xioctl.h"
#include "headers/print_info.h"
#include "headers/camera_controls.h"
#include <unistd.h>




int main() {
    
    /*
       Checks that there is a Camera Connection (device connection) in 
       the video0 folder. 
    */
   
    // Can Confirm Returns 0 on Raspberry Pi
    //

    char cwd[1024];
    if(getcwd(cwd, sizeof(cwd)) != NULL)
	   printf("Current Working Directory: %s\n", cwd);
    else 
	   perror("getcwd() error"); 


    int fd = open_device();
    printf("%d\n", fd);
/*
    if(print_caps(fd))
       return 1;

    if(init_mmap(fd))
        return 1;
*/

    if(capture_image(fd))
        return 1;

    close(fd);

    return 0;
}



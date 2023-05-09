# Capture_images_V4L2_Linux
Walking through the Image Capture Tutorial at: https://jayrambhia.com/blog/capture-v4l2

# Tutorial Start
This tutorial uses OpenCV's VideoCapture API to capture images form webcam of USB cameras

# OpenCV: Drivers
Supports: V4L2






# Functions in use with V4L2 

Opens a driver file /dev/video0 
int open(const char *device_name, int flags)
https://www.kernel.org/doc/html/v4.9/media/uapi/v4l/func-open.html#func-open

Input output control used for video capture/audio capture
int ioctl(int fd, int request, void *argp)
https://www.kernel.org/doc/html/v4.9/media/uapi/v4l/func-ioctl.html#func-ioctl

Closes file by file description
int clost(int fd)
https://www.kernel.org/doc/html/v4.9/media/uapi/v4l/func-close.html#func-close

S_ISCHR
Checks the file mode m, to see whether the file is a character device file
http://porthos.ist.utl.pt/docs/fpc/units/node300.html




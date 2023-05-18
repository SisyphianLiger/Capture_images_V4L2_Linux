These are instructions on how to get this working:

The following C code has only been used on a Raspberry Pi 3B.
However, the code should work on linux systems, so long as the 
v4l2 driver is installed. 

Install v4l2 with:

sudo apt-get install v4l-utils 


FOR RASPBERRY PI

Make sure the Legacy Camera Selected

    - Open a Terminal
    - Type: raspi-config
    - Navigate to Interface Options
    - Navigate to Camera
    - Enable Camera
    - Select Yes and Finish
    - Quit and return to CLI
    - sudo reboot


The Make File:
    Make sure you have make to run the program. The set up is modular
    for future development on DISCOSAT. The commands are the following.

        
        make all          ==> makes files
        make              ==> makes files
        make run          ==> makes and runs files
        make clear        ==> removes all binary in /bin


Current Output:
    Currently the output is a raw file that will be set in the top level
    of the file.


Dealing with the output.raw:
    To Check using a website use: rawpixels.net
    once the raw file is uploaded select the following Options

    width == define macro width from main.c


Update Ideas:
    Use args to make unique output.raw files. 
    Make directory to put the output in a pics folder


Frama-C-Gui Support: 

We need to link v4l2 with for Frama-C-Gui

This should be a template for any VM Linux (has not been tested on a non-VM Linux OS yet

frama-c-gui </path> -main <function_name> -cpp-extra-args="-I ../../../../../../usr/include/linux/" -c11

frama-c-gui                     ==> Frama-C of course
</path>                         ==> The place where you want to look relative to where you run the Frama-C command 
-main <function_name>           ==> Gives the Ability to look at only one function in the GUI 
-cpp-extra-args="-I include/"   ==> This is the includes portion to include libraries not recognized by Frama-C.
                                    Such as V4L2.
-c11                            ==> Flag for C11 Standards
-wp-rte                         ==> Flag for Weakest Post Condition run time error guards 
                                    Very important for running programs...like a satellite :D

Real World Example:
    
    frama-c-gui src/buffer.c -main query_buffer -cpp-extra-args="-I../../../../../../usr/include/" -c11 -wp-rte

Yea, sorry...it's a mouth full :(


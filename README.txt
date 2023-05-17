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

Update Ideas:
    Use args to make unique output.raw files. 
    Make directory to put the output in a pics folder



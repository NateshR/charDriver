# charDriver
This is an example of charDriver module:
You can cat its device file (or open the file with a program) and the driver will put the number of times the device file has been read from into the file. We don't support writing to the file (like echo "hi" > /dev/hello), but catch these attempts and tell the user that the operation isn't supported. 
Don't worry if you don't see what we do with the data we read into the buffer; we don't do much with it. 
We simply read in the data and print a message acknowledging that we received it.

INSTRUCTIONS:
1.run "make" command.
2.run "insmod charDriver.ko"
3.You need to run "mknod /dev/chardev c <majornumber> 0" separately to make it working.
4.Run "cat /dev/chardev" to see the output.

NOTE- To check if your device is registered check "/proc/devices" file and to remove the module run "rmmod charDriver".

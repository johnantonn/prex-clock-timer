Copyright 2016

Ioannis Antoniadis

<johneegr@gmail.com>

# prex-clock-timer
Implementation of a real-time embedded clock-timer running on Prex OS.

# Technologies Involved
Prex OS, QEMU, Debian OS, gcc cross compilation. 

# Installation steps in Debian
Prex was build and tested on a Debian 8.6.0 32-bit distribution.

## Admin privileges
Provide admin priviledges to the current user.

## gcc-4.1 installation
Prex needs a version of gcc older than 4.2 in order to be succesfully compiled. Download the required packages:   
cpp4.1_4.1.1-21_i386.deb  
gcc4.1_4.1.1-21_i386.deb  
gcc4.1-base_4.1.1-21_i386.deb  
libgcc1_4.1.1-21_i386.deb  
libssp0_4.1.1-21_i386.deb  
from [here](http://archive.debian.org/debian/pool/main/g/gcc-4.1/).  
  
Then, go to the Downloads folder and type the following commands in the following order:  
**sudo dpkg -i gcc-4.1-base_4.1.1-21_i386.deb**  
**sudo dpkg -i cpp-4.1_4.1.1-21_i386.deb**  
**sudo dpkg -i libgcc1_4.1.1-21_i386.deb**  
**sudo dpkg -i libssp0_4.1.1-21_i386.deb**  
**sudo apt install binutils**  
**sudo apt -f install**  
**sudo dpkg -i gcc-4.1_4.1.1-21_i386.deb**  
  
Type gcc-4.1 to verify that gcc-4.1 was succesfully installed.  

## Prex build
Download and untar the required Prex files:  
[prex-0.9.0.tar.gz](https://sourceforge.net/projects/prex/files/Source/prex-0.9.0/prex-0.9.0.tar.gz/download?use_mirror=netcologne&download=)  
[prex-0.8.0.i386-pc.img.gz](https://sourceforge.net/projects/prex/files/Floppy%20Image/prex-0.8.0/prex-0.8.0.i386-pc.img.gz/download?use_mirror=netcologne&download=)  
  
Go to the prex-0.9.0 directory and type:  
**sudo apt install make**  
**./configure --cc=gcc-4.1 --target=x86pc**  
**make**  
**make clean**  

## Application
Go to the usr/sample directory and create the application folder, name it clock-timer:  
**mkdir clock-timer**  
  
Inside that folder ceate the following files:  
**clock-timer.c**  
**Makefile**
  
The clock-timer.c contains the C code of the clock-timer application whereas the Makefile should look like this:  
*TASK= clock-timer.rt*  
*include $(SRCDIR)/mk/task.mk*  
  
Open the file prex-0.9.0/conf/etc/tasks.mk and comment out the following lines (by adding *#* before them as shown below):  
*ifeq ($(CONFIG_POSIX),y)*  
*#TASKS+= $(SRCDIR)/usr/server/boot/boot*  
*#TASKS+= $(SRCDIR)/usr/server/proc/proc*  
*#TASKS+= $(SRCDIR)/usr/server/exec/exec*  
*ifeq ($(CONFIG_PM),y)*  
*#TASKS+= $(SRCDIR)/usr/server/pow/pow*  
*endif*  
*#TASKS+= $(SRCDIR)/usr/server/fs/fs*  
*endif*  
  
Also, append this line to the end of the file:  
*TASKS+= $(SRCDIR)/usr/sample/clock-timer/clock-timer.rt*  

Finally, return to the prex-0.9.0/usr/sample directory and modify the Makefile by adding *clock-timer* at the end of the line:
*SUBDIR:= alarm balls cpumon bench hello ipc mutex sem task thread \ tetris clock-timer*  
  
Return to the prex-0.9.0 directory and run the commands:  
**make clean**  
**make**  
  
This process will generate the binary named *prexos* in the prex root directory. From now on, if you want to temper with the app's code, you should only change the clock-timer.c file and run **make clean; make** again for the croos compilation process to generate the new *prexos* binary.  
  
## Run on QEMU
In order to run the generated binary use the QEMU emulator. To install it type the following in the terminal:  
**sudo apt install mtools**  
**sudo apt install qemu-system-i386**  
**sudo nano /etc/mtools.conf**  
  
The last command will open a file with the nano editor. Modify the line:  
*drive a: file=”/dev/fd0” exclusive*  
into this:  
*drive a: file="/home/{user}/Downloads/prex-0.8.0.i386-pc.img"*  
or in general the path to the prex-0.8.0.i386-pc.img file.  
  
Go to the prex-0.9.0 folder and type the command:  
**mcopy -o prexos a:**  
  
Finally, navigate to the folder of the prex .img file and type:  
**qemu-system-i386 -localtime -fda ./prex-0.8.0.i386pc.img**

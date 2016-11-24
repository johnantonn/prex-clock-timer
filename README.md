Copyright 2016

Ioannis Antoniadis

<johneegr@gmail.com>

# prex-clock-timer
Implementation of a real-time embedded clock-timer running on Prex OS.

## Technologies Involved
Prex OS, QEMU, Debian OS, gcc cross compilation. 

## Installation steps in Debian
Prex was build and tested on a Debian 8.6.0 32-bit distribution.

### Admin privileges
Provide admin priviledges to the current user.

### Prex build
Prex needs an older version of gccc in order to be succesfully compiled.
Download the required packages: 
cpp4.1_4.1.1-21_i386.deb
gcc4.1_4.1.1-21_i386.deb
gcc4.1-base_4.1.1-21_i386.deb
libgcc1_4.1.1-21_i386.deb
libssp0_4.1.1-21_i386.deb
from [here](http://archive.debian.org/debian/pool/main/g/gcc-4.1/). Then, go to the Downloads folder and type the following commands in the following order:
**sudo dpkg -i gcc-4.1-base_4.1.1-21_i386.deb**
**sudo dpkg -i cpp-4.1_4.1.1-21_i386.deb**
**sudo dpkg -i libgcc1_4.1.1-21_i386.deb**
**sudo dpkg -i libssp0_4.1.1-21_i386.deb**
**sudo apt install binutils**
**sudo apt -f install**
**sudo dpkg -i gcc-4.1_4.1.1-21_i386.deb**

Type gcc-4.1 to verify that gcc-4.1 was succesfully installed. Next, download and untar the required Prex files:
[prex-0.9.0.tar.gz](https://sourceforge.net/projects/prex/files/Source/prex-0.9.0/prex-0.9.0.tar.gz/download?use_mirror=netcologne&download=)
[prex-0.8.0.i386-pc.img.gz](https://sourceforge.net/projects/prex/files/Floppy%20Image/prex-0.8.0/prex-0.8.0.i386-pc.img.gz/download?use_mirror=netcologne&download=)
Go to the prex-0.9.0 directory and type:
**sudo apt install make**
**./configure --cc=gcc-4.1 --target=x86pc**
**make**
**make clean**

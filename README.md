# MVK
MVK (Minimum Viable Kernel) is a hobby kernel project aiming to provide an OS Kernel (currently x86 only) that is the minimum viable kernel for basic functionality (like user-space programs and a basic shell). Right now it doesn't do any of that as it is heavily WIP.

Most of this code was written by me in 2019, and I have only just came back to it after that time, whether or not I'll keep continuing development I'm not sure, I'm only just releasing it because I fixed the main issue I was having with it and got it to be in a working state. But if anyone finds this code helpful in their own projects then by all means be my guest, this code is under the MIT license so you can do with it what you will.

## Building
It is recommended to build either through Linux or through WSL on Windows.

You'll need an i686-elf toolchain to build the kernel. I found [this script](https://github.com/lordmilko/i686-elf-tools) handy for compiling one (you can even compile a windows toolchain and GDB if you so wish). It is recommended that the GCC and Binutils versions you use are pretty much the latest. To find what are the latest versions check the latest versions listed [here](https://ftp.gnu.org/gnu/gcc/) and [here](https://ftp.gnu.org/gnu/binutils/) for GCC and Binutils respectively.

You'll also require grub and xorriso, on Debian-based operating systems (like Ubuntu) simply install the packages like this:
```
sudo apt install grub2
sudo apt install xorriso
```

## Running
To test the operating system, a good tool would be [Qemu](https://www.qemu.org/).

On Debian-based operating systems you can install it like this
```
sudo apt install qemu
```
then simply run the operating system like this from the project root directory
```
qemu-system-x86_64 -boot d -cdrom build/MVK.iso -m 4096
```
> Note: the number after -m is for the amount of allocated ram, if you need this to be lower, lower it to what you would prefer instead (in MB)

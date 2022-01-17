MAKE = make
BASH = bash

KERNEL_TARGET = ./build/MVK.elf

all: libc kernel iso

libc: 
	$(MAKE) -C libc
kernel:
	$(MAKE) -C kernel
iso: 
	$(BASH) makeiso.sh
.PHONY: libc kernel iso

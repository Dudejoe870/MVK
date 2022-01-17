#define ASM_FILE 1
#include "multiboot.h"
#include "util.h"

#define MULTIBOOT_HEADER_FLAGS MULTIBOOT_PAGE_ALIGN | MULTIBOOT_MEMORY_INFO | MULTIBOOT_VIDEO_MODE

.section .multiboot.data
.align MULTIBOOT_HEADER_ALIGN
.long MULTIBOOT_HEADER_MAGIC
.long MULTIBOOT_HEADER_FLAGS
.long -(MULTIBOOT_HEADER_MAGIC + MULTIBOOT_HEADER_FLAGS)

.long 0, 0, 0, 0, 0
.long 1
.long 0, 0, 0

.section .bootstrap_stack, "aw", @nobits
stack_bottom:
.skip 16384 # 16 KiB
stack_top:

.section .bss, "aw", @nobits
.align 4096
boot_page_directory:
    .skip 4096
boot_page_table1:
    .skip 4096
boot_page_table2:
    .skip 4096

.section .multiboot.text
.global _start
.type _start, @function
_start:
	movl $0, %esi # Start Address

	# Page Table 1
    movl $KERNEL_TO_PHYS(boot_page_table1), %edi
    movl $1024, %ecx
1:
    movl %esi, %edx
    orl $0x003, %edx
    movl %edx, (%edi)

    addl $4096, %esi
    addl $4, %edi
    loop 1b
	
	# Page Table 2
    movl $KERNEL_TO_PHYS(boot_page_table2), %edi
    movl $1024, %ecx
2:
    movl %esi, %edx
    orl $0x003, %edx
    movl %edx, (%edi)

    addl $4096, %esi
    addl $4, %edi
    loop 2b

    movl $(KERNEL_TO_PHYS(boot_page_table1) + 0x003), KERNEL_TO_PHYS(boot_page_directory) + 0 * 4
    movl $(KERNEL_TO_PHYS(boot_page_table1) + 0x003), KERNEL_TO_PHYS(boot_page_directory) + ((KERNEL_ADDRESS / (4096 * 1024) + 0) * 4)
	movl $(KERNEL_TO_PHYS(boot_page_table2) + 0x003), KERNEL_TO_PHYS(boot_page_directory) + ((KERNEL_ADDRESS / (4096 * 1024) + 1) * 4)

    movl $KERNEL_TO_PHYS(boot_page_directory), %ecx
	movl %ecx, %cr3

	# Enable Paging
    movl %cr0, %ecx
	orl $0x80010001, %ecx
	movl %ecx, %cr0

	# Jump to higher half kernel
    lea _kernel_entry, %ecx
	jmp *%ecx

.section .text
_kernel_entry:
    mov $stack_top, %esp # Setup the stack

    fninit # Configure FPU

	addl $KERNEL_ADDRESS, %ebx
    pushl %ebx # Pointer to Multiboot Structure
    pushl %eax # Multiboot Magic

	call kernel_main # Jump to C Kernel Code

	cli
1:  hlt
	jmp 1b
text_end:

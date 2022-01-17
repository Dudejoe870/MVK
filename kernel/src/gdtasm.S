.section .text

#define ASM_FILE 1
#include "gdt.h"

gdtr:
.short 0 # Limit
.long 0 # Base

.global set_gdt
.type set_gdt, @function
set_gdt: # void set_gdt(gdt_entry_bits* gdt, unsigned long gdt_size)
    movl 4(%esp), %eax
    movl %eax, gdtr+2
    movw 8(%esp), %ax
    movw %ax, gdtr
    lgdt gdtr
    jmp $P_CODE_GDT_R0,$1f
1:
    movw $P_DATA_GDT_R0, %ax
    movw %ax, %ds
    movw %ax, %es
    movw %ax, %fs
    movw %ax, %gs
    movw %ax, %ss
    ret

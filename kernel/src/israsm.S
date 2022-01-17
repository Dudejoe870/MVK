.section .text

.extern isr_handler
.extern irq_handler

isr_common_stub:
    pusha # Push all the general purpose registers to the stack

    movl %cr2, %eax # Push CR2 to the stack
    pushl %eax

    movw %ds, %ax # Push DS
    pushl %eax

    movw $0x10, %ax
    movw %ax, %ds
    movw %ax, %es
    movw %ax, %fs
    movw %ax, %gs

    pushl %esp # Give the isr handler the interrupt info on the stack
    call isr_handler
    popl %eax # pop ESP

    popl %eax # pop the DS value

    movw %ax, %ds # Restore the segment values
    movw %ax, %es
    movw %ax, %fs
    movw %ax, %gs

    popl %eax # pop the CR2 value

    popa # Restore general purpose registers
    addl $8, %esp

    sti
    iret

irq_common_stub:
    pusha # Push all the general purpose registers to the stack

    movl %cr2, %eax # Push CR2 to the stack
    pushl %eax

    movw %ds, %ax # Push DS
    pushl %eax

    movw $0x10, %ax
    movw %ax, %ds
    movw %ax, %es
    movw %ax, %fs
    movw %ax, %gs

    pushl %esp # Give the irq handler the interrupt info on the stack
    cld
    call irq_handler
    popl %eax # pop ESP

    popl %eax # pop the DS value

    movw %ax, %ds # Restore the segment values
    movw %ax, %es
    movw %ax, %fs
    movw %ax, %gs

    popl %eax # pop the CR2 value

    popa # Restore general purpose registers
    addl $8, %esp

    sti
    iret

.global isr0
.global isr1
.global isr2
.global isr3
.global isr4
.global isr5
.global isr6
.global isr7
.global isr8
.global isr9
.global isr10
.global isr11
.global isr12
.global isr13
.global isr14
.global isr15
.global isr16
.global isr17
.global isr18
.global isr19
.global isr20
.global isr21
.global isr22
.global isr23
.global isr24
.global isr25
.global isr26
.global isr27
.global isr28
.global isr29
.global isr30
.global isr31

// ISRs
// 0: Divide By Zero Exception
isr0:
    cli
    pushl $0
    pushl $0
    jmp isr_common_stub

// 1: Debug Exception
isr1:
    cli
    pushl $0
    pushl $1
    jmp isr_common_stub

// 2: Non Maskable Interrupt Exception
isr2:
    cli
    pushl $0
    pushl $2
    jmp isr_common_stub

// 3: Int 3 Exception
isr3:
    cli
    pushl $0
    pushl $3
    jmp isr_common_stub

// 4: INTO Exception
isr4:
    cli
    pushl $0
    pushl $4
    jmp isr_common_stub

// 5: Out of Bounds Exception
isr5:
    cli
    pushl $0
    pushl $5
    jmp isr_common_stub

// 6: Invalid Opcode Exception
isr6:
    cli
    pushl $0
    pushl $6
    jmp isr_common_stub

// 7: Coprocessor Not Available Exception
isr7:
    cli
    pushl $0
    pushl $7
    jmp isr_common_stub

// 8: Double Fault Exception (Pushes Error Code)
isr8:
    cli
    pushl $8
    jmp isr_common_stub

// 9: Coprocessor Segment Overrun Exception
isr9:
    cli
    pushl $0
    pushl $9
    jmp isr_common_stub

// 10: Bad TSS Exception (Pushes Error Code)
isr10:
    cli
    pushl $10
    jmp isr_common_stub

// 11: Segment Not Present Exception (Pushes Error Code)
isr11:
    cli
    pushl $11
    jmp isr_common_stub

// 12: Stack Fault Exception (Pushes Error Code)
isr12:
    cli
    pushl $12
    jmp isr_common_stub

// 13: General Protection Fault Exception (Pushes Error Code)
isr13:
    cli
    pushl $13
    jmp isr_common_stub

// 14: Page Fault Exception (Pushes Error Code)
isr14:
    cli
    pushl $14
    jmp isr_common_stub

// 15: Reserved Exception
isr15:
    cli
    pushl $0
    pushl $15
    jmp isr_common_stub

// 16: Floating Point Exception
isr16:
    cli
    pushl $0
    pushl $16
    jmp isr_common_stub

// 17: Alignment Check Exception
isr17:
    cli
    pushl $0
    pushl $17
    jmp isr_common_stub

// 18: Machine Check Exception
isr18:
    cli
    pushl $0
    pushl $18
    jmp isr_common_stub

// 19: Reserved
isr19:
    cli
    pushl $0
    pushl $19
    jmp isr_common_stub

// 20: Reserved
isr20:
    cli
    pushl $0
    pushl $20
    jmp isr_common_stub

// 21: Reserved
isr21:
    cli
    pushl $0
    pushl $21
    jmp isr_common_stub

// 22: Reserved
isr22:
    cli
    pushl $0
    pushl $22
    jmp isr_common_stub

// 23: Reserved
isr23:
    cli
    pushl $0
    pushl $23
    jmp isr_common_stub

// 24: Reserved
isr24:
    cli
    pushl $0
    pushl $24
    jmp isr_common_stub

// 25: Reserved
isr25:
    cli
    pushl $0
    pushl $25
    jmp isr_common_stub

// 26: Reserved
isr26:
    cli
    pushl $0
    pushl $26
    jmp isr_common_stub

// 27: Reserved
isr27:
    cli
    pushl $0
    pushl $27
    jmp isr_common_stub

// 28: Reserved
isr28:
    cli
    pushl $0
    pushl $28
    jmp isr_common_stub

// 29: Reserved
isr29:
    cli
    pushl $0
    pushl $29
    jmp isr_common_stub

// 30: Reserved
isr30:
    cli
    pushl $0
    pushl $30
    jmp isr_common_stub

// 31: Reserved
isr31:
    cli
    pushl $0
    pushl $31
    jmp isr_common_stub

.global irq0
.global irq1
.global irq2
.global irq3
.global irq4
.global irq5
.global irq6
.global irq7
.global irq8
.global irq9
.global irq10
.global irq11
.global irq12
.global irq13
.global irq14
.global irq15

// IRQs
irq0:
    cli
    pushl $0
    pushl $32
    jmp irq_common_stub
irq1:
    cli
    pushl $1
    pushl $33
    jmp irq_common_stub
irq2:
    cli
    pushl $2
    pushl $34
    jmp irq_common_stub
irq3:
    cli
    pushl $3
    pushl $35
    jmp irq_common_stub
irq4:
    cli
    pushl $4
    pushl $36
    jmp irq_common_stub
irq5:
    cli
    pushl $5
    pushl $37
    jmp irq_common_stub
irq6:
    cli
    pushl $6
    pushl $38
    jmp irq_common_stub
irq7:
    cli
    pushl $7
    pushl $39
    jmp irq_common_stub
irq8:
    cli
    pushl $8
    pushl $40
    jmp irq_common_stub
irq9:
    cli
    pushl $9
    pushl $41
    jmp irq_common_stub
irq10:
    cli
    pushl $10
    pushl $42
    jmp irq_common_stub
irq11:
    cli
    pushl $11
    pushl $43
    jmp irq_common_stub
irq12:
    cli
    pushl $12
    pushl $44
    jmp irq_common_stub
irq13:
    cli
    pushl $13
    pushl $45
    jmp irq_common_stub
irq14:
    cli
    pushl $14
    pushl $46
    jmp irq_common_stub
irq15:
    cli
    pushl $15
    pushl $47
    jmp irq_common_stub

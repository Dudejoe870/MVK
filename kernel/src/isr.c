#include "isr.h"

#include "idt.h"
#include "pic.h"

void isr_install(void)
{
    set_idt_gate(0, (uint32_t)isr0, TRAP_GATE);
    set_idt_gate(1, (uint32_t)isr1, TRAP_GATE);
    set_idt_gate(2, (uint32_t)isr2, TRAP_GATE);
    set_idt_gate(3, (uint32_t)isr3, TRAP_GATE);
    set_idt_gate(4, (uint32_t)isr4, TRAP_GATE);
    set_idt_gate(5, (uint32_t)isr5, TRAP_GATE);
    set_idt_gate(6, (uint32_t)isr6, TRAP_GATE);
    set_idt_gate(7, (uint32_t)isr7, TRAP_GATE);
    set_idt_gate(8, (uint32_t)isr8, TRAP_GATE);
    set_idt_gate(9, (uint32_t)isr9, TRAP_GATE);
    set_idt_gate(10, (uint32_t)isr10, TRAP_GATE);
    set_idt_gate(11, (uint32_t)isr11, TRAP_GATE);
    set_idt_gate(12, (uint32_t)isr12, TRAP_GATE);
    set_idt_gate(13, (uint32_t)isr13, TRAP_GATE);
    set_idt_gate(14, (uint32_t)isr14, TRAP_GATE);
    set_idt_gate(15, (uint32_t)isr15, TRAP_GATE);
    set_idt_gate(16, (uint32_t)isr16, TRAP_GATE);
    set_idt_gate(17, (uint32_t)isr17, TRAP_GATE);
    set_idt_gate(18, (uint32_t)isr18, TRAP_GATE);
    set_idt_gate(19, (uint32_t)isr19, TRAP_GATE);
    set_idt_gate(20, (uint32_t)isr20, TRAP_GATE);
    set_idt_gate(21, (uint32_t)isr21, TRAP_GATE);
    set_idt_gate(22, (uint32_t)isr22, TRAP_GATE);
    set_idt_gate(23, (uint32_t)isr23, TRAP_GATE);
    set_idt_gate(24, (uint32_t)isr24, TRAP_GATE);
    set_idt_gate(25, (uint32_t)isr25, TRAP_GATE);
    set_idt_gate(26, (uint32_t)isr26, TRAP_GATE);
    set_idt_gate(27, (uint32_t)isr27, TRAP_GATE);
    set_idt_gate(28, (uint32_t)isr28, TRAP_GATE);
    set_idt_gate(29, (uint32_t)isr29, TRAP_GATE);
    set_idt_gate(30, (uint32_t)isr30, TRAP_GATE);
    set_idt_gate(31, (uint32_t)isr31, TRAP_GATE);

    pic_remap(IRQ0, IRQ8);

    set_idt_gate(IRQ0, (uint32_t)irq0, INTERRUPT_GATE);
    set_idt_gate(IRQ1, (uint32_t)irq1, INTERRUPT_GATE);
    set_idt_gate(IRQ2, (uint32_t)irq2, INTERRUPT_GATE);
    set_idt_gate(IRQ3, (uint32_t)irq3, INTERRUPT_GATE);
    set_idt_gate(IRQ4, (uint32_t)irq4, INTERRUPT_GATE);
    set_idt_gate(IRQ5, (uint32_t)irq5, INTERRUPT_GATE);
    set_idt_gate(IRQ6, (uint32_t)irq6, INTERRUPT_GATE);
    set_idt_gate(IRQ7, (uint32_t)irq7, INTERRUPT_GATE);
    set_idt_gate(IRQ8, (uint32_t)irq8, INTERRUPT_GATE);
    set_idt_gate(IRQ9, (uint32_t)irq9, INTERRUPT_GATE);
    set_idt_gate(IRQ10, (uint32_t)irq10, INTERRUPT_GATE);
    set_idt_gate(IRQ11, (uint32_t)irq11, INTERRUPT_GATE);
    set_idt_gate(IRQ12, (uint32_t)irq12, INTERRUPT_GATE);
    set_idt_gate(IRQ13, (uint32_t)irq13, INTERRUPT_GATE);
    set_idt_gate(IRQ14, (uint32_t)irq14, INTERRUPT_GATE);
    set_idt_gate(IRQ15, (uint32_t)irq15, INTERRUPT_GATE);
}

const char* exception_messages[] = 
{
    "Division By Zero",
    "Debug",
    "Non Maskable Interrupt",
    "Breakpoint",
    "Into Detected Overflow",
    "Out of Bounds",
    "Invalid Opcode",
    "No Coprocessor",

    "Double Fault",
    "Coprocessor Segment Overrun",
    "Bad TSS",
    "Segment Not Present",
    "Stack Fault",
    "General Protection Fault",
    "Page Fault",
    "Unknown Interrupt",

    "Coprocessor Fault",
    "Alignment Check",
    "Machine Check",

    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved"
};

void isr_handler(interrupt_info* info)
{
    puts("Received exception:");
    printf("  Interrupt Number: %u\n", info->int_no);
    if (info->err_code) printf("  Error Code: %u\n", info->err_code);
    printf("  Description: %s\n", exception_messages[info->int_no]);
    puts("  Registers:");
    printf("    EAX: 0x%x\n", info->eax);
    printf("    ECX: 0x%x\n", info->ecx);
    printf("    EDX: 0x%x\n", info->edx);
    printf("    EBX: 0x%x\n", info->ebx);
    printf("    ESI: 0x%x\n", info->esi);
    printf("    EDI: 0x%x\n", info->edi);
    printf("    EIP: 0x%x\n", info->eip);
    printf("    ESP: 0x%x\n", info->esp);
    printf("    EBP: 0x%x\n", info->ebp);
    printf("    CR2: 0x%x\n", info->cr2);

    abort();
}

static irq_func irq_handlers[16];

void irq_handler(interrupt_info* info)
{
    if (irq_handlers[info->irq_no]) irq_handlers[info->irq_no](info);

    pic_eoi(info->irq_no);
}

void register_irq_handler(uint8_t irq, irq_func handler)
{
    irq_handlers[irq] = handler;
}

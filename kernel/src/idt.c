#include "idt.h"

#include "pic.h"
#include "gdt.h"
#include "isr.h"

idt_entry IDT[256];

static inline void lidt(idt_entry* base, uint16_t size)
{
    struct 
    {
        uint16_t length;
        void*    base;
    } __attribute__((packed)) IDTR = { size, base };
 
    asm volatile ( "lidt %0" : : "m"(IDTR) );
}

void idt_initialize(void)
{
    puts("idt_initialize: Initializing the IDT");

    printf("  Setting the IDT Gates and installing the ISR Routines...");
    isr_install();
    puts(" OK");

    printf("  Setting the IDT...");
    lidt(IDT, sizeof(IDT));
    puts(" OK");

    printf("  Enabling Interrupts...");
    asm volatile( "sti" );
    puts(" OK");
}

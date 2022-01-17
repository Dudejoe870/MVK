#pragma once

#include <stdcommon.h>

#include "gdt.h"

#define TRAP_GATE 0x8F
#define INTERRUPT_GATE 0x8E

typedef struct __attribute__((packed))
{
    uint16_t offset_low;
    uint16_t selector;
    uint8_t zero;
    uint8_t type_attr;
    uint16_t offset_high;
} idt_entry;

extern idt_entry IDT[];

static inline void set_idt_gate(size_t i, uint32_t handler, uint8_t type)
{
    IDT[i].offset_low = handler & 0xFFFF;
    IDT[i].selector = P_CODE_GDT_R0;
    IDT[i].zero = 0;
    IDT[i].type_attr = type;
    IDT[i].offset_high = handler >> 16;
}

void idt_initialize(void);

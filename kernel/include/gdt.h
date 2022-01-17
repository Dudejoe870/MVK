#pragma once

#define CODE_GDT_R0 1
#define DATA_GDT_R0 2
#define TSS_GDT_R3 3

#define P_CODE_GDT_R0 0x08
#define P_DATA_GDT_R0 0x10
#define P_TSS_GDT_R3 0x18

#ifndef ASM_FILE

#include <stdcommon.h>
#include "terminal.h"

typedef struct __attribute__((__packed__))
{
	unsigned int limit_low :16;
	unsigned int base_low  :24;
	unsigned int accessed :1;
	unsigned int read_write :1; // readable for code, writable for data
	unsigned int conforming_expand_down :1; // conforming for code, expand down for data
	unsigned int code :1; // 1 for code, 0 for data
	unsigned int TSS :1; // should be 1 for everything but TSS and LDT
	unsigned int DPL :2; // privilege level
	unsigned int present :1;
	unsigned int limit_high :4;
	unsigned int available :1;
	unsigned int always_0 :1; // should always be 0
	unsigned int big :1; // 32bit opcodes for code, uint32_t stack for data
	unsigned int gran :1; // 1 to use 4k page addressing, 0 for byte addressing
	unsigned int base_high :8;
} gdt_entry_bits;

typedef struct __attribute__((packed))
{
   uint32_t prev_tss;
   uint32_t esp0;
   uint32_t ss0;
   uint32_t esp1;
   uint32_t ss1;
   uint32_t esp2;
   uint32_t ss2;
   uint32_t cr3;
   uint32_t eip;
   uint32_t eflags;
   uint32_t eax;
   uint32_t ecx;
   uint32_t edx;
   uint32_t ebx;
   uint32_t esp;
   uint32_t ebp;
   uint32_t esi;
   uint32_t edi;
   uint32_t es;         
   uint32_t cs;        
   uint32_t ss;        
   uint32_t ds;        
   uint32_t fs;       
   uint32_t gs;         
   uint32_t ldt;      
   uint16_t trap;
   uint16_t iomap_base;
} tss_entry;

void gdt_initialize(void);

extern void set_gdt(gdt_entry_bits* gdt, unsigned long gdt_size);

#endif

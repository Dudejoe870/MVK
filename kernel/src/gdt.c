#include "gdt.h"

#include "util.h"
#include "vga.h"

#define GDT_SIZE 4

static gdt_entry_bits GDT[GDT_SIZE];
static tss_entry TSS;

void gdt_initialize(void)
{
    puts("gdt_initialize: Initializing the GDT");

    printf("  Zeroing out the GDT...");

    memset((uint8_t*)GDT, 0, sizeof(GDT));

    puts(" OK");

    printf("  Initializing the kernel code descriptor...");

    GDT[CODE_GDT_R0].limit_high = 0xF;
    GDT[CODE_GDT_R0].limit_low  = 0xFFFF;
    GDT[CODE_GDT_R0].base_high  = 0x0000;
    GDT[CODE_GDT_R0].base_low   = 0x0000;

    GDT[CODE_GDT_R0].accessed = 0;
    GDT[CODE_GDT_R0].read_write = 1;
    GDT[CODE_GDT_R0].conforming_expand_down = 0;
    GDT[CODE_GDT_R0].code = 1;
    GDT[CODE_GDT_R0].TSS = 1;
    GDT[CODE_GDT_R0].DPL = 0;
    GDT[CODE_GDT_R0].present = 1;
    GDT[CODE_GDT_R0].available = 1;
    GDT[CODE_GDT_R0].always_0 = 0;
    GDT[CODE_GDT_R0].big = 1;
    GDT[CODE_GDT_R0].gran = 1;

    puts(" OK");

    printf("  Initializing the kernel data descriptor...");

    GDT[DATA_GDT_R0] = GDT[CODE_GDT_R0];
    GDT[DATA_GDT_R0].code = 0;

    puts(" OK");

    printf("  Initializing the TSS descriptor...");

    uint32_t base = (uint32_t)&TSS;
    uint32_t limit = sizeof(TSS);
    GDT[TSS_GDT_R3].limit_high = (limit & 0xF0000) >> 16;
    GDT[TSS_GDT_R3].limit_low  = (limit & 0xFFFF);
    GDT[TSS_GDT_R3].base_high  = (base & 0xFF000000) >> 24;
    GDT[TSS_GDT_R3].base_low   = (base & 0xFFFFFF);

    GDT[TSS_GDT_R3].accessed = 1; // 1 for TSS
    GDT[TSS_GDT_R3].read_write = 0; // TSS Busy
    GDT[TSS_GDT_R3].conforming_expand_down = 0;
    GDT[TSS_GDT_R3].code = 1; // 32-bit TSS
    GDT[TSS_GDT_R3].TSS = 0;
    GDT[TSS_GDT_R3].DPL = 3;
    GDT[TSS_GDT_R3].present = 1;
    GDT[TSS_GDT_R3].available = 0;
    GDT[TSS_GDT_R3].always_0 = 0;
    GDT[TSS_GDT_R3].big = 0;
    GDT[TSS_GDT_R3].gran = 0;

    puts(" OK");

    printf("  Setting the GDT...");

    set_gdt(GDT, sizeof(GDT));

    puts(" OK");
}

#pragma once

#define KERNEL_ADDRESS 0xC0000000

#ifndef ASM_FILE
#include <stdcommon.h>

#define CHECK_FLAG(flags, bit) ((flags) & (1 << (bit)))

#define KERNEL_TO_PHYS(addr) ((uint32_t)addr - KERNEL_ADDRESS)
#define PHYS_TO_KERNEL(addr) ((uint32_t)addr + KERNEL_ADDRESS)
#else
#define KERNEL_TO_PHYS(addr) (addr - KERNEL_ADDRESS)
#define PHYS_TO_KERNEL(addr) (addr + KERNEL_ADDRESS)
#endif

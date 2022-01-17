#pragma once

#include <stdcommon.h>

#include "multiboot.h"

extern multiboot_memory_map_t* mmap;
extern uint64_t mmap_length;
extern void* mmap_free_memory;
extern uint64_t mmap_free_memory_length;

void mmap_initialize(multiboot_info_t* mbi);

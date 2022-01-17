#include "mmap.h"

#include "util.h"
#include "paging.h"

multiboot_memory_map_t* mmap;
uint64_t mmap_length;
void* mmap_free_memory;
uint64_t mmap_free_memory_length;

extern uint8_t _kernel_start;
extern uint8_t _kernel_end;

void mmap_initialize(multiboot_info_t* mbi)
{
    puts("mmap_initialize: Initializing the Memory Map");

    printf("  Checking the MBI flag to see if we have access to memory info...");
    if (!CHECK_FLAG(mbi->flags, 6)) 
    {
        puts(" FAILED");
        abort();
    }
    puts(" OK");

    printf("  Initializing Memory Map...");
    mmap = (multiboot_memory_map_t*)PHYS_TO_KERNEL(mbi->mmap_addr);
    if (mbi->mmap_length != 0) 
    {
        mmap_length = mbi->mmap_length / sizeof(multiboot_memory_map_t);
        puts(" OK");
    }
    else
    {
        puts(" FAILED");
        abort();
    }

    printf("  Finding free Memory area...");
    // Find the biggest memory area available (most likely the main memory area)
    for (uint64_t i = 0; i < mmap_length; ++i)
    {
        multiboot_memory_map_t* entry = &mmap[i];
        if (entry->type == MULTIBOOT_MEMORY_AVAILABLE)
        {
            uint64_t entry_length = (uint64_t)entry->len_low | ((uint64_t)entry->len_high << 32);
            if (mmap_free_memory_length < entry_length)
            {
                mmap_free_memory_length = entry_length;
                mmap_free_memory = (void*)entry->addr_low;
            }
        }
    }
    // Make sure we don't overlap with the kernel
    if ((uint32_t)mmap_free_memory < KERNEL_TO_PHYS(&_kernel_end) && (uint32_t)((uint64_t)((uint32_t)mmap_free_memory) + mmap_free_memory_length) >= KERNEL_TO_PHYS(&_kernel_start))
    {
        uint32_t prev_pointer = (uint32_t)mmap_free_memory;
        mmap_free_memory = (void*)KERNEL_TO_PHYS(&_kernel_end);
        mmap_free_memory_length -= (uint32_t)mmap_free_memory - prev_pointer;
    }
    // Align the region to the page boundary
    uint32_t pointer_alignment = (uint32_t)mmap_free_memory % PAGE_SIZE;
    if (pointer_alignment != 0)
    {
        uint32_t prev_pointer = (uint32_t)mmap_free_memory;
        mmap_free_memory = (void*)((uint32_t)mmap_free_memory + (PAGE_SIZE - pointer_alignment));
        mmap_free_memory_length -= (uint32_t)mmap_free_memory - prev_pointer;
    }
    uint32_t length_alignment = mmap_free_memory_length % PAGE_SIZE;
    mmap_free_memory_length -= length_alignment;
    printf(" OK (0x%x)\n", (uint32_t)mmap_free_memory);
}

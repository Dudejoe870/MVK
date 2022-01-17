#include "paging.h"

#include "vga.h"
#include "mmap.h"
#include "util.h"

#define KERNEL_PAGE_TABLES_SIZE 2
#define KERNEL_PAGE_DIRECTORY_START (KERNEL_ADDRESS / (4096 * 1024))
#define FREE_PAGE_TABLES_SIZE (KERNEL_PAGE_DIRECTORY_START-1)
#define PAGE_BITMAP_SIZE ((FREE_PAGE_TABLES_SIZE * 1024) / 32)

static uint32_t page_directory[1024] __attribute__((aligned(4096)));
static page_table kernel_page_tables[KERNEL_PAGE_TABLES_SIZE] __attribute__((aligned(4096)));
static page_table free_page_tables[FREE_PAGE_TABLES_SIZE] __attribute__((aligned(4096)));
static uint32_t page_bitmap[PAGE_BITMAP_SIZE] __attribute__((aligned(8)));

extern void load_page_directory(uint32_t* page_directory);

static inline void flush_tlb_single(unsigned long addr)
{
   asm volatile("invlpg (%0)" ::"r" (addr) : "memory");
}

void paging_initialize()
{
    memset(page_directory, 0, sizeof(page_directory));
	
	uint32_t current_address = 0;
	for (uint32_t i = 0; i < KERNEL_PAGE_TABLES_SIZE; ++i)
	{
		for (uint32_t j = 0; j < 1024; ++j, ++current_address) 
			kernel_page_tables[i].table[j] = (current_address * PAGE_SIZE) | PAGE_FLAG_PRESENT | PAGE_FLAG_WRITABLE;
		page_directory[KERNEL_PAGE_DIRECTORY_START + i] = KERNEL_TO_PHYS(kernel_page_tables[i].table) | PAGE_FLAG_PRESENT | PAGE_FLAG_WRITABLE;
	}
    memset(free_page_tables, 0, sizeof(free_page_tables));
	page_directory[1023] = KERNEL_TO_PHYS(page_directory) | PAGE_FLAG_PRESENT | PAGE_FLAG_WRITABLE;

    load_page_directory((uint32_t*)(KERNEL_TO_PHYS(page_directory)));

    memset(page_bitmap, 0, sizeof(page_bitmap));
}

void* get_physical_address(void* vaddr)
{
    unsigned long pd_index = (unsigned long)vaddr >> 22;
    unsigned long pt_index = (unsigned long)vaddr >> 12 & 0x03FF;

    unsigned long* pd = (unsigned long*)0xFFFFF000;
    if (!(pd[pd_index] & PAGE_FLAG_PRESENT)) return NULL;

    unsigned long* pt = ((unsigned long*)0xFFC00000) + (1024 * pd_index);
    if (!(pt[pt_index] & PAGE_FLAG_PRESENT)) return NULL;

    return (void*)((pt[pt_index] & ~0xFFF) + ((unsigned long)vaddr & 0xFFF));
}

void map_page(void* paddr, void* vaddr, page_table* page_tables, size_t page_tables_length, uint16_t flags)
{
    paddr = (void*)((unsigned long)paddr & ~0xFFF);
    vaddr = (void*)((unsigned long)vaddr & ~0xFFF);

    unsigned long pd_index = (unsigned long)vaddr >> 22;
    unsigned long pt_index = (unsigned long)vaddr >> 12 & 0x03FF;

    //printf("map_page: pd: %u, pt: %u\n", pd_index, pt_index);

    unsigned long* pd = (unsigned long*)0xFFFFF000;

    if (!(pd[pd_index] & PAGE_FLAG_PRESENT)) 
    {
        if (pd_index >= page_tables_length)
        {
            puts("map_page: Can't allocate Page Table!");
            abort();
        }
        pd[pd_index] = (KERNEL_TO_PHYS((unsigned long)page_tables[pd_index].table)) | PAGE_FLAG_USER | PAGE_FLAG_WRITABLE | PAGE_FLAG_PRESENT;
    }

    unsigned long* pt = ((unsigned long*)0xFFC00000) + (1024 * pd_index);

    // TODO: Add a way to specify user pages (and use it in the userspace libc malloc through a syscall)
    pt[pt_index] = ((unsigned long)paddr) | (flags & 0xFFF) | PAGE_FLAG_PRESENT;

    flush_tlb_single((unsigned long)vaddr);
}

void unmap_page(void* vaddr)
{
    vaddr = (void*)((unsigned long)vaddr & ~0xFFF);

    unsigned long pd_index = (unsigned long)vaddr >> 22;
    unsigned long pt_index = (unsigned long)vaddr >> 12 & 0x03FF;

    unsigned long* pd = (unsigned long*)0xFFFFF000;
    if (!(pd[pd_index] & PAGE_FLAG_PRESENT)) return;

    unsigned long* pt = ((unsigned long*)0xFFC00000) + (1024 * pd_index);
    if (!(pt[pt_index] & PAGE_FLAG_PRESENT)) return;

    pt[pt_index] &= ~PAGE_FLAG_PRESENT;

    flush_tlb_single((unsigned long)vaddr);
}

void* alloc_page(uint16_t flags)
{
    for (uint32_t bitmap_index = 0; bitmap_index < PAGE_BITMAP_SIZE; ++bitmap_index)
    {
        if (page_bitmap[bitmap_index] != UINT32_MAX)
        {
            uint8_t bit_index = 31;
            while (bit_index > 0)
            {
                uint8_t current_bit = (page_bitmap[bitmap_index] >> bit_index) & 0b1;
                if (current_bit == 0)
                {
                    uint32_t page_index = (bitmap_index * 32) + (31 - bit_index);
                    uint32_t page_offset = page_index * PAGE_SIZE;
                    if (page_offset >= mmap_free_memory_length) 
                    {
                        puts("OUT OF MEMORY");
                        abort();
                    }

                    uint32_t page_physical_address = (uint32_t)mmap_free_memory + page_offset;
                    uint32_t page_virtual_address = page_offset + PAGE_SIZE;

                    map_page((void*)page_physical_address, (void*)page_virtual_address, free_page_tables, FREE_PAGE_TABLES_SIZE, flags);
                    page_bitmap[bitmap_index] |= 1 << bit_index;
                    return (void*)page_virtual_address;
                }
                --bit_index;
            }
        }
    }

    return NULL;
}

void free_page(void* page_addr)
{
    uint32_t page_offset = (uint32_t)page_addr;
    uint32_t page_index = page_offset / PAGE_SIZE;
    uint32_t bitmap_index = (page_index & ~63) / 64;
    uint8_t bit_index = (63 - (page_index & 63));

    unmap_page(page_addr);
    page_bitmap[bitmap_index] &= ~((uint64_t)1 << bit_index);
}

void* alloc_pages(uint32_t pages, uint16_t flags)
{
    void* ptr = NULL;
    for (uint32_t i = 0; i < pages; ++i)
    {
        void* page_ptr = alloc_page(flags);
        if (ptr == NULL) ptr = page_ptr;
    }
    return ptr;
}

void free_pages(void* pages_addr, uint32_t pages)
{
    for (uint32_t i = 0; i < pages; ++i)
        free_page((void*)((uint32_t)pages_addr + (i * PAGE_SIZE)));
}

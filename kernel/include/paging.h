#pragma once

#include <stdcommon.h>

#include "terminal.h"

typedef struct
{
    uint32_t table[1024] __attribute__((aligned(4096)));
} page_table;

#define PAGE_SIZE 4096

#define PAGE_FLAG_PRESENT 0b00000001
#define PAGE_FLAG_WRITABLE 0b00000010
#define PAGE_FLAG_USER 0b00000100
#define PAGE_FLAG_WRITE_THROUGH 0b00001000
#define PAGE_FLAG_CACHE_DISABLE 0b00010000
#define PAGE_FLAG_ACCESSED 0b00100000

#define PAGE_DIRECTORY_FLAG_4MB 0b01000000

#define PAGE_TABLE_FLAG_DIRTY 0b01000000
#define PAGE_TABLE_FLAG_GLOBAL 0b10000000

void paging_initialize();

void* get_physical_address(void* vaddr);

void map_page(void* paddr, void* vaddr, page_table* page_tables, size_t page_tables_length, uint16_t flags);
void unmap_page(void* vaddr);

void* alloc_page(uint16_t flags);
void free_page(void* page_addr);
void* alloc_pages(uint32_t pages, uint16_t flags);
void free_pages(void* pages_addr, uint32_t pages);

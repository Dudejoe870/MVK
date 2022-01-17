#include <stdcommon.h>

#ifdef __is_libk
#include "paging.h"

// TODO: Spin lock

int liballoc_lock()
{
    asm volatile ( "cli" );
    return 0;
}

int liballoc_unlock()
{
    asm volatile ( "sti" );
    return 0;
}

void* liballoc_alloc(size_t pages)
{
    return alloc_pages((uint32_t)pages, PAGE_FLAG_WRITABLE);
}

int liballoc_free(void* ptr, size_t pages)
{
    free_pages(ptr, (uint32_t)pages);
    return 0;
}
#else
// TODO: User space libc
int liballoc_lock()
{
    return -1;
}

int liballoc_unlock()
{
    return -1;
}

void* liballoc_alloc(size_t pages)
{
    return NULL;
}

int liballoc_free(void* ptr, size_t pages)
{
    return -1;
}
#endif
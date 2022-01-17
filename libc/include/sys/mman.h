#ifndef _MMAN_H
#define _MMAN_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdcommon.h>

#include "sys/types.h"

#ifdef __is_libk
#include "paging.h"
#endif

void* mmap(void* addr, size_t length, int prot, int flags, int fd, off_t offset);
int munmap(void* addr, size_t length);

#ifdef __cplusplus
}
#endif

#endif
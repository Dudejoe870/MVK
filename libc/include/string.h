#ifndef _STRING_H
#define _STRING_H 1

#include <sys/cdefs.h>
#include <stdcommon.h>

#ifdef __cplusplus
extern "C" {
#endif

size_t strlen(const char* str);

void* memcpy(void* destination, const void* source, size_t num);
void memset(void* ptr, int value, size_t num);

#ifdef __cplusplus
}
#endif

#endif

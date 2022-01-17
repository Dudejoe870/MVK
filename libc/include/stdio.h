#ifndef _STDIO_H
#define _STDIO_H 1

#ifdef __cplusplus
extern "C" {
#endif

#include <stdcommon.h>

#define EOF (-1)

#ifdef __is_libk
#include "terminal.h"
#include "multiboot.h"

void stdio_initialize(multiboot_info_t* mbi);
#endif

int putchar(int character);
int puts(const char* str);

int printf(const char* format, ...);

#ifdef __cplusplus
}
#endif

#endif

#ifndef _STDLIB_H
#define _STDLIB_H 1

#include <sys/cdefs.h>
#include <stdcommon.h>

#ifdef __cplusplus
extern "C" {
#endif

#define RAND_MAX 32767

void itoa(int value, char* str, int base);

void abort(void) __attribute__((noreturn));

int rand(void);
void srand(unsigned int seed);

void* malloc(size_t size);
void* calloc(size_t num, size_t size);
void* realloc(void* ptr, size_t size);
void free(void* ptr);

#ifdef __cplusplus
}
#endif

#endif

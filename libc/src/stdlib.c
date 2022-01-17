#include "stdlib.h"

#include "string.h"
#include "liballoc.h"

void itoa(int value, char* str, int base)
{
    char* p = str;
    char* p1, *p2;
    unsigned long ud = value;
    int divisor = base;

    if (base == 10 && value < 0)
    {
        *p++ = '-';
        str++;
        ud = -value;
    }

    do
    {
        int remainder = ud % divisor;
        
        *p++ = (remainder < 10) ? remainder + '0' : remainder + 'a' - 10;
    }
    while (ud /= divisor);

    *p = 0;

    p1 = str;
    p2 = p - 1;
    while (p1 < p2)
    {
        char tmp = *p1;
        *p1 = *p2;
        *p2 = tmp;
        p1++;
        p2--;
    }
}

void abort(void)
{
    #ifdef __lib_k
    while (true) asm volatile ( "hlt" );
    #else
    while (true); // TODO: Abort process
    #endif
}

static unsigned int next = 1;

int rand(void)
{
    next = next * 1103515245 + 12345;
    return (unsigned int)(next / 65536) % 32768;
}

void srand(unsigned int seed)
{
    next = seed;
}

void* malloc(size_t size)
{
    return la_malloc(size);
}

void* calloc(size_t num, size_t size)
{
    return la_calloc(num, size);
}

void* realloc(void* ptr, size_t size)
{
    return la_realloc(ptr, size);
}

void free(void* ptr)
{
    la_free(ptr);
}

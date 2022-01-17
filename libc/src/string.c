#include "string.h"

size_t strlen(const char* str) 
{
	size_t len = 0;
	while (str[len])
		len++;
	return len;
}

void* memcpy(void* destination, const void* source, size_t num)
{
    for (; num > 0; ++destination, ++source, --num) 
        *(uint8_t*)destination = *(uint8_t*)source;
    return destination;
}

void memset(void* ptr, int value, size_t num)
{
    for (; num > 0; ++ptr, --num) 
        *(uint8_t*)ptr = (uint8_t)value;
}

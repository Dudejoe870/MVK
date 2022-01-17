#include "stdio.h"

#include "stdlib.h"

#include "ctype.h"

#ifdef __is_libk
#include "vga.h"

static terminal stdterm;
static uint8_t stdterm_current_color;

void stdio_initialize(multiboot_info_t* mbi)
{
    terminal_initialize(&stdterm, mbi);

    stdterm_current_color = TERMINAL_DEFAULT_COLOR;
}
#else
static bool is_init = false;

static void stdio_initialize(void)
{
    // TODO: Initialize FILE streams.

    is_init = true;
}
#endif

int putchar(int character)
{
    #ifdef __is_libk
    terminal_putchar(&stdterm, (char)character, stdterm_current_color);
    return character;
    #else
    if(!is_init) stdio_initialize();
    // TODO: Put character Syscall from user mode.
    return character;
    #endif
}

static int _puts(const char* str)
{
    int characters = 0;
    for (int i = 0;; ++i)
    {
        if (str[i] == '\0') break;

        putchar((int)str[i]);
        ++characters;
    }
    return characters;
}

int puts(const char* str)
{
    int ret = _puts(str);
    putchar('\n');
    return ret;
}

static int _uputs(const char* str)
{
    int characters = 0;
    for (int i = 0;; ++i)
    {
        if (str[i] == '\0') break;

        putchar(toupper((int)str[i]));
        ++characters;
    }
    return characters;
}

static void _uitoa(unsigned int value, char* str, int base)
{
    char* p = str;
    char* p1, *p2;
    unsigned long ud = value;
    int divisor = base;

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

static void _pitoa(void* value, char* str)
{
    char* p = str;
    char* p1, *p2;
    unsigned long ud = (unsigned long)value;
    int divisor = 16;

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

int printf(const char* format, ...)
{
    #ifndef __is_libk
    if(!is_init) stdio_initialize();
    #endif

    va_list arg;
    va_start(arg, format);

    int characters = 0;

    for (int i = 0;; ++i)
    {
        if (format[i] == '\0') break;

        if (format[i] == '%')
        {
            ++i;

            // TODO: Implement other features like flags and precision.

            char buf[128];
            switch (format[i])
            {
            default:
                break;
            case 'd':
            case 'i':
                itoa(va_arg(arg, int), buf, 10);
                characters += _puts(buf);
                break;
            case 'u':
                _uitoa(va_arg(arg, unsigned int), buf, 10);
                characters += _puts(buf);
                break;
            case 'x':
                _uitoa(va_arg(arg, unsigned int), buf, 16);
                characters += _puts(buf);
                break;
            case 'X':
                _uitoa(va_arg(arg, unsigned int), buf, 16);
                characters += _uputs(buf);
                break;
            case 'p':
                _pitoa(va_arg(arg, void*), buf);
                characters += _puts(buf);
                break;
            case 's':
                characters += _puts(va_arg(arg, const char*));
                break;
            case 'c':
                putchar(va_arg(arg, int));
                ++characters;
                break;
            }
            continue;
        }

        putchar((int)format[i]);
    }

    va_end(arg);

    return characters;
}

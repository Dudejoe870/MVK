#pragma once

#include "multiboot.h"

#include <stddef.h>

typedef enum
{
    TERMINAL_EGA_TEXT_MODE
} terminal_mode;

typedef struct
{
    terminal_mode mode;

    size_t width;
    size_t height;

    size_t char_width;
    size_t char_height;

    size_t x;
    size_t y;

    void* buffer;
} terminal;

#include <stdcommon.h>

void terminal_initialize(terminal* term, multiboot_info_t* mbi);
void terminal_writechar(terminal* term, char c, uint8_t color, size_t x, size_t y);
void terminal_putchar(terminal* term, char c, uint8_t color);
void terminal_write(terminal* term, const char* data, size_t size, uint8_t color);
void terminal_writestring(terminal* term, const char* data, uint8_t color);
void terminal_scrolldown(terminal* term);

#define TERMINAL_DEFAULT_COLOR VGA_COLOR(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK)

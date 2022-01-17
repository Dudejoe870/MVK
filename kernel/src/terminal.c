#include "terminal.h"

#include "vga.h"
#include "paging.h"
#include "util.h"

void terminal_initialize(terminal* term, multiboot_info_t* mbi) 
{
    if (!CHECK_FLAG(mbi->flags, 12)) abort();

    term->mode = 0;

	term->x = 0;
	term->y = 0;
    
    switch (mbi->framebuffer_type)
    {
    default:
        abort();
        break;
    case MULTIBOOT_FRAMEBUFFER_TYPE_EGA_TEXT:
        term->mode = TERMINAL_EGA_TEXT_MODE;

        term->buffer = (void*)(PHYS_TO_KERNEL(mbi->framebuffer_addr));
        
        term->width = mbi->framebuffer_width;
        term->height = mbi->framebuffer_height;

        term->char_width = 1;
        term->char_height = 1;
        for (size_t y = 0; y < term->height; ++y) 
            for (size_t x = 0; x < term->width; ++x) 
                ((uint16_t*)term->buffer)[y * term->width + x] = VGA_ENTRY(' ', TERMINAL_DEFAULT_COLOR);
        terminal_writestring(term, "terminal_initialize: Initialized terminal (FRAMEBUFFER_TYPE_EGA_TEXT)\n", TERMINAL_DEFAULT_COLOR);
        break;
    }

    char buf[16];

    terminal_writestring(term, "  Framebuffer Address 0x", TERMINAL_DEFAULT_COLOR);
    itoa((int)term->buffer, buf, 16);
    terminal_writestring(term, buf, TERMINAL_DEFAULT_COLOR);
    terminal_writestring(term, "\n", TERMINAL_DEFAULT_COLOR);
}

void terminal_putentryat(terminal* term, char c, uint8_t color, size_t x, size_t y) 
{
    switch (term->mode)
    {
    default:
        return;
    case TERMINAL_EGA_TEXT_MODE:
	    ((uint16_t*)term->buffer)[y * term->width + x] = VGA_ENTRY(c, color);
        return;
    }
}

void terminal_putchar(terminal* term, char c, uint8_t color) 
{
    switch (c)
    {
    case '\n':
        term->x = 0;
        term->y += term->char_height;
        if (term->y >= term->height)
        {
            --term->y;
            terminal_scrolldown(term);
        }
        return;
    }

	terminal_putentryat(term, c, color, term->x, term->y);

    term->x += term->char_width;
	if (term->x >= term->width) 
    {
		term->x = 0;

        term->y += term->char_height;
		if (term->y >= term->height)
        {
            --term->y;
            terminal_scrolldown(term);
        }
	}
}

void terminal_write(terminal* term, const char* data, size_t size, uint8_t color) 
{
	for (size_t i = 0; i < size; ++i)
        terminal_putchar(term, data[i], color);
}

void terminal_writestring(terminal* term, const char* data, uint8_t color) 
{
	terminal_write(term, data, strlen(data), color);
}

void terminal_scrolldown(terminal* term)
{
    switch (term->mode)
    {
    case TERMINAL_EGA_TEXT_MODE:
        for (size_t y = 1; y < term->height; ++y)
            memcpy(&((uint16_t*)term->buffer)[term->width * (y-1)], &((uint16_t*)term->buffer)[term->width * y], term->width * 2);
        for (size_t x = 0; x < term->width; ++x) ((uint16_t*)term->buffer)[((term->height - 1) * term->width) + x] = VGA_ENTRY(' ', TERMINAL_DEFAULT_COLOR);
        term->x = 0;
        break;
    }
}

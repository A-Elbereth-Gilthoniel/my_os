#include "macros.h"
#include "terminal.h"

//=========================================================

void kernel_main(void)
{
    terminal monitor;
    terminal_initialize(&monitor);
    terminal_writestring(&monitor, "Hello");
}

//=========================================================

static inline uint16_t vga_entry(unsigned char symb, uint8_t color)
{
    return ((uint16_t) symb) | ((uint16_t) color << 8);
}

//=========================================================

static inline uint8_t vga_entry_color(enum vga_color front_color, enum vga_color background_color)
{
    return (front_color | (background_color << 4));
}

//=========================================================

void terminal_initialize(terminal* monitor)
{
    monitor->row = 0;
    monitor->column = 0;
    monitor->color = vga_entry_color(VGA_COLOR_CYAN, VGA_COLOR_BLACK);
    monitor->buffer = (uint16_t*)VGA_MEMORY;

    for (int y = 0; y < HEIGHT; y++)
    {
        int index = y * WIDTH;
        for (int x = 0; x < HEIGHT; x++)
        {
            index++;
            monitor->buffer[index] = vga_entry(' ', monitor->color);
        }
    }
}

//=================================================

void terminal_writestring(terminal* monitor, const char* text)
{
    size_t str_size = strlen(text);
    for (size_t i = 0; i < str_size; i++)
    {
        terminal_putchar(monitor, text[i]);
    }
}

//=================================================

void terminal_putchar(terminal* monitor, char c)
{
    monitor->buffer[monitor->row * WIDTH + monitor->column] = vga_entry(c, monitor->color);
    if (++monitor->column == WIDTH)
    {
        monitor->column = 0;
        if (++monitor->row == HEIGHT)
            monitor->row = 0;
    }
}

//=================================================

size_t strlen(const char* text)
{
    size_t counter = 0;
    while (text[counter] != '\0')
    {
        counter++;
    }
    return counter;
}

//=================================================

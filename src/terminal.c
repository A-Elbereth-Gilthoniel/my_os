#include "macros.h"
#include "terminal.h"

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
        for (int x = 0; x < WIDTH; x++)
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
    if (c == '\n')
    {
        monitor->row++;
        monitor->column = 0;
        return;
    }
    if (c == '\b')
    {
        if (monitor->column == 0)
        {
            if (monitor->row == 0)
            {
                monitor->row = 0;
                monitor->column = 0;
            }
            else
            {
                monitor->row--;
                monitor->column = WIDTH;
                while ((monitor->buffer[monitor->row * WIDTH + monitor->column] == vga_entry(' ', monitor->color)) && monitor->column != 0)
                    monitor->column--;
            }
        }
        else
            monitor->column--;
        monitor->buffer[monitor->row * WIDTH + monitor->column] = vga_entry(' ', monitor->color);
        return;
    }
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

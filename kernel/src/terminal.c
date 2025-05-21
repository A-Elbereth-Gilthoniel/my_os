#include "macros.h"
#include "inline_assembly.h"
#include "terminal.h"
#include <string.h>

extern terminal* monitor;
//=========================================================

static inline uint16_t vga_entry(unsigned char symb, uint8_t color)
{
    return ((uint16_t) symb) | ((uint16_t) color << 8);
}

//=========================================================

static inline char vga_decode(uint16_t pixel)
{
    return (char)(pixel & 0x00FF);
}

//=========================================================

uint8_t vga_entry_color(enum vga_color front_color, enum vga_color background_color)
{
    return (front_color | (background_color << 4));
}

//=========================================================

void terminal_initialize()
{
    monitor->row = 0;
    monitor->column = 0;
    monitor->color = vga_entry_color(VGA_COLOR_CYAN, VGA_COLOR_BLACK);
    monitor->buffer = (uint16_t*)VGA_MEMORY;
    monitor->command[0] = '\0';

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

void terminal_writestring(const char* text)
{
    size_t str_size = strlen(text);
    for (size_t i = 0; i < str_size; i++)
    {
        terminal_putchar(text[i]);
    }
    return;
}

//=================================================

void terminal_putchar(char c)
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
        move_cursor(monitor->row * WIDTH + monitor->column + 1);
        return;
    }
    monitor->buffer[monitor->row * WIDTH + monitor->column] = vga_entry(c, monitor->color);
    move_cursor(monitor->row * WIDTH + monitor->column + 1);
    if (++monitor->column == WIDTH)
    {
        monitor->column = 0;
        if (++monitor->row == HEIGHT)
            monitor->row = 0;
    }
}

//=================================================

char* uint_to_string(unsigned int value) {
    static char buffer[51];
    int i = 0;

    if (value == 0) {
        buffer[0] = '0';
        buffer[1] = '\0';
        return buffer;
    }

    while (value > 0 && i < 50) {
        buffer[i++] = '0' + (value % 10);
        value /= 10;
    }

    buffer[i] = '\0';

    int start = 0;
    int end = i - 1;
    while (start < end) {
        char tmp = buffer[start];
        buffer[start] = buffer[end];
        buffer[end] = tmp;
        start++;
        end--;
    }

    return buffer;
}

//===================================================

void terminal_write(const char* data, size_t size)
{
	for (size_t i = 0; i < size; i++)
		terminal_putchar(data[i]);
}

//===================================================

void getline(char* buffer, uint32_t row, uint32_t n)
{
    for (int i = 0; i < n; i++)
        buffer[i] =  vga_decode(monitor->buffer[row * WIDTH + i]);
    buffer[n] = '\0';
}

//===================================================

void move_cursor(uint16_t position)
{
    outb(VGA_COMMAND_PORT, 0x0F);
    outb(VGA_DATA_PORT, (uint8_t)(position & 0xFF));
    outb(VGA_COMMAND_PORT, 0x0E);
    outb(VGA_DATA_PORT, (uint8_t)((position >> 8) & 0xFF));
}

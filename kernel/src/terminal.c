#include "macros.h"
#include "terminal.h"

extern terminal monitor;
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

void terminal_initialize(terminal* monitor1)
{
    monitor1->row = 0;
    monitor1->column = 0;
    monitor1->color = vga_entry_color(VGA_COLOR_CYAN, VGA_COLOR_BLACK);
    monitor1->buffer = (uint16_t*)VGA_MEMORY;

    for (int y = 0; y < HEIGHT; y++)
    {
        int index = y * WIDTH;
        for (int x = 0; x < WIDTH; x++)
        {
            index++;
            monitor1->buffer[index] = vga_entry(' ', monitor1->color);
        }
    }
}

//=================================================

void terminal_writestring(terminal* monitor1, const char* text)
{
    size_t str_size = strlen(text);
    for (size_t i = 0; i < str_size; i++)
    {
        terminal_putchar(monitor1, text[i]);
    }
    return;
}

//=================================================

void terminal_putchar(terminal* monitor1, char c)
{
    if (c == '\n')
    {
        monitor1->row++;
        monitor1->column = 0;
        return;
    }
    if (c == '\b')
    {
        if (monitor1->column == 0)
        {
            if (monitor1->row == 0)
            {
                monitor1->row = 0;
                monitor1->column = 0;
            }
            else
            {
                monitor1->row--;
                monitor1->column = WIDTH;
                while ((monitor1->buffer[monitor1->row * WIDTH + monitor1->column] == vga_entry(' ', monitor1->color)) && monitor1->column != 0)
                    monitor1->column--;
            }
        }
        else
            monitor1->column--;
        monitor1->buffer[monitor1->row * WIDTH + monitor1->column] = vga_entry(' ', monitor1->color);
        return;
    }
    monitor1->buffer[monitor1->row * WIDTH + monitor1->column] = vga_entry(c, monitor1->color);
    if (++monitor1->column == WIDTH)
    {
        monitor1->column = 0;
        if (++monitor1->row == HEIGHT)
            monitor1->row = 0;
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

char* uint_to_string(unsigned int value) {
    static char buffer[51]; // Статический буфер (всегда возвращает один и тот же)
    int i = 0;

    // Обработка нуля
    if (value == 0) {
        buffer[0] = '0';
        buffer[1] = '\0';
        return buffer;
    }

    // Преобразуем число в строку в обратном порядке
    while (value > 0 && i < 50) {
        buffer[i++] = '0' + (value % 10);
        value /= 10;
    }

    buffer[i] = '\0';

    // Реверс строки
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
		terminal_putchar(&monitor, data[i]);
}

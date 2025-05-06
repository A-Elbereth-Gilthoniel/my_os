#include "kernel.h"

//=========================================================

extern char stack_top;

void kernel_main(void)
{
    terminal monitor;
    terminal_initialize(&monitor);
    uint32_t* stack = &stack_top;
    char last_symb;
    while (1)
    {
        char c = keyboard_handler();
        if (c != '\0' && c != last_symb)
        {
            terminal_putchar(&monitor, c);
            move_cursor(monitor.row * WIDTH + monitor.column);
        }
        last_symb = c;
    }

}

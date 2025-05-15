#include "kernel.h"

terminal monitor;

//=========================================================

void aaa(void)
{
    get_esp();
    // uint32_t esp;
    // __asm__ volatile ("mov %%esp, %0" : "=r"(esp));  // Сохраняем значение ESP в переменную
    // terminal_writestring(&monitor, "%esp = ");
    // terminal_writestring(&monitor, uint_to_string(esp));
    // terminal_writestring(&monitor, "\n");
}

void kernel_main(void)
{
    terminal_initialize(&monitor);
    heap_init();
    terminal_writestring(&monitor, "kernel_main:\n");
    get_esp();
    // aaa();
    get_esp();
//     char* s = (char*) alloc(10);
//     s[0] = 'h'; s[1] = 'e'; s[2] = 'l'; s[3] = 'l'; s[4] = 'o'; s[5] = '\0';
//
//     char* s2 = (char*) alloc(6);
//     s2[0] = 'h'; s2[1] = 'e'; s2[2] = 'l'; s2[3] = 'l'; s2[4] = 'o'; s2[5] = '\0';
//
//     char* s3 = (char*) alloc(6);
//     s3[0] = 'h'; s3[1] = 'e'; s3[2] = 'l'; s3[3] = 'l'; s3[4] = 'o'; s3[5] = '\0';
//
//     terminal_writestring(&monitor, s);
//     terminal_writestring(&monitor, s2);
//     terminal_writestring(&monitor, s3);
//     terminal_writestring(&monitor, uint_to_string(sizeof(block_header)));
//     terminal_writestring(&monitor, "\n");
//     terminal_writestring(&monitor, uint_to_string(s));
//     terminal_writestring(&monitor, "\n");
//     terminal_writestring(&monitor, uint_to_string(s2));
//     terminal_writestring(&monitor, "\n");
//     terminal_writestring(&monitor, uint_to_string(s3));
    // terminal_putchar(&monitor, 'f');
    // terminal_writestring(&monitor, "hei2");
    create_thread(&thread1);
    create_thread(&thread2);
//
    thread_start();
    // get_esp();
    // uint32_t* stack = &stack_top;
    char last_symb;
    while (1)
    {
        // terminal_writestring(&monitor, "hei2");
        // char c = keyboard_handler();
        // if (c != '\0' && c != last_symb)
        // {
        //     terminal_putchar(&monitor, c);
        //     move_cursor(monitor.row * WIDTH + monitor.column);
        // }
        // last_symb = c;
    }

}

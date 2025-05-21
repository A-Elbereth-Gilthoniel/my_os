#include "kernel.h"
#include <stdio.h>
#include <string.h>

terminal* monitor;

//=========================================================

void kernel_main(void)
{
    monitor = (terminal*) alloc(sizeof(terminal));
    terminal_initialize();
    heap_init();

    create_thread(&keyboard);
    create_thread(&command_handler);
    thread_start();
}

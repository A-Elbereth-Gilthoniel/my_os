#include "macros.h"
#include "inline_assembly.h"
#include "keyboard.h"
#include "terminal.h"

extern terminal* monitor;

char scancode_table[128] = {
    0,  27, '1','2','3','4','5','6','7','8','9','0','-','=','\b',  // 0x00–0x0F
    '\t','q','w','e','r','t','y','u','i','o','p','[',']','\n',    // 0x10–0x1D
    0,  'a','s','d','f','g','h','j','k','l',';','\'','`',         // 0x1E–0x2A
    0,  '\\','z','x','c','v','b','n','m',',','.','/',0,           // 0x2B–0x37
    '*', 0,  ' ', 0,  0,  0,  0,  0,  0,  0,  0,  0,              // 0x38–0x45
    0,   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,                   // 0x46–0x50
    0,   0,  0,  0,  0,  0,  0,  0                                // 0x51–0x58
};

//===========================================

void keyboard(void)
{
    char last_symb;
    while (1)
    {
        char c = keyboard_handler();
        if (c != '\0' && c != last_symb)
        {
            if (c == '\n')
                getline(monitor->command, monitor->row, monitor->column);
            terminal_putchar(c);
            move_cursor(monitor->row * WIDTH + monitor->column);
        }
        last_symb = c;
        schedule();
    }
}

//===========================================

char keyboard_handler()
{
    uint8_t scancode = inb(KEYBOARD_PORT);
    outb(0x20, 0x20);
    char c = '\0';
    if (!(scancode & 0x80))
        c = scancode_table[scancode];
    return c;
}

//=========================================

void thread1(void)
{
    terminal_writestring("thread1 started");
    while (1) {
        for (volatile int i = 0; i < 1000000; ++i);
        schedule();
    }
}

//==================================================

void thread2(void) {
    terminal_writestring("thread2 started");
    while (1) {
        for (volatile int i = 0; i < 1000000; ++i);
        schedule();
    }
}

//==================================================

inline uint32_t get_esp(void) {
    uint32_t esp;
    __asm__ volatile ("mov %%esp, %0" : "=r"(esp));
    return esp;
}

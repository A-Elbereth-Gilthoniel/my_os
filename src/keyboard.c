#include "macros.h"
#include "inline_assembly.h"
#include "keyboard.h"
#include "terminal.h"

extern terminal monitor;

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

char keyboard_handler()
{
    // terminal_putchar(&monitor, 'd');
    uint8_t scancode = inb(KEYBOARD_PORT);
    outb(0x20, 0x20);
    char c = '\0';
    if (!(scancode & 0x80))
        c = scancode_table[scancode];
    // terminal_putchar(&monitor, c);
    // outb(PIC1_COMMAND, PIC1_COMMAND);
    return c;
}

//========================================

void move_cursor(uint16_t position)
{
    outb(VGA_COMMAND_PORT, 0x0F);
    outb(VGA_DATA_PORT, (uint8_t)(position & 0xFF));
    outb(VGA_COMMAND_PORT, 0x0E);
    outb(VGA_DATA_PORT, (uint8_t)((position >> 8) & 0xFF));
}

//=========================================

void thread1()
{
    terminal_writestring(&monitor, "thread1 started");
    while (1) {
        terminal_writestring(&monitor, "Thread 1");
        for (volatile int i = 0; i < 1000000; ++i);
        schedule();
    }
    terminal_writestring(&monitor, "thread1 END");
}

//==================================================

void thread2() {
    terminal_writestring(&monitor, "thread2 started");
    while (1) {
        terminal_writestring(&monitor, "Thread 2");
        for (volatile int i = 0; i < 1000000; ++i);
        schedule();
    }
}

//==================================================

inline uint32_t get_esp(void) {
    uint32_t esp;
    __asm__ volatile ("mov %%esp, %0" : "=r"(esp));  // Сохраняем значение ESP в переменную
    // terminal_writestring(&monitor, "%esp = ");
    // terminal_writestring(&monitor, uint_to_string(esp));
    // terminal_writestring(&monitor, "\n");
    return esp
}

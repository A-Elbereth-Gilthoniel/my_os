#include "macros.h"

#define PIC1_COMMAND 0x20
#define PIC1_DATA    0x21
#define PIC2_COMMAND 0xA0
#define PIC2_DATA    0xA1
#define KEYBOARD_PORT 0x60
#define MOUSE_PORT       0x60
#define MOUSE_COMMAND    0x64
#define MOUSE_WRITE      0xD4
#define ENABLE_AUX_MOUSE 0xA8
#define ENABLE_PACKET    0xF4
#define VGA_COMMAND_PORT 0x3D4
#define VGA_DATA_PORT    0x3D5


void outb(uint16_t port, uint8_t val);
uint8_t inb(uint16_t port);
void io_wait(void);

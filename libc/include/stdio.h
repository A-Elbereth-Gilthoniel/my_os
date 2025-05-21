#ifndef _STDIO_H
#define _STDIO_H

int printf(const char* __restrict, ...);
int putchar(int);
char* utoa(unsigned int value, char* buffer);
char* itoa(int value, char* buffer);

#endif

#ifndef _KERNEL_TTY_H
#define _KERNEL_TTY_H

#include <stddef.h>

// not exposing everything
void terminal_initialize(void);
void terminal_putchar(char c);
void terminal_write(const char* data, size_t size); //bad
void terminal_writestring(const char* data);

#endif

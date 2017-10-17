#include <stdio.h>
#include <nonstandard.h>
#include <kernel/tty.h>
#include <arch/i386/init.h>
#include <stdint.h>

void print_mem(char * string) {
    char memstr[255];
    terminal_writestring("String at 0x");
    terminal_writestring(itoa((int)string, memstr, 16));
    terminal_writestring(": ");
    terminal_writestring(string);
    terminal_writestring("\n");
}

void kernel_main(void) {
    // initialize terminal
    terminal_initialize();
    //very_bad_u16toa(aaa, *(terminal_buffer+14));
    terminal_writestring("welcome i guess\n\n");

    char * w;
    w = (char *)0xBBBB;
    w[0] = 'a';
    w[1] = 'w';
    w[2] = 'a';
    w[3] = 'w';
    w[4] = '\0';
    terminal_writestring("printing w: ");
    terminal_writestring((char *)0xBBBB);
    terminal_writestring("\n");
    print_mem((char *) 0xBBBB);

    init_descriptor_tables();
    
    print_mem((char *) 0xBBBB);

    
    itoa(10/0, w, 10);
}



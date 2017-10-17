//
// isr.c -- High level interrupt service routines and interrupt request handlers.
// Part of this code is modified from Bran's kernel development tutorials.
// Rewritten for JamesM's kernel development tutorials.
//

#include "isr_handler.h"
#include <string.h>
#include <kernel/tty.h>
#include <nonstandard.h>

static const char *exception_messages[32] = {
	"Division by zero",
	"Debug",
	"Non-maskable interrupt",
	"Breakpoint",
	"Detected overflow",
	"Out-of-bounds",
	"Invalid opcode",
	"No coprocessor",
	"Double fault",
	"Coprocessor segment overrun",
	"Bad TSS",
	"Segment not present",
	"Stack fault",
	"General protection fault",
	"Page fault",
	"Unknown interrupt",
	"Coprocessor fault",
	"Alignment check",
	"Machine check",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved"
};

// This gets called from our ASM interrupt handler stub.
void isr_handler(registers_t *r) {
    char string[20];
    itoa(r->int_no, string, 16);
    terminal_writestring("interrupt: 0x");
    terminal_writestring(string);
    terminal_writestring(" - ");
    terminal_writestring(exception_messages[r->int_no]);
    terminal_writestring("\n");
    for(;;);

    return;
}


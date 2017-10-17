#include <kernel/tty.h>
#include "include/init.h"
#include "gdt.h"
#include "idt.h"

void init_descriptor_tables()
{
   // Initialise the global descriptor table.
   init_gdt();
   terminal_writestring("GDT initialized\n");
   init_idt();
   terminal_writestring("IDT initialized\n");
   init_irq();
   terminal_writestring("IRQ initialized\n");
}

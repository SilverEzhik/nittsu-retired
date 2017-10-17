#include <stdint.h>
#include "isr_handler.h"
#include "idt.h"

// array of function pointers
void *irq_routines[16] = {
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0
};

void irq_install_handler(int irq, void (*handler)(registers_t *r))
{
    irq_routines[irq] = handler;
}
void irq_uninstall_handler(int irq)
{
        irq_routines[irq] = 0;
}

// originally, IRQ 0-7 are mapped to IDT entries 8-15, which will mess up exception
// handling. outportb sends commands to the Programmable Interrupt Controllers in 
// order to remap the entries to 32-47.
void irq_remap(void)
{
    outportb(0x20, 0x11);
    outportb(0xA0, 0x11);
    outportb(0x21, 0x20);
    outportb(0xA1, 0x28);
    outportb(0x21, 0x04);
    outportb(0xA1, 0x02);
    outportb(0x21, 0x01);
    outportb(0xA1, 0x01);
    outportb(0x21, 0x0);
    outportb(0xA1, 0x0);
}


// remap, then install the appropriate handlers
void irq_install() {
    irq_remap();

    idt_set_gate(32, (uint32_t)irq0, 0x08, 0x8E);
    idt_set_gate(33, (uint32_t)irq1, 0x08, 0x8E);
    idt_set_gate(34, (uint32_t)irq2, 0x08, 0x8E);
    idt_set_gate(35, (uint32_t)irq3, 0x08, 0x8E);
    idt_set_gate(36, (uint32_t)irq4, 0x08, 0x8E);
    idt_set_gate(37, (uint32_t)irq5, 0x08, 0x8E);
    idt_set_gate(38, (uint32_t)irq6, 0x08, 0x8E);
    idt_set_gate(39, (uint32_t)irq7, 0x08, 0x8E);
    idt_set_gate(40, (uint32_t)irq8, 0x08, 0x8E);
    idt_set_gate(41, (uint32_t)irq9, 0x08, 0x8E);
    idt_set_gate(42, (uint32_t)irq10, 0x08, 0x8E);
    idt_set_gate(43, (uint32_t)irq11, 0x08, 0x8E);
    idt_set_gate(44, (uint32_t)irq12, 0x08, 0x8E);
    idt_set_gate(45, (uint32_t)irq13, 0x08, 0x8E);
    idt_set_gate(46, (uint32_t)irq14, 0x08, 0x8E);
    idt_set_gate(47, (uint32_t)irq15, 0x08, 0x8E);
}

// Each IRQ ISR points to this function. 
// IRQ controllers need to be told when you are done servicing them,
// so we have to send them the "End of Interrupt" command (0x20).
// First controller is located at 0x20, second is at 0xA0.
// If second (slave) controller gets an interrupt, it has to be
// acknowledged on both controllers. 
void irq_handler(registers_t *r) {
    // blank function pointer
    void (*handler)(registers_t *r);

    // if custom handle is available, run it
    handler = irq_routines[r->int_no - 32];
    if (handler != 0) {
        handler(r);
    }

    // if IDT entry >= 40 (IRQ8-15), send EOI to slave controller
    if (r->int_no >= 40) {
        outportb(0xA0, 0x20);
    }

    // send EOI to master controller
    outportb(0x20, 0x20);
}

void init_irq() {
    irq_install();
}

#include <stdint.h>

void init_idt();
void idt_set_gate(uint8_t,uint32_t,uint16_t,uint8_t);

// A struct describing an interrupt gate.
struct idt_entry_struct
{
   uint16_t base_lo;             // The lower 16 bits of the address to jump to when this interrupt fires.
   uint16_t sel;                 // Kernel segment selector.
   uint8_t  always0;             // This must always be zero.
   uint8_t  flags;               // More flags.
   uint16_t base_hi;             // The upper 16 bits of the address to jump to.
} __attribute__((packed));
typedef struct idt_entry_struct idt_entry_t;

//flags:
//[PDDSGGGG]
// P - Present - signifies the entry is present (0 for unused interrupts)
// D - DPL - Privilege level to be called from
// D
// S - Storage segment - 0 for interrupt and trap gates
// G - Gate Type - 5 possible values  
// G - 0101 - 0x5 - 80386 32 bit task gate
// G - 0110 - 0x6 - 80286 16 bit interrupt gate
// G - 0111 - 0x7 - 80286 16 bit trap gate
//    1110 - 0xE - 80386 32 bit interrupt gate
//    1111 - 0xF - 80386 32 bit trap gate

// A struct describing a pointer to an array of interrupt handlers.
// This is in a format suitable for giving to 'lidt'.
struct idt_ptr_struct
{
   uint16_t limit;
   uint32_t base;                // The address of the first element in our idt_entry_t array.
} __attribute__((packed));
typedef struct idt_ptr_struct idt_ptr_t;

// These extern directives let us access the addresses of our ASM ISR handlers.
extern void isr0 ();  //  0 - division by zero exception
extern void isr1 ();  //  1 - debug exception
extern void isr2 ();  //  2 - non maskable interrupt
extern void isr3 ();  //  3 - breakpoint exception
extern void isr4 ();  //  4 - 'into detected overflow'
extern void isr5 ();  //  5 - out of bounds exception
extern void isr6 ();  //  6 - invalid opcode exception
extern void isr7 ();  //  7 - no coprocessor exception
extern void isr8 ();  //  8 - double fault                  (pushes error code)
extern void isr9 ();  //  9 - coprocessor segment overrun
extern void isr10 (); // 10 - bad TSS                       (pushes error code)
extern void isr11 (); // 11 - segment not present           (pushes error code)
extern void isr12 (); // 12 - stack fault                   (pushes error code)
extern void isr13 (); // 13 - general protection fault      (pushes error code)
extern void isr14 (); // 14 - page fault                    (pushes error code)
extern void isr15 (); // 15 - unknown interrupt exception
extern void isr16 (); // 16 - coprocessor fault
extern void isr17 (); // 17 - alignment check exception     (pushes error code)
extern void isr18 (); // 18 - machine check exception
extern void isr19 (); // 19 - SIMD floating point exception
extern void isr20 (); // 20 - virtualization exception
extern void isr21 (); // 21 - reserved
extern void isr22 (); // 22 - reserved
extern void isr23 (); // 23 - reserved
extern void isr24 (); // 24 - reserved
extern void isr25 (); // 25 - reserved
extern void isr26 (); // 26 - reserved
extern void isr27 (); // 27 - reserved
extern void isr28 (); // 28 - reserved
extern void isr29 (); // 29 - reserved
extern void isr30 (); // 30 - security exception            (pushes error code)
extern void isr31 (); // 31 - reserved

/* thanks toaruos */

.section .text
.align 4

.macro IRQ ident byte
    .global irq\ident
    .type irq\ident, @function
    irq\ident:
        cli
        push $0x00
        push $\byte
        jmp irq_common
.endm


IRQ 0, 32
IRQ 1, 33
IRQ 2, 34
IRQ 3, 35
IRQ 4, 36
IRQ 5, 37
IRQ 6, 38
IRQ 7, 39
IRQ 8, 40
IRQ 9, 41
IRQ 10, 42
IRQ 11, 43
IRQ 12, 44
IRQ 13, 45
IRQ 14, 46
IRQ 15, 47

.extern irq_handler
.type irq_handler, @function

irq_common:
    /* Push all registers */
    pusha

    /* Save segment registers */
    push %ds
    push %es 
    push %fs
    push %gs
    mov $0x10, %ax   # load data segment descriptor 
    mov %ax, %ds     
    mov %ax, %es
    mov %ax, %fs
    mov %ax, %gs
    cld

    push %esp
    call irq_handler
    add $4, %esp
    
    /* Restore segment registers */
    pop %gs
    pop %fs
    pop %es
    pop %ds

    /* Restore registers */
    popa
    /* Cleanup error code and IRQ # */
    add $8, %esp
    sti
    /* pop CS, EIP, EFLAGS, SS and ESP */
    iret

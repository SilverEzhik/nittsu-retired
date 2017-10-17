/* thanks toaruos */

.section .text
.align 4

.macro ISR_NOERR index
    .global isr\index
    isr\index:
        cli
        push $0
        push $\index
        jmp isr_common
.endm

.macro ISR_ERR index
    .global isr\index
    isr\index:
        cli
        push $\index
        jmp isr_common
.endm

/* Standard X86 interrupt service routines */
ISR_NOERR 0     //0x00 
ISR_NOERR 1     //0x01
ISR_NOERR 2     //0x02
ISR_NOERR 3     //0x03
ISR_NOERR 4     //0x04
ISR_NOERR 5     //0x05
ISR_NOERR 6     //0x06
ISR_NOERR 7     //0x07
ISR_ERR   8     //0x08
ISR_NOERR 9     //0x09
ISR_ERR   10    //0x0a
ISR_ERR   11    //0x0b
ISR_ERR   12    //0x0c
ISR_ERR   13    //0x0d
ISR_ERR   14    //0x0e
ISR_NOERR 15    //0x0f
ISR_NOERR 16    //0x10
ISR_ERR   17    //0x11
ISR_NOERR 18    //0x12
ISR_NOERR 19    //0x13
ISR_NOERR 20    //0x14
ISR_NOERR 21    //0x15
ISR_NOERR 22    //0x16
ISR_NOERR 23    //0x17
ISR_NOERR 24    //0x18
ISR_NOERR 25    //0x19
ISR_NOERR 26    //0x1a
ISR_NOERR 27    //0x1b
ISR_NOERR 28    //0x1c
ISR_NOERR 29    //0x1d
ISR_ERR   30    //0x1e
ISR_NOERR 31    //0x1f
#ISR_NOERR 127

.extern isr_handler
.type isr_handler, @function

isr_common:
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
    call isr_handler
    add $4, %esp
    
    /* Restore segment registers */
    pop %gs
    pop %fs
    pop %es
    pop %ds

    /* Restore registers */
    popa
    /* Cleanup error code and ISR # */
    add $8, %esp
    sti
    /* pop CS, EIP, EFLAGS, SS and ESP */
    iret

/* thanks toaruos */

.section .text
.align 4

.global gdt_flush
.type gdt_flush, @function

gdt_flush:
    /* Load GDT */
    mov 4(%esp), %eax 
    lgdt (%eax) /* load the GDT pointer */

    mov $0x10, %ax  # offset to the data segment 
    mov %ax, %ds    # load all the data segment selectors
    mov %ax, %es
    mov %ax, %fs
    mov %ax, %ss

    ljmp $0x08, $.flush # 0x08 is the offset to the code segment, far jump 
.flush:
    ret # pop return address from stack and come back

; multiboot header constants
MBALIGN  equ 1<<0               ; align loaded modules on page boundaries
MEMINFO  equ 1<<1               ; provide memory map
FLAGS    equ MBALIGN | MEMINFO  ; multiboot flag field
MAGIC    equ 0x1BADB002         ; magic number for the bootloader to find header
CHECKSUM equ -(MAGIC + FLAGS)   ; checksum

; declare multiboot header that marks program as a kernel.
; magic values defined in the standard
; bootloader searches for this signature in the first 8 KiB of the kernel file, aligned to 32 KiB boundaries
; the signature is in its own section to force the header to be within the first 8 KiB of the kernel file

section .multiboot
align 4
    dd MAGIC
    dd FLAGS
    dd CHECKSUM

; the multiboot standard does not define the value of the stack pointer register (esp)
; it is up to the kernel to provide a stack. this will allocate room for a small stack
; by creating a symbol at the bottom of it, then allocating 16384 bytes for it, then
; finally creating a symbol on top. stack grows downward on x86. the stack is in its
; own section so that it can be marked nobits, making the kernel file smaller, as it 
; does not contain the uninitialized stack.
; the stack on x86 must be 16 byte-aligned according to System V ABI standard and 
; de-facto extensions. the compiler will assume that, and bad things may happen if it's not.

section .bss
align 4
stack_bottom:
resb 16384 ; 16 KiB
stack_top:

; linker script specifies _start as the entry point to the kernel.
; bootloader will jump to this position once the kernel has loaded.
; since the bootloader will be gone, it won't make sense to return.
; declare _start as a function symbol with the given symbol size.

section .text
global _start:function (_start.end - _start)

_start:
    ; the bootloader has loaded us into 32 bit protected mode
    ; interrupts and paging are disabled.
    ; processor state is as defined in the multiboot standard.
    ; kernel has full control of the cpu. kernel can only use
    ; code it provides and hardware features
    ; there is no secuity, no safeguards, no debugging.
    ; final destination

    ; set the esp register to point to the top of the stack (grows downward on x86)
    ; necessary to do in assembly - can't use C without a stack
    mov esp, stack_top

    ; this is a good place to initialize crucial processor state before high-level kernel is entered
    ; it's better to minimize this early environment where crucial features are offline.
    ; processor is not fully initialized. floating point and instruction set extensions are not initialized.

    ; global descriptor table (GDT) should be loaded here. paging should be enabled here.
    ; C++ features like global constructors and exceptions will require runtime support to work.

    ; enter high-level kernel. ABI requires the stack to be 16-byte aligned at the time of the
    ; call instruction (which pushes the return pointer of size 4 bytes).

    ; the stack was originally 16-byte aligned, and 0 is a multiple of 16 (well, it is!), so the alignment is
    ; in order and the call is well defined.
    extern kernel_main
    call kernel_main

    ; if the system is all done, put the computer in an infinite loop
    cli ; clear interrupts (although they are already disabled by the bootloader)
        ; but hey, maybe you returned from the kernel with them on... idk why, but maybe.

.hang: hlt
       jmp .hang
.end:

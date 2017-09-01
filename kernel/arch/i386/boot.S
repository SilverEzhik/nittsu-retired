# multiboot header constants
.set ALIGN,     1<<0                # align loaded modules on page boundaries
.set MEMINFO,   1<<1                # provide memory map
.set FLAGS,     ALIGN | MEMINFO     # multiboot flag field
.set MAGIC,     0x1BADB002          # magic number for the bootloader to find header
.set CHECKSUM,  -(MAGIC + FLAGS)    # checksum
    
/*
    Declare multiboot header that marks the program as a kernel using the magic
    values defined in the standard. Bootloader searches for this signature in 
    the first 8 KiB of the kernel file, aligned to 32 KiB boundaries.
    The signature is in its own section to force the header to be within the 
    first 8 KiB of the kernel file.
*/

.section .multiboot
.align 4
.long MAGIC
.long FLAGS
.long CHECKSUM

/* 
    The multiboot standard does not define the value of the stack pointer
    register (esp). It is up to the kernel to provide a stack. This will
    allocate room for a small stack by creating a symbol at the bottom of it,
    then allocating 16384 bytes for it, then finally creating a symbol on top.
    The stack grows downward on x86. The stack is in its own section so that it
    can be marked "nobits", which makes the kernel file smaller, as it does not
    contain the uninitialized stack.
    The stack on x86 must be 16 byte-aligned according to System V ABI standard
    and de-facto extensions. The compiler will assume that, and undefined
    behavior would result if it's not properly aligned.
*/

.section .bss
.align 4
stack_bottom:
.skip 16384 # 16 KiB
stack_top:

/*
    Linker script specifies _start as the entry point for the kernel.
    The bootloader will jump to this position once the kernel has loaded.
    The bootloader is gone, so it won't make sense to return from the function.
*/

.section .text
.global _start
.type _start, @function
_start:
    /*
        The bootloader put us into 32 bit unprotected mode.
        Interrupts and paging are disabled.
        Processor state is as defined in the multiboot standard.
        Kernel has full control of the CPU. 
        Kernel can only use hardware features and the code it provides. 

        There is no printf, unless we make one.
        There is no security.
        No safeguards. 
        No debugging.
        Final destination.
    */
    
    # set ESP to point to the top of the stack (grows down on x86)
    mov $stack_top, %esp 

    /*  
        This is a good place to initialize crucial processor space before
        we enter the high-level kernel.
        It's best to minimize this early environment without crucial features
        like floating point and instruction set extensions.

        Load GDT (Global Descriptor Table) here.
        Enable paging here.

        C++ features like global constructs and exception will require runtime
        support. 
    */

    /*
        Enter high-level kernel. 
        ABI requires the stack to be 16 byte aligned at the time of the call
        instruction, which will then push a return pointer of 4 bytes.
        It was 16 byte alignede, and we pushed 0 bytes since, so everything
        is cool.
    */
    call kernel_main

    cli
1:  hlt
    jmp 1b

/*
    Set the size of the _start symbol to the current location minus its start.
    Useful for debugging and eventually call tracing.
*/
.size _start, . - _start

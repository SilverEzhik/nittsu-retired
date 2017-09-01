/* bootloader will look at this image and start executing at the symbol designated as entry point */
ENTRY(_start)

/* tell where various sections of object files will be in the final kernel image */
SECTIONS {
    /* begin putting sections at 1 MiB, the usual place for kernels to be loaded at by the bootloader */
    . = 1M;

    /* first goes the multiboot header, which needs to be very early in the image
     * then goes the .text section */
    .text BLOCK(4K) : ALIGN(4K) {
        *(.multiboot)
        *(.text)
    }

    /* read-only data */
    .rodata BLOCK(4K) : ALIGN(4K) {
        *(.rodata)
    }

    /* read-write data (initialized) */
    .data BLOCK(4K) : ALIGN(4K) {
        *(.data)
    }

    /* read-write data (uninitialized) and stack */
    .bss BLOCK(4K) : ALIGN(4K) {
        *(COMMON)
        *(.bss)
    }

    /* compiler may produce other sections, which will by defauly be put in the identically named segments
     * add stuff here if needed */
}

#ifndef ARCH_I386_VGA_H
#define ARCH_I386_VGA_H

#include <stdint.h>

// hardware text mode color constants
enum vga_color {
    VGA_COLOR_BLACK = 0,
    VGA_COLOR_BLUE = 1,
    VGA_COLOR_GREEN = 2,
    VGA_COLOR_CYAN = 3,
    VGA_COLOR_RED = 4,
    VGA_COLOR_MAGENTA = 5,
    VGA_COLOR_BROWN = 6, 
    VGA_COLOR_LIGHT_GREY = 7,
    VGA_COLOR_DARK_GREY = 8,
    VGA_COLOR_LIGHT_BLUE = 9,
    VGA_COLOR_LIGHT_GREEN = 10,
    VGA_COLOR_LIGHT_CYAN = 11,
    VGA_COLOR_LIGHT_RED = 12,
    VGA_COLOR_LIGHT_MAGENTA = 13,
    VGA_COLOR_LIGHT_BROWN = 14,
    VGA_COLOR_WHITE = 15,
};

// uintXX_t - precisely an unsigned int that is XX bits.
// static - function name is not externally linked
// inline - compiler will try to just copy the function code and call it when it's called
//          makes sense to do something like that when it's a simple one-liner
// 
// so static inline will make the compiler not bother with making a function that you can call

// crafts the color byte of an vga entry from the defined colors above
static inline uint8_t vga_entry_color(enum vga_color fg, enum vga_color bg) {
    return fg | bg << 4; // byte divided into 2 nibbles (4 bits) for fg and bg
                         // 7654|3210
                         // 7654 - bg; 7 - bg bright bit
                         // 3210 - fg; 3 - fg bright bit
                         // because of this we shift the bg by 4 bits and OR it
}

// crafts a complete vga byte given a character and a color
static inline uint16_t vga_entry(unsigned char uc, uint8_t color) {
    return (uint16_t) uc | (uint16_t) color << 8; // each character takes up 2 bytes
                                                  // first - color (hence byte shift)
                                                  // second - char itself (ASCII 8 bit)
}

#endif

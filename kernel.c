#include <stdbool.h> // no booleans in c by default
#include <stddef.h>
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

// size_t is a data type used for sizes, according to C99

// length of string... duh
size_t strlen(const char* str) {
    size_t len = 0;
    // just count until null terminator lol
    while (str[len] != '\0') { 
        len++;
    }
    return len;
}

// terminal dimensions
static const size_t VGA_WIDTH  = 80;
static const size_t VGA_HEIGHT = 25;

// global (gasp) variables storing the current cursor position and color used
size_t terminal_row;
size_t terminal_column;
uint8_t terminal_color;

uint16_t* terminal_buffer; //pointer to the VGA memory address (0xB800)

// generates the offset for the VGA memory buffer
// well we use this one a lot, don't we?
static inline size_t vga_index(size_t x, size_t y) {
    return y * VGA_WIDTH + x;
}

// initialize the variables used to display stuff, and clean the screen
void terminal_initialize(void) {
    terminal_row = 0;
    terminal_column = 0;
    terminal_color = vga_entry_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK); 
    terminal_buffer = (uint16_t*) 0xB8000; //cast memory value to pointer to an uint16_t

    // clear screen
    for (size_t y = 0; y < VGA_HEIGHT; y++) {
        for (size_t x = 0; x < VGA_WIDTH; x++) {
            terminal_buffer[vga_index(x, y)] = vga_entry(' ', terminal_color);
        }
    }
}

// set the color manually, i guess
void terminal_setcolor(uint8_t color) {
    terminal_color = color;
}

// put a character with color at position
void terminal_putentryat(char c, uint8_t color, size_t x, size_t y) {
    terminal_buffer[vga_index(x,y)] = vga_entry(c, color); // idk why i like this so much 
}

// put a character where you'd expect it to be 
void terminal_putchar(char c) {
    // handle newlines
    if (c == '\n') {
        terminal_column = 0;
        // TODO: don't just copypaste code from below
        if (++terminal_row == VGA_HEIGHT) {
            terminal_row = 0;
        }
        return;
    }
    terminal_putentryat(c, terminal_color, terminal_column, terminal_row);

    // prevent overflowing
    if (++terminal_column == VGA_WIDTH) { 
        terminal_column = 0;
        // TODO: make this move everything up instead flowing up like that
        if (++terminal_row == VGA_HEIGHT) {
            terminal_row = 0;
        }
    }
}

// write char array to screen
void terminal_write(const char* data, size_t size) {
    for (size_t i = 0; i < size; i++) {
        terminal_putchar(data[i]);
    }
}

// write string to screen (well, same as above, but does strlen for you)
void terminal_writestring(const char* data) {
    terminal_write(data, strlen(data));
}

#if defined (__cplusplus)
extern "C" // use C linkage for kernel_main
#endif

void kernel_main(void) {
    // initialize terminal
    terminal_initialize();
    terminal_writestring("Hello 1x World.\nEzhik here. Hope we have fun!\n");
}

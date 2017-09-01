#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

#include <kernel/tty.h>

#include "vga.h"

// terminal constants
static const size_t VGA_WIDTH  = 80;
static const size_t VGA_HEIGHT = 25;
static uint16_t* const VGA_MEMORY = (uint16_t*) 0xB8000;

// global (gasp) variables storing the current cursor position and color used
static size_t terminal_row;
static size_t terminal_column;
static uint8_t terminal_color;
 
static uint16_t* terminal_buffer; //pointer to the VGA memory address (0xB800)

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
    terminal_buffer = VGA_MEMORY; //cast memory value to pointer to an uint16_t

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

static inline void terminal_increase_row() {
        if (++terminal_row == VGA_HEIGHT) {
            terminal_row = VGA_HEIGHT - 1;
            // shift everything up by one
            for (unsigned int i = VGA_WIDTH; i < VGA_WIDTH * VGA_HEIGHT; i++) {
                terminal_buffer[i - VGA_WIDTH] = terminal_buffer[i];
                // blank out the last line when switching up
                if (i >= VGA_WIDTH * (VGA_HEIGHT - 1)) {
                    terminal_buffer[i] = vga_entry(' ', vga_entry_color(VGA_COLOR_BLACK, VGA_COLOR_BLACK));
                }
            }
        }
}
// put a character where you'd expect it to be 
void terminal_putchar(char c) {
    // handle newlines
    if (c == '\n') {
        terminal_column = 0;
        terminal_increase_row();
        return;
    }
    terminal_putentryat(c, terminal_color, terminal_column, terminal_row);

    // prevent overflowing
    if (++terminal_column == VGA_WIDTH) { 
        terminal_column = 0;
        terminal_increase_row();
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

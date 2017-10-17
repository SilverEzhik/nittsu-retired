#include "gdt.h"

// Lets us access our ASM functions from our C code.
// extern means that the function will be found elsewhere
extern void gdt_flush(uint32_t);

// Internal function prototypes.
void init_gdt();
static void gdt_set_gate(int32_t, uint32_t, uint32_t, uint8_t, uint8_t);

gdt_entry_t gdt_entries[5];
gdt_ptr_t   gdt_ptr;

void init_gdt() {
    gdt_ptr.limit = (sizeof(gdt_entry_t) * 5) - 1;
    gdt_ptr.base  = (uint32_t)&gdt_entries;
 
    //not using segmentation proper, so just one segment for everything from 0 to max
    gdt_set_gate(0, 0, 0, 0, 0);                // Null segment
    gdt_set_gate(1, 0, 0xFFFFFFFF, 0x9A, 0xCF); // Code segment
    gdt_set_gate(2, 0, 0xFFFFFFFF, 0x92, 0xCF); // Data segment
    gdt_set_gate(3, 0, 0xFFFFFFFF, 0xFA, 0xCF); // User mode code segment
    gdt_set_gate(4, 0, 0xFFFFFFFF, 0xF2, 0xCF); // User mode data segment
 
    //5 segments made for the gdt entry, now to LGDT:
    gdt_flush((uint32_t)&gdt_ptr);
}


// Set the value of one GDT entry.
static void gdt_set_gate(int32_t num, uint32_t base, uint32_t limit, uint8_t access, uint8_t gran) {
    // 1. write the base (starting address)
    
    // broken apart due to compatibility reasons
    gdt_entries[num].base_low    = (base & 0xFFFF);     //lower 16 bits
    gdt_entries[num].base_middle = (base >> 16) & 0xFF; //middle 8 bits
    gdt_entries[num].base_high   = (base >> 24) & 0xFF; //high 8 bits
 
    // 2. write the limit
    
    // first the lower 16 bits
    gdt_entries[num].limit_low   = (limit & 0xFFFF); //1 hex char - 4 bits
    // next the last 4 bits (20 bit long)
    gdt_entries[num].granularity = (limit >> 16) & 0x0F; 
    // bit shift and only keep the last 4 bits
 
    // 3. set flags
    // first, granularity ( |= - "OR assign")
    // that is, a |= b is the same as a = a | b
    gdt_entries[num].granularity |= (gran & 0xF0); // only the higher 4 bits
    // then, the access byte
    gdt_entries[num].access = access; //set the access byte
}

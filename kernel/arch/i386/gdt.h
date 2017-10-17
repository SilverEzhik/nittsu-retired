#include <stdint.h>

// Initialisation function is publicly accessible.
void init_gdt();

// This structure contains the value of one GDT entry.
// We use the attribute 'packed' to tell GCC not to change
// any of the alignment in the structure.


// each entry is 8 bytes long
// limit is 20 bits - depending on granularity bit either up to 1 MB or 2^32 bytes
// base is 32 bits - starting address of segment, broken up due to compatibility reasons
struct gdt_entry_struct {
   uint16_t limit_low;           // The lower 16 bits of the limit.
   uint16_t base_low;            // The lower 16 bits of the base.
   uint8_t  base_middle;         // The next 8 bits of the base.
   uint8_t  access;              // Access flags, determine what ring this segment can be used in.
   uint8_t  granularity;         // Granularity + last 4 bits of the limit
   uint8_t  base_high;           // The last 8 bits of the base.
}
__attribute__((packed));
typedef struct gdt_entry_struct gdt_entry_t;

// 1 char - 1 bit
//
//     Limit (15..0)       Base (15..0)
// |---- ---- ---- ----|---- ---- ---- ----|
//
//  B(23..16)  Access     Gran.  B.(31..24)
// |---- ----|PDDd TTTT|GD0A|Lim.|---- ----|
//                         (19..16) 
          
// access byte structure:
//
// 7 - P - Is segment present? (1 - yes)
// 6 - DPL - Descriptor Privilege Level (ring 0/3)
// 5 - DPL
// 4 - DT - Descriptor type
// 3 - Type - Segment type (code segment/data segment)
// 2 - Type
// 1 - Type
// 0 - Type

// granularity byte structure 
// 7 - G - granularity (0 - 1 byte, limit up to 1MB, 1 - pages (4KB), limit up to 2^32 bytes)
// 6 - D - operand size (0 - 16 bits, 1 - 32 bits)
// 5 - 0 - should always be zero
// 4 - A - available for system use, always zero
// 3 - limit (segment length) - bits 19:16
// 2 - limit 
// 1 - limit
// 0 - limit

// 

struct gdt_ptr_struct {
   uint16_t limit; // "size"   - The upper 16 bits of all selector limits.
   uint32_t base;  // "offset" - The address of the first gdt_entry_t struct.
}
 __attribute__((packed));
typedef struct gdt_ptr_struct gdt_ptr_t;



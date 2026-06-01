#include "stdint.h"

extern void gdt_flush(void);

typedef struct {
    uint16_t limit_low;
    uint16_t base_low;
    uint8_t base_mid;
    uint8_t access;
    uint8_t flags;
    uint8_t base_high;
} __attribute__((packed)) gdt_entry_t;

typedef struct {
    uint16_t limit;
    uint64_t base;
} __attribute__((packed)) gdt_pointer_t;

static gdt_entry_t gdt[6];
static gdt_pointer_t gdt_ptr;

void gdt_init(void) {
    gdt[0] = (gdt_entry_t){0, 0, 0, 0, 0, 0};
    
    uint32_t base = 0;
    uint32_t limit = 0xFFFFF;
    
    gdt[1] = (gdt_entry_t){limit & 0xFFFF, base & 0xFFFF, (base >> 16) & 0xFF, 0x9A, 0xA0, (base >> 24) & 0xFF};
    gdt[2] = (gdt_entry_t){limit & 0xFFFF, base & 0xFFFF, (base >> 16) & 0xFF, 0x92, 0xC0, (base >> 24) & 0xFF};
    
    gdt_ptr.limit = sizeof(gdt) - 1;
    gdt_ptr.base = (uint64_t)&gdt;
    
    gdt_flush();
}

#include "stdint.h"
#include "vga.h"

extern void gdt_init(void);
extern void idt_init(void);

void kernel_main(void) {
    vga_init();
    vga_write_string(0, 0, "PolyOS x86_64 Kernel", 0x0F);
    vga_write_string(0, 1, "======================", 0x0F);
    
    gdt_init();
    idt_init();
    
    __asm__ volatile ("sti");
    
    vga_write_string(0, 3, "GDT/IDT loaded!", 0x0A);
    vga_write_string(0, 5, "Hello World!", 0x0B);
    vga_write_string(0, 7, "System ready.", 0x0C);
    
    for (;;) __asm__ volatile ("hlt");
}

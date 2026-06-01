#include <kernel.h>
#include <vga.h>

extern void gdt_flush();
extern void idt_flush();

void kernel_main() {
    volatile char *vga = (volatile char *)0xB8000;
    const char *msg = "PolyOS booted!";
    for (int i = 0; msg[i]; i++) {
        vga[i*2] = msg[i];
        vga[i*2+1] = 0x07;
    }
    
    for (;;) {
        __asm__ volatile ("hlt");
    }
}

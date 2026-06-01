/* =============================================================================
 * src/kernel/main.c - Kernel Main Entry Point (Freestanding)
 * ============================================================================= */

#include "stdint.h"
#include "gdt.h"
#include "idt.h"
#include "pic.h"
#include "vga.h"

/* =============================================================================
 * kernel_main - Wird vom Bootloader aufgerufen
 * ============================================================================= */
void kernel_main(void) {
    /* VGA initialisieren */
    vga_init();
    vga_clear();
    
    /* Willkommensnachricht */
    vga_write_string(0, 0, "========================================", 0x0F);
    vga_write_string(0, 1, "  PolyOS - x86_64 Bare Metal Kernel", 0x0F);
    vga_write_string(0, 2, "========================================", 0x0F);
    
    /* Segment-Tabellen initialisieren */
    vga_write_string(0, 4, "[INIT] Loading GDT...", 0x0A);
    gdt_init();
    
    vga_write_string(0, 5, "[INIT] Loading IDT...", 0x0A);
    idt_init();
    
    vga_write_string(0, 6, "[INIT] Configuring PIC...", 0x0A);
    pic_init();
    
    /* Interrupts aktivieren */
    __asm__ volatile ("sti");
    vga_write_string(0, 7, "[INIT] Interrupts enabled.", 0x0A);
    
    /* Hello World */
    vga_write_string(0, 9, "Hello World from protected mode!", 0x0B);
    
    /* Boot-Info */
    vga_write_string(0, 11, "GDT/IDT/PIC loaded. System ready.", 0x0C);
    vga_write_string(0, 12, "Exceptions will be caught!", 0x0C);
    
    /* Infinite Loop */
    for (;;) {
        __asm__ volatile ("hlt");
    }
}

/* =============================================================================
 * src/kernel/pic.c - Programmable Interrupt Controller Implementation
 * ============================================================================= */

#include "pic.h"
#include "idt.h"

/* =============================================================================
 * Sendet einen End-of-Interrupt Befehl an den PIC
 * ============================================================================= */
void pic_send_eoi(uint8_t irq) {
    if (irq >= 8) {
        /* Slave PIC */
        __asm__ volatile ("movb $0xA0, %%al; outb %%al, $0xA0" : : : "rax");
    }
    /* Master PIC - immer senden */
    __asm__ volatile ("movb $0x20, %%al; outb %%al, $0x20" : : : "rax");
}

/* =============================================================================
 * Initialisiert den PIC und remapped die Interrupts
 * 
 * Standard: IRQ 0-7 -> INT 8-15 (kollidiert mit CPU Exceptions!)
 * Nach Remap: IRQ 0-7 -> INT 32-39, IRQ 8-15 -> INT 40-47
 * ============================================================================= */
void pic_init(void) {
    /* ICW1: Start initialization */
    __asm__ volatile ("movb $0x11, %%al; outb %%al, $0x20" : : : "rax");  /* Master */
    __asm__ volatile ("movb $0x11, %%al; outb %%al, $0xA0" : : : "rax");  /* Slave */

    /* ICW2: Interrupt Vectors */
    __asm__ volatile ("movb $0x20, %%al; outb %%al, $0x21" : : : "rax");  /* Master: IRQ 0 -> INT 0x20 */
    __asm__ volatile ("movb $0x28, %%al; outb %%al, $0xA1" : : : "rax");  /* Slave: IRQ 8 -> INT 0x28 */

    /* ICW3: Cascade setup */
    __asm__ volatile ("movb $0x04, %%al; outb %%al, $0x21" : : : "rax");  /* Master: IRQ 2 hat Slave */
    __asm__ volatile ("movb $0x02, %%al; outb %%al, $0xA1" : : : "rax");  /* Slave: Slave an IRQ 2 */

    /* ICW4: 8086 mode */
    __asm__ volatile ("movb $0x01, %%al; outb %%al, $0x21" : : : "rax");  /* Master */
    __asm__ volatile ("movb $0x01, %%al; outb %%al, $0xA1" : : : "rax");  /* Slave */

    /* OCW1: Alle Interrupts maskieren (deaktivieren) */
    __asm__ volatile ("movb $0xFF, %%al; outb %%al, $0x21" : : : "rax");  /* Master */
    __asm__ volatile ("movb $0xFF, %%al; outb %%al, $0xA1" : : : "rax");  /* Slave */
}
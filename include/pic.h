/* =============================================================================
 * include/pic.h - Programmable Interrupt Controller (PIC) Header
 * =============================================================================
 * Der PIC verwaltet Hardware-Interrupts (IRQ 0-15).
 * Im Long Mode muss der PIC für APIC deaktiviert werden.
 * ============================================================================= */

#ifndef PIC_H
#define PIC_H

#include "stdint.h"

/* PIC I/O Ports */
#define PIC1_COMMAND    0x20    /* Master PIC Command Port */
#define PIC1_DATA       0x21    /* Master PIC Data Port */
#define PIC2_COMMAND    0xA0    /* Slave PIC Command Port */
#define PIC2_DATA       0xA1    /* Slave PIC Data Port */

/* PIC Commands */
#define PIC_CMD_INIT     0x11   /* Initialize */
#define PIC_CMD_EOI      0x20   /* End of Interrupt */

/* PIC ICW (Initialization Command Words) */
#define PIC_ICW1_INIT    0x10   /* Start initialization */
#define PIC_ICW1_ICW4    0x01   /* ICW4 needed */
#define PIC_ICW4_8086    0x01   /* 8086/88 Mode */

/* Remapped IRQ bases (nach Init) */
#define IRQ_BASE_MASTER  0x20   /* IRQ 0-7 -> INT 0x20-0x27 */
#define IRQ_BASE_SLAVE   0x28   /* IRQ 8-15 -> INT 0x28-0x2F */

/* PIC Funktionen */
void pic_init(void);
void pic_send_eoi(uint8_t irq);

#endif /* PIC_H */
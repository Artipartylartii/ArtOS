/* =============================================================================
 * include/kernel.h - PolyOS Kernel Header
 * =============================================================================
 * Enthält Kernel-Initialisierung und Hauptfunktionen.
 * ============================================================================= */

#ifndef KERNEL_H
#define KERNEL_H

#include "stdint.h"

/* Kernel Initialisierung */
void kernel_init(void);

/* Segment-Deskriptor Tabellen */
void gdt_init(void);
void idt_init(void);
void pic_init(void);

/* Kernel Entry Point */
void kmain(void);

#endif /* KERNEL_H */
/* =============================================================================
 * src/kernel/idt.c - Interrupt Descriptor Table Implementation
 * =============================================================================
 * Füllt die IDT mit Gate-Deskriptoren und lädt sie.
 * Verwendet die ISR-Handler aus isr_stub.S.
 * ============================================================================= */

#include "idt.h"
#include "gdt.h"

/* ISR Handler Adresse (aus isr_stub.S) */
extern uint64_t isr_table[32];

/* Exception Namen */
const char* exception_names[IDT_INTERRUPTS] = {
    "Division Error",
    "Debug Exception",
    "NMI",
    "Breakpoint",
    "Overflow",
    "Bound Range",
    "Invalid Opcode",
    "Device Not Available",
    "Double Fault",
    "Coprocessor Overrun",
    "Invalid TSS",
    "Segment Not Present",
    "Stack Fault",
    "General Protection",
    "Page Fault",
    "Reserved (15)",
    "x87 FPU Error",
    "Alignment Check",
    "Machine Check",
    "SIMD FP Error",
    "Virtualization",
    "Reserved (20)",
    "Reserved (21)",
    "Reserved (22)",
    "Reserved (23)",
    "Reserved (24)",
    "Reserved (25)",
    "Reserved (26)",
    "Reserved (27)",
    "Reserved (28)",
    "Reserved (29)",
    "Reserved (30)",
    "Reserved (31)"
};

/* IDT Tabelle */
static idt_entry_t idt_entries[IDT_MAX_ENTRIES];

/* IDTR Register */
static idt_register_t idt_register;

/* =============================================================================
 * Setzt einen IDT Gate-Eintrag
 * ============================================================================= */
void idt_set_gate(int index, uint64_t offset, uint16_t selector, uint8_t type_attr) {
    if (index >= IDT_MAX_ENTRIES) return;

    idt_entry_t *entry = &idt_entries[index];

    /* Offset in 16-bit Teile zerlegen */
    entry->offset_low  = (uint16_t)(offset & 0xFFFF);
    entry->offset_mid  = (uint16_t)((offset >> 16) & 0xFFFF);
    entry->offset_high = (uint32_t)((offset >> 32) & 0xFFFFFFFF);

    entry->selector   = selector;
    entry->ist        = 0;
    entry->type_attr  = type_attr;
    entry->reserved   = 0;
}

/* =============================================================================
 * Initialisiert die IDT mit Exception-Handlern
 * ============================================================================= */
void idt_init(void) {
    /* Kernel Code Segment Selector */
    uint16_t kernel_cs = GDT_ENTRY_KERNEL_CODE * 8;

    /* Alle 32 CPU Exceptions eintragen */
    for (int i = 0; i < 32; i++) {
        idt_set_gate(i, isr_table[i], kernel_cs, IDT_TYPE_INTERRUPT_GATE);
    }

    /* Restliche Gates auf 0 (nicht verwendet) */
    for (int i = 32; i < IDT_MAX_ENTRIES; i++) {
        idt_entries[i].offset_low = 0;
        idt_entries[i].selector = 0;
        idt_entries[i].ist = 0;
        idt_entries[i].type_attr = 0;
        idt_entries[i].offset_mid = 0;
        idt_entries[i].offset_high = 0;
        idt_entries[i].reserved = 0;
    }

    /* IDTR laden */
    idt_register.limit = (uint16_t)(sizeof(idt_entry_t) * IDT_MAX_ENTRIES - 1);
    idt_register.base = (uint64_t)&idt_entries[0];

    /* IDT in CPU laden */
    idt_load();
}

/* =============================================================================
 * Lädt die IDT in das CPU Register
 * ============================================================================= */
void idt_load(void) {
    __asm__ volatile ("lidt %0" : : "m" (idt_register));
}
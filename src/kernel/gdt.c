/* =============================================================================
 * src/kernel/gdt.c - Global Descriptor Table Implementation
 * ============================================================================= */

#include "gdt.h"

/* GDT Tabelle - 16 entries x 8 bytes = 128 bytes */
static gdt_entry_t gdt_entries[GDT_MAX_ENTRIES];

/* GDTR Register */
static gdt_register_t gdt_register;

/* Aktuelle Anzahl der Einträge */
static int gdt_entry_count = 0;

/* =============================================================================
 * Setzt einen GDT Eintrag mit den angegebenen Parametern
 * ============================================================================= */
void gdt_set_entry(int index, uint32_t base, uint32_t limit, uint8_t access, uint8_t flags) {
    if (index >= GDT_MAX_ENTRIES) return;

    gdt_entry_t *entry = &gdt_entries[index];

    /* Base Address (32-bit) */
    entry->base_low  = (uint16_t)(base & 0xFFFF);
    entry->base_mid  = (uint8_t)((base >> 16) & 0xFF);
    entry->base_high = (uint8_t)((base >> 24) & 0xFF);

    /* Limit (20-bit) */
    entry->limit_low = (uint16_t)(limit & 0xFFFF);
    entry->flags = (uint8_t)((flags & 0xF0) | ((limit >> 16) & 0x0F));

    /* Access Byte */
    entry->access = access;
}

/* =============================================================================
 * Initialisiert die GDT mit den Standard-Deskriptoren
 * ============================================================================= */
void gdt_init(void) {
    /* Null Deskriptor (immer erforderlich) */
    gdt_set_entry(GDT_ENTRY_KERNEL_NULL, 0, 0, 0, 0);

    /* Kernel Code Segment - 64-bit Long Mode */
    gdt_set_entry(GDT_ENTRY_KERNEL_CODE,
                  0,                      /* Base */
                  0xFFFF,                  /* Limit */
                  0x9A,                    /* Access: Present, DPL0, Code, R */
                  GDT_FLAGS_LONG_MODE | 0x0C); /* Flags: Granularity, Long Mode */

    /* Kernel Data Segment */
    gdt_set_entry(GDT_ENTRY_KERNEL_DATA,
                  0,
                  0xFFFF,
                  0x92,                    /* Access: Present, DPL0, Data, W */
                  0x0C);                   /* Flags: Granularity */

    /* User Space Null Deskriptor */
    gdt_set_entry(GDT_ENTRY_USER_NULL, 0, 0, 0, 0);

    /* User Code Segment */
    gdt_set_entry(GDT_ENTRY_USER_CODE,
                  0,
                  0xFFFF,
                  0xFA,                    /* Access: Present, DPL3, Code, R */
                  GDT_FLAGS_LONG_MODE | 0x0C);

    /* User Data Segment */
    gdt_set_entry(GDT_ENTRY_USER_DATA,
                  0,
                  0xFFFF,
                  0xF2,                    /* Access: Present, DPL3, Data, W */
                  0x0C);

    gdt_entry_count = GDT_ENTRY_USER_DATA + 1;

    /* GDTR Register laden */
    gdt_register.limit = (uint16_t)(sizeof(gdt_entry_t) * gdt_entry_count - 1);
    gdt_register.base  = (uint64_t)&gdt_entries[0];

    /* GDT laden */
    gdt_load();
}

/* =============================================================================
 * Lädt die GDT in das CPU Register
 * ============================================================================= */
void gdt_load(void) {
    /* Inline Assembly für lgdt */
    __asm__ volatile ("lgdt %0" : : "m" (gdt_register));

    /* Reload Segments - CS muss via far jump geladen werden */
    __asm__ volatile (
        "mov %0, %%ax\n\t"
        "mov %%ax, %%ds\n\t"
        "mov %%ax, %%es\n\t"
        "mov %%ax, %%fs\n\t"
        "mov %%ax, %%gs\n\t"
        "mov %%ax, %%ss"
        : : "r" ((uint16_t)(GDT_ENTRY_KERNEL_DATA * 8))
    );
}
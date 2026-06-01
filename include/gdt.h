/* =============================================================================
 * include/gdt.h - Global Descriptor Table (GDT) für x86_64
 * =============================================================================
 * Die GDT definiert Segment-Deskriptoren für den Protected Mode.
 * In x86_64 Long Mode sind die meisten GDT-Einträge obsolet,
 * aber sie wird für特权级 ( CPL / Ring ) und TSS benötigt.
 * ============================================================================= */

#ifndef GDT_H
#define GDT_H

#include "stdint.h"

/* GDT Limits */
#define GDT_MAX_ENTRIES    16
#define GDT_ENTRY_SIZE    8       /* 8 bytes per entry in Long Mode */

/* GDT Selector Flags (Access Byte) */
#define GDT_ACCESS_RW       0x02   /* Read/Write */
#define GDT_ACCESS_EXPAND   0x04   /* Expand-Down */
#define GDT_ACCESS_CODE     0x0A   /* Executable/Code */
#define GDT_ACCESS_NORMAL   0x92   /* Normal Data/Stack Access */
#define GDT_ACCESS_PRESENT  0x80   /* Present */
#define GDT_ACCESS_DPL0     0x00   /* Ring 0 */
#define GDT_ACCESS_DPL3     0x60   /* Ring 3 */

/* GDT Selector Flags (Flags Nibbles) */
#define GDT_FLAGS_LONG_MODE  0x20   /* Long Mode (64-bit) */
#define GDT_FLAGS_PROTECTED 0x40   /* Protected Mode (32-bit) */
#define GDT_FLAGS_GRANULAR  0x80   /* Granularity 4KB */

/* Standard GDT Indizes */
#define GDT_ENTRY_KERNEL_NULL  0   /* Null Deskriptor */
#define GDT_ENTRY_KERNEL_CODE  1   /* Kernel Code Segment */
#define GDT_ENTRY_KERNEL_DATA  2   /* Kernel Data Segment */
#define GDT_ENTRY_USER_NULL    3   /* User Space Null */
#define GDT_ENTRY_USER_CODE    4   /* User Code Segment */
#define GDT_ENTRY_USER_DATA    5   /* User Data Segment */

/* GDT Deskriptor Struktur (64-bit) */
typedef struct __attribute__((packed)) {
    uint16_t limit_low;      /* Limit Bits 0-15 */
    uint16_t base_low;       /* Base Bits 0-15 */
    uint8_t  base_mid;       /* Base Bits 16-23 */
    uint8_t  access;         /* Access Byte */
    uint8_t  flags;          /* Flags + Limit Bits 16-19 */
    uint8_t  base_high;      /* Base Bits 24-31 */
} gdt_entry_t;

/* GDTR Register Struktur */
typedef struct __attribute__((packed)) {
    uint16_t limit;          /* GDT Größe - 1 */
    uint64_t base;           /* Basisadresse der GDT */
} gdt_register_t;

/* GDT Funktionen */
void gdt_init(void);
void gdt_load(void);
void gdt_set_entry(int index, uint32_t base, uint32_t limit, uint8_t access, uint8_t flags);

#endif /* GDT_H */

/* =============================================================================
 * include/idt.h - Interrupt Descriptor Table (IDT) für x86_64
 * =============================================================================
 * Die IDT fängt Hardware- und Software-Interrupts ab.
 * Im Long Mode werden nur 64-bit IDT-Einträge verwendet.
 * ============================================================================= */

#ifndef IDT_H
#define IDT_H

#include "stdint.h"

/* IDT Limits */
#define IDT_MAX_ENTRIES    256
#define IDT_INTERRUPTS      32    /* CPU Exceptions 0-31 */

/* IDT Entry Typen */
#define IDT_TYPE_INTERRUPT_GATE   0x8E   /* Present, DPL0, 64-bit Interrupt Gate */
#define IDT_TYPE_TRAP_GATE        0x8F   /* Present, DPL0, 64-bit Trap Gate */

/* Exception Numbers */
#define EXCEPTION_DIVIDE_ERROR     0x00
#define EXCEPTION_DEBUG            0x01
#define EXCEPTION_NMI              0x02
#define EXCEPTION_BREAKPOINT       0x03
#define EXCEPTION_OVERFLOW         0x04
#define EXCEPTION_BOUND_RANGE      0x05
#define EXCEPTION_INVALID_OPCODE   0x06
#define EXCEPTION_DEVICE_NOT_AVAIL 0x07
#define EXCEPTION_DOUBLE_FAULT     0x08
#define EXCEPTION_COPROC_SEG      0x09
#define EXCEPTION_INVALID_TSS      0x0A
#define EXCEPTION_SEG_NOT_PRESENT  0x0B
#define EXCEPTION_STACK_FAULT      0x0C
#define EXCEPTION_GENERAL_PROTECT  0x0D
#define EXCEPTION_PAGE_FAULT       0x0E
#define EXCEPTION_RESERVED         0x0F
#define EXCEPTION_FLOAT_ERROR      0x10
#define EXCEPTION_ALIGNMENT_CHECK  0x11
#define EXCEPTION_MACHINE_CHECK    0x12
#define EXCEPTION_FLOAT_SIMD       0x13
#define EXCEPTION_VIRTUALIZATION   0x14
#define EXCEPTION_RESERVED_15      0x15
#define EXCEPTION_RESERVED_18      0x1B
#define EXCEPTION_RESERVED_20      0x1C
#define EXCEPTION_RESERVED_21      0x1D
#define EXCEPTION_RESERVED_22      0x1E
#define EXCEPTION_RESERVED_23      0x1F

/* Exception Namen (für Debug-Ausgabe) */
extern const char* exception_names[IDT_INTERRUPTS];

/* IDT Gate Struktur (64-bit) */
typedef struct __attribute__((packed)) {
    uint16_t offset_low;     /* Offset Bits 0-15 */
    uint16_t selector;       /* Code Segment Selector */
    uint8_t  ist;           /* IST Pointer (0 = legacy) */
    uint8_t  type_attr;     /* Type + Attribute */
    uint16_t offset_mid;    /* Offset Bits 16-31 */
    uint32_t offset_high;   /* Offset Bits 32-63 */
    uint32_t reserved;      /* Must be zero */
} idt_entry_t;

/* IDTR Register Struktur */
typedef struct __attribute__((packed)) {
    uint16_t limit;          /* IDT Größe - 1 */
    uint64_t base;           /* Basisadresse der IDT */
} idt_register_t;

/* Interrupt Frame (auf dem Stack bei Exception) */
typedef struct {
    uint64_t rdi, rsi, rdx, rcx, r8, r9, r10, r11;
    uint64_t rax, rbx, rbp, r12, r13, r14, r15;
    uint64_t error_code;
    uint64_t rip, cs, rflags, rsp, ss;
} interrupt_frame_t;

/* Exception Handler Funktionstyp */
typedef void (*exception_handler_t)(interrupt_frame_t* frame, uint64_t error_code);

/* IDT Funktionen */
void idt_init(void);
void idt_load(void);
void idt_set_gate(int index, uint64_t offset, uint16_t selector, uint8_t type_attr);

#endif /* IDT_H */
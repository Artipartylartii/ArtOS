/* =============================================================================
 * src/kernel/isr.c - Interrupt Service Routine Handler Implementation
 * ============================================================================= */

#include "idt.h"
#include "stdint.h"

/* VGA Debug Output */
static void vga_puts_at(int row, int col, const char* str, uint8_t color) {
    volatile char* vga = (volatile char*)0xB8000;
    int idx = (row * 80 + col) * 2;
    
    while (*str) {
        vga[idx++] = *str++;
        vga[idx++] = color;
    }
}

static void vga_hex(int col, int row, uint64_t value) {
    volatile char* vga = (volatile char*)0xB8000;
    int idx = (row * 80 + col) * 2;
    
    for (int i = 60; i >= 0; i -= 4) {
        uint8_t nibble = (value >> i) & 0xF;
        vga[idx++] = nibble < 10 ? ('0' + nibble) : ('A' + nibble - 10);
        vga[idx++] = 0x0F;
    }
}

static void vga_clear_screen(void) {
    volatile char* vga = (volatile char*)0xB8000;
    for (int i = 0; i < 80 * 25; i++) {
        vga[i * 2] = ' ';
        vga[i * 2 + 1] = 0x07;
    }
}

/* =============================================================================
 * Exception Handler
 * ============================================================================= */
void exception_handler(interrupt_frame_t* frame, uint64_t error_code, int exception_num) {
    vga_clear_screen();
    
    /* Header */
    vga_puts_at(0, 0, "========================================", 0x0C);
    vga_puts_at(1, 0, "  PolyOS Exception Handler", 0x0C);
    vga_puts_at(2, 0, "========================================", 0x0C);
    
    /* Exception Nummer */
    vga_puts_at(4, 0, "Exception #", 0x0A);
    
    volatile char* vga = (volatile char*)0xB8000;
    if (exception_num >= 10) {
        vga[9 * 2] = '1';
        vga[10 * 2] = '0' + (exception_num % 10);
    } else {
        vga[10 * 2] = '0' + exception_num;
    }
    
    /* Exception Name */
    if (exception_num < 32 && exception_names[exception_num]) {
        vga_puts_at(4, 14, (char*)exception_names[exception_num], 0x0E);
    }
    
    /* Error Code */
    vga_puts_at(6, 0, "Error Code:", 0x0B);
    vga_hex(12, 6, error_code);
    
    /* Register Ausgabe */
    vga_puts_at(8, 0, "RIP:", 0x09);
    vga_hex(4, 8, frame->rip);
    
    vga_puts_at(10, 0, "RSP:", 0x09);
    vga_hex(4, 10, frame->rsp);
    
    vga_puts_at(12, 0, "RFLAGS:", 0x09);
    vga_hex(8, 12, frame->rflags);
    
    vga_puts_at(14, 0, "CS:", 0x09);
    vga_hex(3, 14, frame->cs);
    
    /* CPU anhalten */
    __asm__ volatile ("cli");
    for (;;) {
        __asm__ volatile ("hlt");
    }
}

/* Handler ohne Error Code */
#define DEFINE_HANDLER(n) \
    void exception_handler_##n(interrupt_frame_t* frame, uint64_t error_code) { \
        (void)error_code; \
        exception_handler(frame, 0, n); \
    }

/* Handler mit Error Code */
#define DEFINE_ERR_HANDLER(n) \
    void exception_handler_##n(interrupt_frame_t* frame, uint64_t error_code) { \
        exception_handler(frame, error_code, n); \
    }

/* Kein Error Code */
DEFINE_HANDLER(0)
DEFINE_HANDLER(1)
DEFINE_HANDLER(2)
DEFINE_HANDLER(3)
DEFINE_HANDLER(4)
DEFINE_HANDLER(5)
DEFINE_HANDLER(6)
DEFINE_HANDLER(7)
DEFINE_HANDLER(9)
DEFINE_HANDLER(15)
DEFINE_HANDLER(16)
DEFINE_HANDLER(18)
DEFINE_HANDLER(19)
DEFINE_HANDLER(20)
DEFINE_HANDLER(21)
DEFINE_HANDLER(22)
DEFINE_HANDLER(23)
DEFINE_HANDLER(24)
DEFINE_HANDLER(25)
DEFINE_HANDLER(26)
DEFINE_HANDLER(27)
DEFINE_HANDLER(28)
DEFINE_HANDLER(29)
DEFINE_HANDLER(30)
DEFINE_HANDLER(31)

/* Mit Error Code */
DEFINE_ERR_HANDLER(8)
DEFINE_ERR_HANDLER(10)
DEFINE_ERR_HANDLER(11)
DEFINE_ERR_HANDLER(12)
DEFINE_ERR_HANDLER(13)
DEFINE_ERR_HANDLER(14)
DEFINE_ERR_HANDLER(17)

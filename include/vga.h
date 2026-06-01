/* =============================================================================
 * include/vga.h - VGA Textmodus Treiber (Freestanding)
 * =============================================================================
 * Direkter Hardware-Zugriff auf VGA Textmodus (0xB8000)
 * ============================================================================= */

#ifndef VGA_H
#define VGA_H

#include "stdint.h"

/* VGA I/O Ports */
#define VGA_CTRL_PORT  0x3D4   /* VGA Control Register */
#define VGA_DATA_PORT  0x3D5   /* VGA Data Register */

/* VGA Register Indices */
#define VGA_CURSOR_HIGH  0x0E
#define VGA_CURSOR_LOW   0x0F

/* VGA Farben */
enum vga_color {
    VGA_COLOR_BLACK         = 0,
    VGA_COLOR_BLUE         = 1,
    VGA_COLOR_GREEN        = 2,
    VGA_COLOR_CYAN         = 3,
    VGA_COLOR_RED          = 4,
    VGA_COLOR_MAGENTA      = 5,
    VGA_COLOR_BROWN         = 6,
    VGA_COLOR_LIGHT_GREY   = 7,
    VGA_COLOR_DARK_GREY     = 8,
    VGA_COLOR_LIGHT_BLUE    = 9,
    VGA_COLOR_LIGHT_GREEN   = 10,
    VGA_COLOR_LIGHT_CYAN    = 11,
    VGA_COLOR_LIGHT_RED     = 12,
    VGA_COLOR_LIGHT_MAGENTA = 13,
    VGA_COLOR_YELLOW        = 14,
    VGA_COLOR_WHITE         = 15
};

/* VGA Konfiguration */
#define VGA_WIDTH   80
#define VGA_HEIGHT  25
#define VGA_SIZE    (VGA_WIDTH * VGA_HEIGHT)

/* VGA Text Buffer (Physical Address) */
#define VGA_BUFFER  ((volatile uint16_t*)0xB8000)

/* Hilfsfunktionen */
static inline uint8_t vga_entry_color(enum vga_color fg, enum vga_color bg) {
    return fg | (bg << 4);
}

static inline uint16_t vga_entry(unsigned char c, uint8_t color) {
    return (uint16_t)c | ((uint16_t)color << 8);
}

/* Funktionen */
void vga_init(void);
void vga_clear(void);
void vga_put_char(int x, int y, char c, uint8_t color);
void vga_write_string(int x, int y, const char* str, uint8_t color);

#endif /* VGA_H */

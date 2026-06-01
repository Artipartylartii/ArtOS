/* =============================================================================
 * src/kernel/vga.c - VGA Textmodus Treiber (Freestanding)
 * =============================================================================
 * Direkter Hardware-Zugriff - Keine Standardbibliothek!
 * ============================================================================= */

#include "stdint.h"
#include "vga.h"

/* VGA Cursor Position speichern */
static size_t cursor_x = 0;
static size_t cursor_y = 0;

/* =============================================================================
 * IO Port Operationen (Inline Assembly)
 * ============================================================================= */

static inline unsigned char inb(unsigned short port) {
    unsigned char ret;
    __asm__ volatile ("inb %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}

static inline void outb(unsigned short port, unsigned char val) {
    __asm__ volatile ("outb %0, %1" : : "a"(val), "Nd"(port));
}

/* =============================================================================
 * Cursor Management
 * ============================================================================= */

static void update_cursor(void) {
    /* Cursor Position berechnen */
    uint16_t pos = cursor_y * VGA_WIDTH + cursor_x;
    
    /* High Byte senden */
    outb(VGA_CTRL_PORT, VGA_CURSOR_HIGH);
    outb(VGA_DATA_PORT, (unsigned char)((pos >> 8) & 0xFF));
    
    /* Low Byte senden */
    outb(VGA_CTRL_PORT, VGA_CURSOR_LOW);
    outb(VGA_DATA_PORT, (unsigned char)(pos & 0xFF));
}

static void scroll(void) {
    /* Zeilen verschieben wenn am unteren Rand */
    if (cursor_y >= VGA_HEIGHT) {
        /* Alles eine Zeile nach oben kopieren */
        for (size_t i = 0; i < (VGA_HEIGHT - 1) * VGA_WIDTH; i++) {
            VGA_BUFFER[i] = VGA_BUFFER[i + VGA_WIDTH];
        }
        
        /* Letzte Zeile leeren */
        for (size_t i = (VGA_HEIGHT - 1) * VGA_WIDTH; i < VGA_SIZE; i++) {
            VGA_BUFFER[i] = vga_entry(' ', VGA_COLOR_BLACK);
        }
        
        cursor_y = VGA_HEIGHT - 1;
    }
}

/* =============================================================================
 * Core VGA Funktionen
 * ============================================================================= */

void vga_init(void) {
    vga_clear();
    cursor_x = 0;
    cursor_y = 0;
}

void vga_clear(void) {
    /* Bildschirm leeren */
    for (size_t i = 0; i < VGA_SIZE; i++) {
        VGA_BUFFER[i] = vga_entry(' ', VGA_COLOR_BLACK);
    }
    cursor_x = 0;
    cursor_y = 0;
}

void vga_put_char(int x, int y, char c, uint8_t color) {
    /* Bounds prüfen */
    if (x < 0 || x >= VGA_WIDTH || y < 0 || y >= VGA_HEIGHT) {
        return;
    }
    
    /* Zeichen schreiben */
    size_t index = y * VGA_WIDTH + x;
    VGA_BUFFER[index] = vga_entry(c, color);
}

void vga_write_string(int x, int y, const char* str, uint8_t color) {
    if (!str) return;
    
    int i = 0;
    while (str[i] != '\0') {
        vga_put_char(x + i, y, str[i], color);
        i++;
    }
}

/* =============================================================================
 * Debug-Funktionen (für early Boot)
 * ============================================================================= */

void vga_write_at(const char* str) {
    /* Schreibt String an current cursor position */
    if (!str) return;
    
    scroll();
    
    while (*str) {
        if (cursor_x >= VGA_WIDTH) {
            cursor_x = 0;
            cursor_y++;
            scroll();
        }
        
        if (*str == '\n') {
            cursor_x = 0;
            cursor_y++;
            str++;
            scroll();
            continue;
        }
        
        size_t index = cursor_y * VGA_WIDTH + cursor_x;
        VGA_BUFFER[index] = vga_entry(*str, VGA_COLOR_LIGHT_GREEN);
        
        cursor_x++;
        str++;
    }
    
    update_cursor();
}

void vga_write_hex(uint32_t value) {
    /* Hex-Wert ausgeben für Debugging */
    const char hex_chars[] = "0123456789ABCDEF";
    char buffer[11];
    buffer[0] = '0';
    buffer[1] = 'x';
    buffer[10] = '\0';
    
    for (int i = 9; i >= 2; i--) {
        buffer[i] = hex_chars[value & 0xF];
        value >>= 4;
        if (value == 0) break;
    }
    
    vga_write_at(buffer);
}

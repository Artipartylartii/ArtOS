/* =============================================================================
 * linker.ld - Linker Script for PolyOS Kernel (ELF64/x86_64)
 * =============================================================================
 * Creates correct sections for:
 * - Multiboot2 Header (must be in first 8KB, 8-Byte aligned)
 * - Code Section (.text - 64-bit Code)
 * - Read-Only Data (.rodata)  
 * - Data Section (.data)
 * - BSS Section (.bss) - for uninitialized data
 * ============================================================================= */

OUTPUT_FORMAT("elf64-x86-64")

ENTRY(_start)

SECTIONS
{
    /* Kernel Base Address (1MB) */
    kernel_vbase = 0x100000;
    
    /* Multiboot2 Header Section - must be at the start */
    . = kernel_vbase;
    
    .multiboot : {
        *(.multiboot)
        *(.multiboot.*)
    }
    
    /* Text Section - 64-bit Code */
    .text : {
        _start = .;
        *(.text .text.*)
        *(.rodata .rodata.*)
        *(.rodata.cst16)
        *(.rodata.str1.*)
        *(.eh_frame)
        . = ALIGN(16);
    }
    
    /* Read-Only Data */
    .rodata : {
        *(.rodata .rodata.*)
    }
    
    /* Initialized Data */
    .data : {
        *(.data .data.*)
    }
    
    /* BSS - uninitialized data (must be in memory) */
    .bss : {
        _bss_start = .;
        *(.COMMON)
        *(.bss .bss.*)
        . = ALIGN(16);
        _bss_end = .;
    }
    
    /* Discard debug sections */
    /DISCARD/ : {
        *(.debug*)
        *(.comment)
        *(.note.GNU-stack)
        *(.eh_frame_hdr)
    }
    
    /* End Address */
    _end = .;
    
    /* Multiboot Header should be in first 8KB (informational only) */
    /* ASSERT(_end - kernel_vbase <= 0x2000, "Kernel too large (>8KB)") */
}

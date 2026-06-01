OUTPUT_FORMAT("elf32-i386")
ENTRY(_start)

SECTIONS
{
    . = 0x100000;
    
    .multiboot : AT(0) {
        KEEP(*(.multiboot))
    }
    
    .text : {
        *(.text .text.*)
        . = ALIGN(16);
    }
    
    .rodata : {
        *(.rodata .rodata.*)
    }
    
    .data : {
        *(.data .data.*)
    }
    
    .bss : {
        *(.COMMON)
        *(.bss .bss.*)
        . = ALIGN(16);
    }
}

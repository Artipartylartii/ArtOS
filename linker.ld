OUTPUT_FORMAT("elf64-x86-64")
ENTRY(_start)

SECTIONS
{
    . = 0x100000;
    
    .multiboot : AT(0) {
        *(.multiboot)
        *(.multiboot.*)
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
    
    /DISCARD/ : {
        *(.debug*)
        *(.comment)
    }
}

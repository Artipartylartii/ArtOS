.section .text
.global gdt_flush
gdt_flush:
    lgdt gdt_ptr
    movw $0x10, %ax
    movw %ax, %ds
    movw %ax, %es
    movw %ax, %fs
    movw %ax, %gs
    movw %ax, %ss
    ret

.section .data
.global gdt_ptr
gdt_ptr:
    .space 16

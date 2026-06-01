section .multiboot
align 4

multiboot_start:
dd 0x1BADB002           ; magic
dd 0x00010003           ; flags (align, mem info)
dd -((0x1BADB002) + (0x00010003))  ; checksum
dd 0                    ; header_addr
dd 0                    ; load_addr
dd 0                    ; load_end_addr
dd 0                    ; bss_end_addr
dd _start               ; entry_addr

section .text
global _start
_start:
    mov esp, stack_top
    cli
    extern kernel_main
    call kernel_main
.hang:
    hlt
    jmp .hang

section .bss
align 16
stack_bottom:
    resb 16384
stack_top:

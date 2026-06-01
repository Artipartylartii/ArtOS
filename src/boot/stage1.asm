section .multiboot
align 8
multiboot_start:
dd 0xE85250D6
dd 0
dd multiboot_end - $$
dd -((multiboot_end - $$) + 0xE85250D6)
dw 0
dw 0
dd 8
multiboot_end:

section .text
global _start
_start:
    mov rsp, stack_top
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

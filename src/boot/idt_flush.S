.section .text
.global idt_flush
idt_flush:
    lidt idt_ptr
    ret

.section .data
.global idt_ptr
idt_ptr:
    .space 16

.text
.globl idt_flush
.type idt_flush, @function
idt_flush:
    mov 0x10(%esp), %eax
    lidt (%eax)
    ret

/* ISR Stubs for PolyOS - GNU Assembler */
.section .text

/* Macro for exception handler without error code */
.macro EXCEPTION_NO_ERR num
    .global isr_handler_\num
isr_handler_\num:
    subq $8, %rsp
    pushq %r15
    pushq %r14
    pushq %r13
    pushq %r12
    pushq %r11
    pushq %r10
    pushq %r9
    pushq %r8
    pushq %rbp
    pushq %rdi
    pushq %rsi
    pushq %rdx
    pushq %rcx
    pushq %rbx
    pushq %rax
    movq $0, %rsi
    movq %rsp, %rdi
    call exception_handler_\num
.endm

/* Macro for exception handler with error code */
.macro EXCEPTION_ERR num
    .global isr_handler_\num
isr_handler_\num:
    subq $8, %rsp
    pushq %r15
    pushq %r14
    pushq %r13
    pushq %r12
    pushq %r11
    pushq %r10
    pushq %r9
    pushq %r8
    pushq %rbp
    pushq %rdi
    pushq %rsi
    pushq %rdx
    pushq %rcx
    pushq %rbx
    pushq %rax
    movq 128(%rsp), %rsi
    movq %rsp, %rdi
    call exception_handler_\num
.endm

/* Exception handlers 0-31 */
EXCEPTION_NO_ERR 0
EXCEPTION_NO_ERR 1
EXCEPTION_NO_ERR 2
EXCEPTION_NO_ERR 3
EXCEPTION_NO_ERR 4
EXCEPTION_NO_ERR 5
EXCEPTION_NO_ERR 6
EXCEPTION_NO_ERR 7
EXCEPTION_ERR 8
EXCEPTION_NO_ERR 9
EXCEPTION_ERR 10
EXCEPTION_ERR 11
EXCEPTION_ERR 12
EXCEPTION_ERR 13
EXCEPTION_ERR 14
EXCEPTION_NO_ERR 15
EXCEPTION_NO_ERR 16
EXCEPTION_ERR 17
EXCEPTION_NO_ERR 18
EXCEPTION_NO_ERR 19
EXCEPTION_NO_ERR 20
EXCEPTION_NO_ERR 21
EXCEPTION_NO_ERR 22
EXCEPTION_NO_ERR 23
EXCEPTION_NO_ERR 24
EXCEPTION_NO_ERR 25
EXCEPTION_NO_ERR 26
EXCEPTION_NO_ERR 27
EXCEPTION_NO_ERR 28
EXCEPTION_NO_ERR 29
EXCEPTION_NO_ERR 30
EXCEPTION_NO_ERR 31

/* ISR table for IDT initialization */
.section .rodata
.global isr_table
.align 8
isr_table:
    .quad isr_handler_0
    .quad isr_handler_1
    .quad isr_handler_2
    .quad isr_handler_3
    .quad isr_handler_4
    .quad isr_handler_5
    .quad isr_handler_6
    .quad isr_handler_7
    .quad isr_handler_8
    .quad isr_handler_9
    .quad isr_handler_10
    .quad isr_handler_11
    .quad isr_handler_12
    .quad isr_handler_13
    .quad isr_handler_14
    .quad isr_handler_15
    .quad isr_handler_16
    .quad isr_handler_17
    .quad isr_handler_18
    .quad isr_handler_19
    .quad isr_handler_20
    .quad isr_handler_21
    .quad isr_handler_22
    .quad isr_handler_23
    .quad isr_handler_24
    .quad isr_handler_25
    .quad isr_handler_26
    .quad isr_handler_27
    .quad isr_handler_28
    .quad isr_handler_29
    .quad isr_handler_30
    .quad isr_handler_31

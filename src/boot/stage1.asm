; =============================================================================
; boot/stage1.asm - Multiboot2 Stage 1 Bootloader (x86_64)
; =============================================================================
; Anforderungen: 64-bit Long Mode, Multiboot2 Header, Stack-Setup
; =============================================================================

section .multiboot
align 8

; Multiboot2 Header Tags
multiboot2_start:
dd 0xE85250D6              ; Magic
dd 0                        ; Arch: 0 = i386/x86_64
dd multiboot2_end - $$      ; Header Länge
dd -(0xE85250D6 + 0 + (multiboot2_end - $$))  ; Checksum

; Obligatorische Tags
; End-Tag (Markiert Ende der Tags)
dw 0                        ; Type = 0 (End)
dw 0                        ; Flags
dd 8                        ; Size (minimale Größe)
multiboot2_end:


section .text
global _start
_start:
    ; Multiboot2 Header wird vom Loader gelesen
    ; Hier beginnt der Kernel nach dem Bootstrap
    
    ; Stack für C-Code - 16-Byte alignment (ABI Requirement)
    mov rsp, stack_top
    
    ; Interrupts deaktivieren (Bare Metal)
    cli
    
    ; Debug: "K1" in VGA schreiben (x86_64 = 64-bit)
    mov rdx, 0xB8000           ; VGA Textmodus Buffer
    mov word [rdx], 0x4B31     ; 'K' (hellgrün) + '1' (magenta)
    
    ; Schreibe "Hello from stage1!" Pattern zur Validierung
    ; Wird später vom VGA-Treiber überschrieben
    mov rax, 0x0B0A            ; Attribute + newline
    mov qword [rdx + 2], rax
    
    ; C-Kernel aufrufen (Freestanding)
    extern kernel_main
    call kernel_main
    
    ; Kernel returned -> HLT Loop
.hang:
    hlt
    jmp .hang
    
.fini:
    ; Wird nie erreicht
    jmp .fini


section .bss
align 16
stack_bottom:
    resb 16384              ; 16KB Stack
stack_top:


section .rodata
bootloader_info:
    db "PolyOS Stage1 Bootloader - x86_64", 0

section .multiboot
align 4
dd 0x1BADB002          ; Magic number
dd 0x0                  ; Flags
dd -(0x1BADB002 + 0x0)  ; Checksum

section .bss
align 16
stack_bottom:
    resb 16384          ; 16KB stack
stack_top:

section .text
global _start
_start:
    mov esp, stack_top  ; Set stack pointer
    extern kernel_main
    call kernel_main    ; Jump to C code (main function)

.loop:
    hlt
    jmp .loop            ; Infinite halt loop

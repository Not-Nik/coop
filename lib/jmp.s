; coop (c) Nikolas Wipper 2021

section .text

global _coop_setjmp_asm
global _coop_longjmp_asm

_coop_setjmp_asm:
%if __?BITS?__ == 64
    ; buf will be in rax
    ; rbx rsp rbp and r12-r15 are callee saved
    ; the other registers have to be saved by
    ; the caller anyways so we can just ignore them

    ; Save the registers
    ; TODO: We don't save the flags register here
    ;       We could get it using pushf and then
    ;       restore with popf, but that's quite slow
    ;       https://stackoverflow.com/q/1406783
    mov qword [rax +  0], rbx
    mov qword [rax +  8], rcx
    mov qword [rax + 16], rsi
    mov qword [rax + 24], rdi
    mov qword [rax + 32], rbp
    mov qword [rax + 40], r8
    mov qword [rax + 48], r9
    mov qword [rax + 56], r10
    mov qword [rax + 64], r11
    mov qword [rax + 72], r12
    mov qword [rax + 80], r13
    mov qword [rax + 88], r14
    mov qword [rax + 96], r15

    ; Save the address rsp + 24
    ; 24 because we have three 8 byte values to store
    ; later on. 24 is safe because we have at least
    ; 1) The return address to the original caller
    ; 2) The stack base pushed by the compiler
    ; 3) The return address to the wrapper function
    lea rdx, qword [rsp + 24]
    mov qword [rax + 104], rdx

    ; The next three blocks are basically just
    ; saving the important part of the stack since
    ; it is likely to be overwritten by variables
    ; after we return

    ; Save the return address to the wrapper function
    mov rdx, qword [rsp]
    mov qword [rax + 112], rdx

    ; Save the stack base of the original caller
    mov rdx, qword [rbp]
    mov qword [rax + 120], rdx

    ; Save the return address to the original caller
    mov rdx, qword [rbp + 8]
    mov qword [rax + 128], rdx

    ; setjmp needs to return 0, if it doesn't return from a
    ; longjmp. xor rdx, rdx is just a fancy way of writing
    ; mov rdx, 0 which is a bit faster
    xor rdx, rdx
%else
%error Only x86_64/amd64 is supported at the moment
%endif
    ret

_coop_longjmp_asm:
%if __?BITS?__ == 64
    ; buf will be in rax
    ; ret will be in rdx

    ; Restore rsp
    mov rsp, qword [rax + 104]

    ; Restore the return address to the original caller
    push qword [rax + 128]
    ; Restore the stack base of the original caller
    push qword [rax + 120]
    ; Restore the return address to the wrapper function
    push qword [rax + 112]

    ; Restore the registers
    mov rbx, qword [rax +  0]
    mov rcx, qword [rax +  8]
    mov rsi, qword [rax + 16]
    mov rdi, qword [rax + 24]
    mov rbp, qword [rax + 32]
    mov r8 , qword [rax + 40]
    mov r9 , qword [rax + 48]
    mov r10, qword [rax + 56]
    mov r11, qword [rax + 64]
    mov r12, qword [rax + 72]
    mov r13, qword [rax + 80]
    mov r14, qword [rax + 88]
    mov r15, qword [rax + 96]
%else
%error Only x86_64/amd64 is supported at the moment
%endif
    ret

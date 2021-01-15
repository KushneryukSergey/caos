.intel_syntax noprefix
.text
.globl dot_product

dot_product:
    mov rax, 4
    pxor xmm0, xmm0  # clear xmm0 = ans
    cmp rax, rdi
    jg end_loop
loop:
    sub rax, 4
    movups xmm1, XMMWORD PTR[rsi+rax*4]
    movups xmm2, XMMWORD PTR[rdx+rax*4]
    dpps xmm1, xmm2, 0xF1
    addps xmm0, xmm1

    add rax, 8
    cmp rax, rdi
    jle loop

end_loop:
    sub rax, 4
    cmp rax, rdi
    jge end_mini_loop

mini_loop:
    movss xmm1, DWORD PTR[rsi+rax*4]
    mulss xmm1, DWORD PTR[rdx+rax*4]
    addss xmm0, xmm1
    inc rax
    cmp rax, rdi
    jge end_mini_loop
    jmp mini_loop

end_mini_loop:
    ret
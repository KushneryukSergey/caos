.intel_syntax noprefix
.text
.global calc, vector_sum

calc:
    endbr64
    addsd xmm0, xmm1
    pxor xmm3, xmm3 // reset by pxor, because cvtsi2sd badly designed and
    cvtsi2sd xmm3, edi // has dependency on previous value of dest
    addsd xmm2, xmm3
    divsd xmm0, xmm2
    ret

vector_sum:
    xor rax, rax // reset to zero
loop:
    movaps xmm0, XMMWORD PTR [rsi+4*rax]
    addps xmm0, XMMWORD PTR [rdx+4*rax]
    movaps XMMWORD PTR [rcx+4*rax], xmm0
    add rax, 4
    cmp rax, rdi
    je end
    jmp loop
end:
    ret
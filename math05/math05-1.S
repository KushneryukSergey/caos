.intel_syntax noprefix
.text
.global my_sin

my_sin:
    mov rax, 2
    movsd xmm1, xmm0 // xmm0 = res; xmm1 = x;
    movsd xmm2, xmm0 // series element a_i
loop:
    movsd xmm3, xmm0 // xmm3 = last;
    mulsd xmm2, xmm1 //
    mulsd xmm2, xmm1 // a_i * x * x

    pxor xmm4, xmm4 // reset by pxor, because cvtsi2sd badly designed and
    cvtsi2sd xmm4, rax // has dependency on previous value of dest
    divsd xmm2, xmm4
    inc rax

    pxor xmm4, xmm4
    cvtsi2sd xmm4, rax
    divsd xmm2, xmm4 // (2n - 1)! -> (2n + 1)!
    inc rax

    mov rdi, rax
    shr rdi, 1
    and rdi, 1
    cmp rdi, 1 // + or - a_i
    jne minus
    addsd xmm0, xmm2
    jmp end_upd
minus:
    subsd xmm0, xmm2
end_upd:
    ucomisd xmm0, xmm3
    jne loop

end:
    ret
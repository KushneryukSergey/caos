.intel_syntax noprefix
.text
.global	summ

summ:
    endbr64
    mov rax, 0
    push rbx

// for(i = 0; i < N; ++i)
loop:
    cmp rax, rdi 
    jge end

    mov ebx, DWORD PTR [rsi+rax*4]
    add ebx, DWORD PTR [rdx+rax*4]
    mov DWORD PTR[rcx+rax*4], ebx

    add eax, 1
    jmp loop

end:
    pop rbx
    ret

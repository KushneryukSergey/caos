.intel_syntax noprefix
.text
.global	summ
.global everyday795

summ:
    endbr64
    mov eax, 0
    mov rcx, QWORD PTR A
    mov rdx, QWORD PTR B
    mov rsi, QWORD PTR R

// for(i = 0; i < N; ++i)
loop:
    cmp eax, N
    jge end

    mov edi, DWORD PTR [rcx+rax*4]
    add edi, DWORD PTR [rdx+rax*4]
    mov DWORD PTR[rsi+rax*4], edi

    add eax, 1
    jmp loop

end:
    ret


everyday795:
    //scanf
    push rbp
    push rsi
    push rdi
    sub rsp, 16

    mov rdi, offset .format
    mov rsi, rsp
    add rsi, 8
    call scanf

    //calculating
    mov rsi, QWORD PTR [rsp+8]
    add rsp, 16
    pop rdi
    imul rsi, rdi
    pop rdi
    add rsi, rdi
   

    //printf
    mov rdi, offset .format
    call printf

    pop rbp
    mov rsp, rbp
    ret



.format:
    .string "%d"

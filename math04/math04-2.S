.intel_syntax noprefix
.text
.global	merge
.global mergesort
.global mergesort_helper


//-----------
//   MERGE
//-----------

// rdi = from
// rsi = mid
// rdx = to
// rcx = out
merge:
    push rbx
    mov r8, rdi
    mov r9, rsi

// r8 = it1
// r9 = it2
loop:
    cmp r8, rsi
    jge push_back2 ; # if (it1 >= mid) push all element from right part
    cmp r9, rdx
    jge push_back1 ; # if (it2 >= to) push all element from left part
    mov eax, DWORD PTR [rcx+4*r8]
    mov ebx, DWORD PTR [rcx+4*r9]
    cmp eax, ebx
    jge inc_it2 ; # if (*it1 > *it2) ++it2 else ++it1
    inc_it1:
        add r8, 1
        jmp end_inc
    inc_it2:
        add r9, 1
end_inc:
    cmp eax, ebx
    cmovg eax, ebx
    push rax
    jmp loop

push_back1: ; # push all elements from left part in stack
    cmp r8, rsi
    jge prep_load ; # if (it1 >= mid) return elements from stack in buffer
    mov eax, DWORD PTR [rcx+4*r8]
    push rax
    add r8, 1
    jmp push_back1

push_back2: ; # push all elements from right part in stack
    cmp r9, rdx
    jge prep_load ; # if (it2 >= to) return elements from stack in buffer
    mov eax, DWORD PTR [rcx+4*r9]
    push rax
    add r9, 1
    jmp push_back2

prep_load:
    mov r10, rdx

load: ; # load element from stack to buffer
    sub r10, 1
    cmp r10, rdi
    jl end_merge
    pop rax
    mov DWORD PTR [rcx+4*r10], eax
    jmp load

end_merge:
    pop rbx
    ret




//--------------------
//  MERGESORT_HELPER
//--------------------

// rdi = from
// rsi = to
// rdx = out
mergesort_helper:
    // if (to - from <= 1) return
    mov rax, rsi
    sub rax, rdi
    cmp rax, 1
    jle end_mg_hlpr

    // rax = mid = (from + to) / 2
    mov rax, rdi
    add rax, rsi
    shr rax, 1

    push rdx

    // mergesort_helper(from, mid, out)
    push rdi
    push rsi
    push rdx
    push rax
    mov rsi, rax
    call mergesort_helper
    pop rax
    pop rdx
    pop rsi
    pop rdi

    // mergesort_helper(mid, to, out)
    push rdi
    push rsi
    push rdx
    push rax
    mov rdi, rax
    call mergesort_helper
    mov rdx, rsi

    // prepare for merge
    // before: (from, to, out) + mid
    // regs:   (rdi, rsi, rdx) + rax
    //
    // after:  (from, mid, to, out)
    pop rsi // rax = mid -> rsi
    pop rcx // rdx = out -> rcx
    pop rdx // rsi = to -> rdx
    pop rdi // rdi = from -> rdi

    // merge(from, mid, to, out)
    pop rcx
    push rbp
    call merge
    pop rbp
    jmp end_mg_hlpr

end_mg_hlpr:
    ret


//-------------
//  MERGESORT
//-------------

// rdi = from
// rsi = to
// rdx = in
// rcx = out
mergesort:
    push rcx
    mov rax, rdi
    mov r8, 4
    imulq rax, 4
    sub rcx, rax
    mov rax, rdi

mgsrt_loop:
    cmp rax, rsi
    jge mgsrt_end
    mov r8d, DWORD PTR [rdx+4*rax]
    mov DWORD PTR [rcx+4*rax], r8d
    add rax, 1
    jmp mgsrt_loop

mgsrt_end:
    pop rdx
    push rbp
    sub rsi, rdi
    mov rdi, 0
    call mergesort_helper
    pop rbp
    ret
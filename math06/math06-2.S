#include <sys/syscall.h>

.intel_syntax noprefix
.text
.global my_exit, my_read, my_write, _start
.global my_read_str, my_write_str, my_malloc
.global syscall

syscall:
// copypasted syscall
    // prologue
    push rbx
    push rbp
    push r12
    push r13
    push r14
    push r15
    push r11
    push rcx
    // push arguments into memory to reaarange them
    push r9
    push r8
    push rcx
    push rdx
    push rsi
    push rdi
    // first argument is syscall number, move it into rax
    pop rax
    // move arguments into registers to match syscall calling conventions
    pop rdi
    pop rsi
    pop rdx
    pop r10
    pop r8
    // make actual syscall
    syscall
    // epilogue
    pop rcx
    pop r11
    pop r15
    pop r14
    pop r13
    pop r12
    pop rbp
    pop rbx
    ret

my_exit:
    push rbp
    mov rsi, rdi
    mov rdi, SYS_exit
    call syscall
    pop rbp
    ret

// rdi, rsi, rdx, rcx
my_read:
    push rbp
    mov rcx, rdx
    mov rdx, rsi
    mov rsi, rdi
    mov rdi, SYS_read
    call syscall
    pop rbp
    ret

my_write:
    push rbx
    mov rcx, rdx
    mov rdx, rsi
    mov rsi, rdi
    mov rdi, SYS_write
    call syscall
    pop rbp
    ret

// r10 curr_read
// r11 left_to_read
// r12 to_read saved
// r13 buf saved
my_read_str:
    mov r11, rdx
    push rbp
    push r12
    push r13
    mov r12, rdx
    mov r13, rsi
my_read_str_loop:
    cmp r11, 0  // left_to_read != 0
    je my_read_str_end
    mov rsi, r13
    add rsi, r12
    sub rsi, r11

    push rdi
    push rdx
    call my_read
    pop rdx
    pop rdi

    mov r10, rax
    cmp r10, 0  // curr_read != 0
    je my_read_str_end

    sub r11, r10
    jmp my_read_str_loop

my_read_str_end:
    mov rax, rdx
    sub rax, r11
    pop r13
    pop r12
    pop rbp
    ret

// r10 curr_write
// r11 left_to_write
// r12 to_write saved
// r13 buf saved
my_write_str:
    mov r11, rdx
    push rbp
    push r12
    push r13
    mov r12, rdx
    mov r13, rsi
my_write_str_loop:
    cmp r11, 0  // left_to_write != 0
    je my_write_str_end
    mov rsi, r13
    add rsi, r12
    sub rsi, r11
    call my_write
    mov r10, rax
    cmp r10, 0  // curr_write != 0
    je my_write_str_end

    sub r11, r10
    jmp my_write_str_loop

my_write_str_end:
    mov rax, rdx
    sub rax, r11
    pop r13
    pop r12
    pop rbp
    ret

my_malloc:
    push rdi  // size
    mov rsi, 0
    mov rdi, SYS_brk
    call syscall  // void* buf_start = (void*)syscall(SYS_brk, 0);

    pop rsi
    push rax
    add rsi, rax
    mov rdi, SYS_brk
    call syscall  //  syscall(SYS_brk, (char*)buf_start + size);

    pop rax
    ret

// r8 = buf_size
// r9 = buf
// r10 = curr_read
_start:
    mov r8, 4096
    mov rdi, r8

    push rbp
    push r8
    push r10
    call my_malloc
    pop r10
    pop r8
    pop rbp

    mov r9, rax
loop:
// curr_read = my_read_str(STDIN, buf, buf_size) {
    mov rdi, 0  // STDIN
    mov rsi, r9
    mov rdx, r8

    push r8
    push r9
    push r10
    call my_read_str
    pop r10
    pop r9
    pop r8

    mov r10, rax
    cmp r10, 0
    je end_loop

// my_write_str(STDOUT, buf, curr_read);
    mov rdi, 1  //  STDOUT
    mov rsi, r9
    mov rdx, r10

    push r8
    push r9
    push r10
    call my_write_str
    pop r10
    pop r9
    pop r8

    jmp loop

end_loop:
end__start:
    mov rdi, 0
    call my_exit
    pop rbp
    ret

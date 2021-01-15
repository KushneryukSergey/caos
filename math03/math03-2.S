.section .text
.global main 

main:
    push {lr}

input:
    ldr r0, =stdin
    ldr r0, [r0]
    bl fgetc

check_eof:
    cmp r0, #-1
    beq end

check_digit:
    mov r2, r0
    ldr r1, =.start
    ldr r1, [r1]
    sub r0, r1
    ldr r1, =.len
    ldr r1, [r1]
    cmp r0, r1
    bge input

output:
    mov r0, r2
    ldr r1, =stdout
    ldr r1, [r1]
    bl fputc
    b input

end:
    mov r0, #0
    pop {pc}


.section .data    
.start:
    .word 48

.len:
    .word 10

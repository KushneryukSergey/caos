.section .text
.global calculate

calculate:
    ldr r0, =A
    ldr r0, [r0]
    ldr r1, =B
    ldr r1, [r1]
    mul r0, r0, r1
    ldr r1, =C
    ldr r1, [r1]
    ldr r2, =D
    ldr r2, [r2]
    mul r1, r1, r2
    add r0, r0, r1
    ldr r1, =R
    str r0, [r1]
    bx lr

.section .data
.global R
R:
    .word 

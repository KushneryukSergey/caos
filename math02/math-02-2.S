.text
.global solve

solve:
    push {r4, r5, lr}
    mov r4, #0   

while:
    mul r5, r0, r4 // Ax
    add r5, r5, r1 // Ax + B
    mul r5, r5, r4 // Ax^2 + Bx
    add r5, r5, r2 // Ax^2 + Bx + C
    mul r5, r5, r4 // Ax^3 + Bx^2 + Cx
    adds r5, r5, r3 // Ax^3 + Bx^2 + Cx + D

    add r4, r4, #1

    bne while
    sub r4, #1
    mov r0, r4
    pop {r4, r5, pc}

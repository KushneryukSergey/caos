.text
.global summ

summ:
    push {lr}
    while:
        subs r1, #1
        bge add_element
    pop {lr}
    bx lr

add_element:
    ldr r3, [r2]
    add r0, r0, r3
    add r2, r2, #4
    b while

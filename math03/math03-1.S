.section .text
.global main 

main:
    push {lr}
    ldr r0, =.input_format
    ldr r1, =.a 
    ldr r2, =.b
    bl scanf
    
    ldr r1, =.a
    ldr r1, [r1]
    ldr r2, =.b
    ldr r2, [r2]
    add r2, r1, r2
    ldr r1, =.a
    str r2, [r1]
    
    ldr r0, =.output_format
    ldr r1, =.a
    ldr r1, [r1]
    mov r1, #3
    bl printf
    pop {pc}

.section .data    
.input_format:
    .ascii "%d %d"
    .ascii "\0"

.output_format:
    .ascii "%d"
    .ascii "\0"

.a:
    .word 0

.b:
    .word 0

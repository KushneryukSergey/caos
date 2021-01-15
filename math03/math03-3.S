.section .text
.global main 
.global update_stack
.global decrease_current


main:
    push {lr}
    bl update_stack

input:
    ldr r0, =stdin
    ldr r0, [r0]
    bl fgetc
    
    ldr r1, =.stack_pointer
    ldr r1, [r1]
    ldr r2, =.stack_current
    ldr r2, [r2]
    add r1, r2
    str r0, [r1]

check_eof:
    cmp r0, #-1
    beq prepare
    b increase_current

prepare:
    //bl decrease_current

output:
    ldr r1, =.stack_current
    ldr r1, [r1]
    cmp r1, #0
    bleq end
    bl decrease_current

    ldr r1, =.stack_current
    ldr r1, [r1]

    ldr r0, =.stack_pointer
    ldr r0, [r0]
    add r0, r1
    ldr r0, [r0]
    //mov r0, #48
    ldr r1, =stdout
    ldr r1, [r1]
    bl fputc

    b output

end:
    ldr r0, =.stack_pointer
    ldr r0, [r0]
    bl free

    pop {lr}
    bx lr

update_stack:
    push {lr}
    ldr r0, =.stack_pointer
    ldr r0, [r0]

    ldr r2, =.stack_size
    ldr r1, [r2]
    add r1, #16
    str r1, [r2]

    bl realloc
    
    ldr r1, =.stack_pointer
    str r0, [r1]
    pop {lr}
    bx lr

increase_current:
    ldr r1, =.stack_current
    ldr r0, [r1]
    add r0, #1
    str r0, [r1]

    b check_size

decrease_current:
    push {lr}

    ldr r1, =.stack_current
    ldr r0, [r1]
    subs r0, #1
    str r0, [r1]

    pop {lr}
    bx lr

check_size:
    ldr r0, =.stack_size
    ldr r0, [r0]
    ldr r1, =.stack_current
    ldr r1, [r1]
    add r1, #1
    cmp r0, r1
    
    bleq update_stack
    b input
    

.section .data
.stack_pointer:
    .word 0x00

.stack_size:
    .int 0 

.stack_current:
    .word 0

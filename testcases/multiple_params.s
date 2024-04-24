    .data
    .extern printf
fmt:
    .string "%ld\n"
    .align 7
v_argc:
    .quad -1
v_it0:
    .quad 0
v_it1:
    .quad 0
v_a:
    .quad 0
    .text
    .global main
main:
    stp x29, x30, [SP, #-16]!
    ldr x1, =v_argc
    str x0, [x1]
// ASSIGN
    b func0_end
func0:
    stp x29, x30, [SP, #-16]!
    ldr x3, =v_it0
    ldr x2, [x3]
    str x2, [SP, #-16]!
    str x0, [x3]
    ldr x3, =v_it1
    ldr x2, [x3]
    str x2, [SP, #-16]!
    str x1, [x3]
    ldr x0, =v_it0
    ldr x0, [x0]
    ldr x1, [SP], #16
    ldr x2, =v_it1
    str x1, [x2]
    ldr x1, [SP], #16
    ldr x2, =v_it0
    str x1, [x2]
    ldp x29, x30, [SP], #16
    ret
    ldr x1, [SP], #16
    ldr x2, =v_it1
    str x1, [x2]
    ldp x29, x30, [SP], #16
    mov x0, #0
    ldr x1, [SP], #16
    ldr x2, =v_it0
    str x1, [x2]
    ldp x29, x30, [SP], #16
    mov x0, #0
    ret
func0_end:
    ldr x0, =func0
    ldr x1, =v_a
    str x0, [x1]
// print
    ldr x0, =v_a
    ldr x0, [x0]
    str x0, [SP, #-16]!
    ldr x0, =2
    ldr x2, [SP], #16
    blr x2
    mov x1, x0
    ldr x0, =fmt
    bl printf
    mov x0, #0
    ldp x29, x30, [SP], #16
    ret

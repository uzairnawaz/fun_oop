    .data
    .extern printf
fmt:
    .string "%ld\n"
    .align 7
v_argc:
    .quad -1
v_it:
    .quad 0
v_Dog:
    .quad 0
v_Object:
    .quad 0
v_a:
    .quad 0
v_b:
    .quad 0
v_Animal:
    .quad 0
v_c:
    .quad 0
    .text
    .global main
main:
    stp x29, x30, [SP, #-16]!
    ldr x1, =v_argc
    str x0, [x1]
// print
    ldr x0, =5
    mov x1, x0
    ldr x0, =fmt
    bl printf
// print
    ldr x0, =4
    mov x1, x0
    ldr x0, =fmt
    bl printf
    b class0_end
class0:
    stp x29, x30, [SP, #-16]!
// ASSIGN
    ldr x0, =4
    str x0, [x10, #0]
    ldp x29, x30, [SP], #16
    ret
class0_end:
    ldr x0, =class0
    ldr x1, =v_Dog
    str x0, [x1]
// ASSIGN
    b func1_end
func1:
    stp x29, x30, [SP, #-16]!
    ldr x2, =v_it
    ldr x1, [x2]
    str x1, [SP, #-16]!
    str x0, [x2]
// print
    ldr x0, =3
    mov x1, x0
    ldr x0, =fmt
    bl printf
    ldr x1, [SP], #16
    ldr x2, =v_it
    str x1, [x2]
    ldp x29, x30, [SP], #16
    mov x0, #0
    ret
func1_end:
    ldr x0, =func1
    ldr x1, =v_b
    str x0, [x1]
// ASSIGN
    ldr x0, =v_b
    ldr x0, [x0]
    str x0, [SP, #-16]!
    ldr x0, =2
    ldr x1, [SP], #16
    blr x1
    ldr x1, =v_c
    str x0, [x1]
    mov x0, #0
    ldp x29, x30, [SP], #16
    ret

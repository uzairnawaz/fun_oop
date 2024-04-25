    .data
    .extern printf
fmt:
    .string "%ld\n"
    .align 7
v_argc:
    .quad -1
v_it:
    .quad 0
v_Building:
    .quad 0
v_Object:
    .quad 0
v_int:
    .quad 0
v_height:
    .quad 0
v_capacity:
    .quad 0
v_x1:
    .quad 0
v_self:
    .quad 0
v_x2:
    .quad 0
v_ConcertHall:
    .quad 0
v_roomCount:
    .quad 0
v_x:
    .quad 0
    .text
    .global main
main:
    stp x29, x30, [SP, #-16]!
    ldr x1, =v_argc
    str x0, [x1]
    b class0_end
class0:
    stp x29, x30, [SP, #-16]!
// ASSIGN
    ldr x0, =4
    str x0, [x10, #0]
// ASSIGN
    ldr x0, =10
    str x0, [x10, #8]
// ASSIGN
    b func1_end
func1:
    stp x29, x30, [SP, #-16]!
    ldr x2, =v_it
    ldr x1, [x2]
    str x1, [SP, #-16]!
    str x0, [x2]
    mov x0, x10
    ldr x0, [x0, #0]
    ldr x1, [SP], #16
    ldr x2, =v_it
    str x1, [x2]
    ldp x29, x30, [SP], #16
    ret
    ldr x1, [SP], #16
    ldr x2, =v_it
    str x1, [x2]
    ldp x29, x30, [SP], #16
    mov x0, #0
    ret
func1_end:
    ldr x0, =func1
    str x0, [x10, #16]
// ASSIGN
    b func2_end
func2:
    stp x29, x30, [SP, #-16]!
    ldr x2, =v_it
    ldr x1, [x2]
    str x1, [SP, #-16]!
    str x0, [x2]
    mov x0, x10
    ldr x0, [x0, #16]
    str x0, [SP, #-16]!
    ldr x0, =0
    ldr x1, [SP], #16
    ldr x2, [SP], #16
    ldr x3, =v_it
    str x2, [x3]
    ldp x29, x30, [SP], #16
    br x1
    ldr x1, [SP], #16
    ldr x2, =v_it
    str x1, [x2]
    ldp x29, x30, [SP], #16
    mov x0, #0
    ret
func2_end:
    ldr x0, =func2
    str x0, [x10, #24]
    ldp x29, x30, [SP], #16
    ret
class0_end:
    ldr x0, =class0
    ldr x1, =v_Building
    str x0, [x1]
    b class3_end
class3:
    stp x29, x30, [SP, #-16]!
    ldr x0, =v_Building
    ldr x0, [x0]
    blr x0
// ASSIGN
    ldr x0, =3
    str x0, [x10, #32]
// ASSIGN
    ldr x0, =100
    str x0, [x10, #8]
    ldp x29, x30, [SP], #16
    ret
class3_end:
    ldr x0, =class3
    ldr x1, =v_ConcertHall
    str x0, [x1]
// ASSIGN
    ldr x0, =32
    bl fun_malloc
    str x0, [SP, #-16]!
    ldr x1, =v_Building
    ldr x1, [x1]
    str x10, [SP, #-16]!
    mov x10, x0
    blr x1
    ldr x10, [SP], #16
    ldr x0, [SP], #16
    ldr x1, =v_x
    str x0, [x1]
// print
    ldr x0, =v_x
    ldr x0, [x0]
    str x10, [SP, #-16]!
    mov x10, x0
    ldr x0, [x0, #24]
    str x0, [SP, #-16]!
    ldr x0, =0
    ldr x1, [SP], #16
    blr x1
    ldr x10, [SP], #16
    mov x1, x0
    ldr x0, =fmt
    bl printf
    mov x0, #0
    ldp x29, x30, [SP], #16
    ret

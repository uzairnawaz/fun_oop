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
v_Dog:
    .quad 0
v_Object:
    .quad 0
v_age:
    .quad 0
v_size:
    .quad 0
v_makeBigger:
    .quad 0
v_self:
    .quad 0
v_bark:
    .quad 0
v_Cat:
    .quad 0
v_enemy:
    .quad 0
v_friend:
    .quad 0
v_makeOlder:
    .quad 0
v_c:
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
    ldr x0, =1
    str x0, [x10, #0]
// ASSIGN
    ldr x0, =4
    str x0, [x10, #8]
// ASSIGN
    b func1_end
func1:
    stp x29, x30, [SP, #-16]!
    ldr x3, =v_it0
    ldr x2, [x3]
    str x2, [SP, #-16]!
    str x0, [x3]
    ldr x3, =v_it1
    ldr x2, [x3]
    str x2, [SP, #-16]!
    str x1, [x3]
// ASSIGN
    mov x0, x10
    str x0, [SP, #-16]!
    mov x0, x10
    ldr x0, [x0, #8]
    str x0, [SP, #-16]!
    ldr x0, =v_it0
    ldr x0, [x0]
    mov x1, x0
    ldr x0, [SP], #16
    add x0, x0, x1
    mov x1, x0
    ldr x0, [SP], #16
    str x1, [x0, #8]
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
func1_end:
    ldr x0, =func1
    str x0, [x10, #16]
// ASSIGN
    b func2_end
func2:
    stp x29, x30, [SP, #-16]!
    ldr x3, =v_it0
    ldr x2, [x3]
    str x2, [SP, #-16]!
    str x0, [x3]
    ldr x3, =v_it1
    ldr x2, [x3]
    str x2, [SP, #-16]!
    str x1, [x3]
// print
    ldr x0, =9999
    mov x1, x0
    ldr x0, =fmt
    bl printf
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
func2_end:
    ldr x0, =func2
    str x0, [x10, #24]
    ldp x29, x30, [SP], #16
    ret
class0_end:
    ldr x0, =class0
    ldr x1, =v_Dog
    str x0, [x1]
    b class3_end
class3:
    stp x29, x30, [SP, #-16]!
// ASSIGN
    ldr x0, =5
    str x0, [x10, #0]
// ASSIGN
    ldr x0, =32
    bl malloc
    str x0, [SP, #-16]!
    ldr x1, =v_Dog
    ldr x1, [x1]
    str x10, [SP, #-16]!
    mov x10, x0
    blr x1
    ldr x10, [SP], #16
    ldr x0, [SP], #16
    str x0, [x10, #8]
// ASSIGN
    b func4_end
func4:
    stp x29, x30, [SP, #-16]!
    ldr x3, =v_it0
    ldr x2, [x3]
    str x2, [SP, #-16]!
    str x0, [x3]
    ldr x3, =v_it1
    ldr x2, [x3]
    str x2, [SP, #-16]!
    str x1, [x3]
// ASSIGN
    mov x0, x10
    str x0, [SP, #-16]!
    mov x0, x10
    ldr x0, [x0, #0]
    str x0, [SP, #-16]!
    ldr x0, =v_it0
    ldr x0, [x0]
    mov x1, x0
    ldr x0, [SP], #16
    add x0, x0, x1
    mov x1, x0
    ldr x0, [SP], #16
    str x1, [x0, #0]
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
func4_end:
    ldr x0, =func4
    str x0, [x10, #24]
    ldp x29, x30, [SP], #16
    ret
class3_end:
    ldr x0, =class3
    ldr x1, =v_Cat
    str x0, [x1]
// ASSIGN
    ldr x0, =32
    bl malloc
    str x0, [SP, #-16]!
    ldr x1, =v_Cat
    ldr x1, [x1]
    str x10, [SP, #-16]!
    mov x10, x0
    blr x1
    ldr x10, [SP], #16
    ldr x0, [SP], #16
    ldr x1, =v_c
    str x0, [x1]
// print
    ldr x0, =v_c
    ldr x0, [x0]
    ldr x0, [x0, #0]
    mov x1, x0
    ldr x0, =fmt
    bl printf
    ldr x0, =v_c
    ldr x0, [x0]
    str x10, [SP, #-16]!
    mov x10, x0
    ldr x0, [x0, #24]
    str x0, [SP, #-16]!
    ldr x0, =3
    ldr x2, [SP], #16
    blr x2
    ldr x10, [SP], #16
// print
    ldr x0, =v_c
    ldr x0, [x0]
    ldr x0, [x0, #0]
    mov x1, x0
    ldr x0, =fmt
    bl printf
// print
    ldr x0, =v_c
    ldr x0, [x0]
    ldr x0, [x0, #8]
    ldr x0, [x0, #0]
    mov x1, x0
    ldr x0, =fmt
    bl printf
// print
    ldr x0, =v_c
    ldr x0, [x0]
    ldr x0, [x0, #8]
    ldr x0, [x0, #8]
    mov x1, x0
    ldr x0, =fmt
    bl printf
    ldr x0, =v_c
    ldr x0, [x0]
    ldr x0, [x0, #8]
    str x10, [SP, #-16]!
    mov x10, x0
    ldr x0, [x0, #16]
    str x0, [SP, #-16]!
    ldr x0, =2
    ldr x2, [SP], #16
    blr x2
    ldr x10, [SP], #16
// print
    ldr x0, =v_c
    ldr x0, [x0]
    ldr x0, [x0, #8]
    ldr x0, [x0, #8]
    mov x1, x0
    ldr x0, =fmt
    bl printf
    ldr x0, =v_c
    ldr x0, [x0]
    ldr x0, [x0, #8]
    str x10, [SP, #-16]!
    mov x10, x0
    ldr x0, [x0, #24]
    str x0, [SP, #-16]!
    ldr x0, =0
    ldr x2, [SP], #16
    blr x2
    ldr x10, [SP], #16
    mov x0, #0
    ldp x29, x30, [SP], #16
    ret

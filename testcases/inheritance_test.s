    .data
    .extern printf
fmt:
    .string "%ld\n"
    .align 7
v_argc:
    .quad -1
v_it:
    .quad 0
v_Organism:
    .quad 0
v_Object:
    .quad 0
v_age:
    .quad 0
v_isMulticelled:
    .quad 0
v_speak:
    .quad 0
v_ageInMonths:
    .quad 0
v_self:
    .quad 0
v_Animal:
    .quad 0
v_canSwim:
    .quad 0
v_makeOlder:
    .quad 0
v_doesBark:
    .quad 0
v_Dog:
    .quad 0
v_bark:
    .quad 0
v_Cat:
    .quad 0
v_meow:
    .quad 0
v_c:
    .quad 0
v_d:
    .quad 0
v_a:
    .quad 0
v_o:
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
    ldr x0, =0
    str x0, [x10, #0]
// ASSIGN
    ldr x0, =0
    str x0, [x10, #8]
// ASSIGN
    b func1_end
func1:
    stp x29, x30, [SP, #-16]!
    ldr x2, =v_it
    ldr x1, [x2]
    str x1, [SP, #-16]!
    str x0, [x2]
// print
    ldr x0, =1000
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
    str x0, [x10, #16]
// ASSIGN
    b func2_end
func2:
    stp x29, x30, [SP, #-16]!
    ldr x2, =v_it
    ldr x1, [x2]
    str x1, [SP, #-16]!
    str x0, [x2]
    ldr x0, =12
    str x0, [SP, #-16]!
    mov x0, x10
    ldr x0, [x0, #0]
    mov x1, x0
    ldr x0, [SP], #16
    mul x0, x0, x1
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
func2_end:
    ldr x0, =func2
    str x0, [x10, #24]
    ldp x29, x30, [SP], #16
    ret
class0_end:
    ldr x0, =class0
    ldr x1, =v_Organism
    str x0, [x1]
    b class3_end
class3:
    stp x29, x30, [SP, #-16]!
    ldr x0, =v_Organism
    ldr x0, [x0]
    blr x0
// ASSIGN
    ldr x0, =0
    str x0, [x10, #32]
// ASSIGN
    ldr x0, =1
    str x0, [x10, #8]
// ASSIGN
    b func4_end
func4:
    stp x29, x30, [SP, #-16]!
    ldr x2, =v_it
    ldr x1, [x2]
    str x1, [SP, #-16]!
    str x0, [x2]
// print
    ldr x0, =2000
    mov x1, x0
    ldr x0, =fmt
    bl printf
    ldr x1, [SP], #16
    ldr x2, =v_it
    str x1, [x2]
    ldp x29, x30, [SP], #16
    mov x0, #0
    ret
func4_end:
    ldr x0, =func4
    str x0, [x10, #16]
// ASSIGN
    b func5_end
func5:
    stp x29, x30, [SP, #-16]!
    ldr x2, =v_it
    ldr x1, [x2]
    str x1, [SP, #-16]!
    str x0, [x2]
// ASSIGN
    mov x0, x10
    str x0, [SP, #-16]!
    mov x0, x10
    ldr x0, [x0, #0]
    str x0, [SP, #-16]!
    ldr x0, =1
    mov x1, x0
    ldr x0, [SP], #16
    add x0, x0, x1
    mov x1, x0
    ldr x0, [SP], #16
    str x1, [x0, #0]
    ldr x1, [SP], #16
    ldr x2, =v_it
    str x1, [x2]
    ldp x29, x30, [SP], #16
    mov x0, #0
    ret
func5_end:
    ldr x0, =func5
    str x0, [x10, #40]
// ASSIGN
    ldr x0, =0
    str x0, [x10, #48]
    ldp x29, x30, [SP], #16
    ret
class3_end:
    ldr x0, =class3
    ldr x1, =v_Animal
    str x0, [x1]
    b class6_end
class6:
    stp x29, x30, [SP, #-16]!
    ldr x0, =v_Animal
    ldr x0, [x0]
    blr x0
// ASSIGN
    ldr x0, =1
    str x0, [x10, #32]
// ASSIGN
    ldr x0, =1
    str x0, [x10, #48]
// ASSIGN
    b func7_end
func7:
    stp x29, x30, [SP, #-16]!
    ldr x2, =v_it
    ldr x1, [x2]
    str x1, [SP, #-16]!
    str x0, [x2]
// print
    ldr x0, =3000
    mov x1, x0
    ldr x0, =fmt
    bl printf
    ldr x1, [SP], #16
    ldr x2, =v_it
    str x1, [x2]
    ldp x29, x30, [SP], #16
    mov x0, #0
    ret
func7_end:
    ldr x0, =func7
    str x0, [x10, #56]
// ASSIGN
    b func8_end
func8:
    stp x29, x30, [SP, #-16]!
    ldr x2, =v_it
    ldr x1, [x2]
    str x1, [SP, #-16]!
    str x0, [x2]
// print
    ldr x0, =7000
    mov x1, x0
    ldr x0, =fmt
    bl printf
    ldr x1, [SP], #16
    ldr x2, =v_it
    str x1, [x2]
    ldp x29, x30, [SP], #16
    mov x0, #0
    ret
func8_end:
    ldr x0, =func8
    str x0, [x10, #16]
    ldp x29, x30, [SP], #16
    ret
class6_end:
    ldr x0, =class6
    ldr x1, =v_Dog
    str x0, [x1]
    b class9_end
class9:
    stp x29, x30, [SP, #-16]!
    ldr x0, =v_Animal
    ldr x0, [x0]
    blr x0
// ASSIGN
    b func10_end
func10:
    stp x29, x30, [SP, #-16]!
    ldr x2, =v_it
    ldr x1, [x2]
    str x1, [SP, #-16]!
    str x0, [x2]
// print
    ldr x0, =4000
    mov x1, x0
    ldr x0, =fmt
    bl printf
    ldr x1, [SP], #16
    ldr x2, =v_it
    str x1, [x2]
    ldp x29, x30, [SP], #16
    mov x0, #0
    ret
func10_end:
    ldr x0, =func10
    str x0, [x10, #56]
    ldp x29, x30, [SP], #16
    ret
class9_end:
    ldr x0, =class9
    ldr x1, =v_Cat
    str x0, [x1]
// ASSIGN
    ldr x0, =64
    bl fun_malloc
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
// ASSIGN
    ldr x0, =64
    bl fun_malloc
    str x0, [SP, #-16]!
    ldr x1, =v_Dog
    ldr x1, [x1]
    str x10, [SP, #-16]!
    mov x10, x0
    blr x1
    ldr x10, [SP], #16
    ldr x0, [SP], #16
    ldr x1, =v_d
    str x0, [x1]
// ASSIGN
    ldr x0, =56
    bl fun_malloc
    str x0, [SP, #-16]!
    ldr x1, =v_Animal
    ldr x1, [x1]
    str x10, [SP, #-16]!
    mov x10, x0
    blr x1
    ldr x10, [SP], #16
    ldr x0, [SP], #16
    ldr x1, =v_a
    str x0, [x1]
// ASSIGN
    ldr x0, =32
    bl fun_malloc
    str x0, [SP, #-16]!
    ldr x1, =v_Organism
    ldr x1, [x1]
    str x10, [SP, #-16]!
    mov x10, x0
    blr x1
    ldr x10, [SP], #16
    ldr x0, [SP], #16
    ldr x1, =v_o
    str x0, [x1]
// print
    ldr x0, =v_o
    ldr x0, [x0]
    ldr x0, [x0, #0]
    mov x1, x0
    ldr x0, =fmt
    bl printf
// print
    ldr x0, =v_o
    ldr x0, [x0]
    ldr x0, [x0, #8]
    mov x1, x0
    ldr x0, =fmt
    bl printf
    ldr x0, =v_o
    ldr x0, [x0]
    str x10, [SP, #-16]!
    mov x10, x0
    ldr x0, [x0, #16]
    str x0, [SP, #-16]!
    ldr x0, =0
    ldr x1, [SP], #16
    blr x1
    ldr x10, [SP], #16
// print
    ldr x0, =v_o
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
// print
    ldr x0, =v_a
    ldr x0, [x0]
    ldr x0, [x0, #0]
    mov x1, x0
    ldr x0, =fmt
    bl printf
// print
    ldr x0, =v_a
    ldr x0, [x0]
    ldr x0, [x0, #8]
    mov x1, x0
    ldr x0, =fmt
    bl printf
    ldr x0, =v_a
    ldr x0, [x0]
    str x10, [SP, #-16]!
    mov x10, x0
    ldr x0, [x0, #16]
    str x0, [SP, #-16]!
    ldr x0, =0
    ldr x1, [SP], #16
    blr x1
    ldr x10, [SP], #16
// print
    ldr x0, =v_a
    ldr x0, [x0]
    ldr x0, [x0, #32]
    mov x1, x0
    ldr x0, =fmt
    bl printf
    ldr x0, =v_a
    ldr x0, [x0]
    str x10, [SP, #-16]!
    mov x10, x0
    ldr x0, [x0, #40]
    str x0, [SP, #-16]!
    ldr x0, =0
    ldr x1, [SP], #16
    blr x1
    ldr x10, [SP], #16
// print
    ldr x0, =v_a
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
// print
    ldr x0, =v_d
    ldr x0, [x0]
    ldr x0, [x0, #0]
    mov x1, x0
    ldr x0, =fmt
    bl printf
// print
    ldr x0, =v_d
    ldr x0, [x0]
    ldr x0, [x0, #32]
    mov x1, x0
    ldr x0, =fmt
    bl printf
    ldr x0, =v_d
    ldr x0, [x0]
    str x10, [SP, #-16]!
    mov x10, x0
    ldr x0, [x0, #56]
    str x0, [SP, #-16]!
    ldr x0, =0
    ldr x1, [SP], #16
    blr x1
    ldr x10, [SP], #16
// print
    ldr x0, =v_d
    ldr x0, [x0]
    ldr x0, [x0, #48]
    mov x1, x0
    ldr x0, =fmt
    bl printf
// print
    ldr x0, =v_c
    ldr x0, [x0]
    ldr x0, [x0, #48]
    mov x1, x0
    ldr x0, =fmt
    bl printf
    ldr x0, =v_c
    ldr x0, [x0]
    str x10, [SP, #-16]!
    mov x10, x0
    ldr x0, [x0, #56]
    str x0, [SP, #-16]!
    ldr x0, =0
    ldr x1, [SP], #16
    blr x1
    ldr x10, [SP], #16
// print
    ldr x0, =v_c
    ldr x0, [x0]
    ldr x0, [x0, #32]
    mov x1, x0
    ldr x0, =fmt
    bl printf
// ASSIGN
    ldr x0, =64
    bl fun_malloc
    str x0, [SP, #-16]!
    ldr x1, =v_Dog
    ldr x1, [x1]
    str x10, [SP, #-16]!
    mov x10, x0
    blr x1
    ldr x10, [SP], #16
    ldr x0, [SP], #16
    ldr x1, =v_a
    str x0, [x1]
// print
    ldr x0, =v_a
    ldr x0, [x0]
    ldr x0, [x0, #32]
    mov x1, x0
    ldr x0, =fmt
    bl printf
    ldr x0, =v_a
    ldr x0, [x0]
    str x10, [SP, #-16]!
    mov x10, x0
    ldr x0, [x0, #16]
    str x0, [SP, #-16]!
    ldr x0, =0
    ldr x1, [SP], #16
    blr x1
    ldr x10, [SP], #16
    mov x0, #0
    ldp x29, x30, [SP], #16
    ret

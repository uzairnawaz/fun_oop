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
v_it2:
    .quad 0
v_it3:
    .quad 0
v_it4:
    .quad 0
v_it5:
    .quad 0
v_it6:
    .quad 0
v_it7:
    .quad 0
v_a:
    .quad 0
v_int:
    .quad 0
v_Adder:
    .quad 0
v_Object:
    .quad 0
v_func3:
    .quad 0
v_func2:
    .quad 0
v_func1:
    .quad 0
v_func0:
    .quad 0
v_obj:
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
    ldr x9, =v_it0 
    ldr x8, [x9]
    str x8, [SP, #-16]!
    str x0, [x9]
    ldr x9, =v_it1 
    ldr x8, [x9]
    str x8, [SP, #-16]!
    str x1, [x9]
    ldr x9, =v_it2 
    ldr x8, [x9]
    str x8, [SP, #-16]!
    str x2, [x9]
    ldr x9, =v_it3 
    ldr x8, [x9]
    str x8, [SP, #-16]!
    str x3, [x9]
    ldr x9, =v_it4 
    ldr x8, [x9]
    str x8, [SP, #-16]!
    str x4, [x9]
    ldr x9, =v_it5 
    ldr x8, [x9]
    str x8, [SP, #-16]!
    str x5, [x9]
    ldr x9, =v_it6 
    ldr x8, [x9]
    str x8, [SP, #-16]!
    str x6, [x9]
    ldr x9, =v_it7 
    ldr x8, [x9]
    str x8, [SP, #-16]!
    str x7, [x9]
    ldr x0, =v_it2
    ldr x0, [x0]
    str x0, [SP, #-16]!
    ldr x0, =v_it1
    ldr x0, [x0]
    mov x1, x0
    ldr x0, [SP], #16
    mul x0, x0, x1
    str x0, [SP, #-16]!
    ldr x0, =v_it0
    ldr x0, [x0]
    mov x1, x0
    ldr x0, [SP], #16
    mul x0, x0, x1
    str x0, [SP, #-16]!
    ldr x0, =v_it3
    ldr x0, [x0]
    mov x1, x0
    ldr x0, [SP], #16
    mul x0, x0, x1
    ldr x1, [SP], #16
    ldr x2, =v_it7
    str x1, [x2]
    ldr x1, [SP], #16
    ldr x2, =v_it6
    str x1, [x2]
    ldr x1, [SP], #16
    ldr x2, =v_it5
    str x1, [x2]
    ldr x1, [SP], #16
    ldr x2, =v_it4
    str x1, [x2]
    ldr x1, [SP], #16
    ldr x2, =v_it3
    str x1, [x2]
    ldr x1, [SP], #16
    ldr x2, =v_it2
    str x1, [x2]
    ldr x1, [SP], #16
    ldr x2, =v_it1
    str x1, [x2]
    ldr x1, [SP], #16
    ldr x2, =v_it0
    str x1, [x2]
    ldp x29, x30, [SP], #16
    ret
    ldr x1, [SP], #16
    ldr x2, =v_it7
    str x1, [x2]
    ldr x1, [SP], #16
    ldr x2, =v_it6
    str x1, [x2]
    ldr x1, [SP], #16
    ldr x2, =v_it5
    str x1, [x2]
    ldr x1, [SP], #16
    ldr x2, =v_it4
    str x1, [x2]
    ldr x1, [SP], #16
    ldr x2, =v_it3
    str x1, [x2]
    ldr x1, [SP], #16
    ldr x2, =v_it2
    str x1, [x2]
    ldr x1, [SP], #16
    ldr x2, =v_it1
    str x1, [x2]
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
    ldr x0, =30
    str x0, [SP, #-16]!
    ldr x0, =20
    str x0, [SP, #-16]!
    ldr x0, =30
    str x0, [SP, #-16]!
    ldr x0, =50
    str x0, [SP, #-16]!
    ldr x3, [SP], #16
    ldr x2, [SP], #16
    ldr x1, [SP], #16
    ldr x0, [SP], #16
    ldr x8, [SP], #16
    blr x8
    mov x1, x0
    ldr x0, =fmt
    bl printf
    b class1_end
class1:
    stp x29, x30, [SP, #-16]!
// ASSIGN
    b func2_end
func2:
    stp x29, x30, [SP, #-16]!
    ldr x9, =v_it0 
    ldr x8, [x9]
    str x8, [SP, #-16]!
    str x0, [x9]
    ldr x9, =v_it1 
    ldr x8, [x9]
    str x8, [SP, #-16]!
    str x1, [x9]
    ldr x9, =v_it2 
    ldr x8, [x9]
    str x8, [SP, #-16]!
    str x2, [x9]
    ldr x9, =v_it3 
    ldr x8, [x9]
    str x8, [SP, #-16]!
    str x3, [x9]
    ldr x9, =v_it4 
    ldr x8, [x9]
    str x8, [SP, #-16]!
    str x4, [x9]
    ldr x9, =v_it5 
    ldr x8, [x9]
    str x8, [SP, #-16]!
    str x5, [x9]
    ldr x9, =v_it6 
    ldr x8, [x9]
    str x8, [SP, #-16]!
    str x6, [x9]
    ldr x9, =v_it7 
    ldr x8, [x9]
    str x8, [SP, #-16]!
    str x7, [x9]
    ldr x0, =v_it0
    ldr x0, [x0]
    str x0, [SP, #-16]!
    ldr x0, =v_it1
    ldr x0, [x0]
    mov x1, x0
    ldr x0, [SP], #16
    add x0, x0, x1
    str x0, [SP, #-16]!
    ldr x0, =v_it2
    ldr x0, [x0]
    mov x1, x0
    ldr x0, [SP], #16
    add x0, x0, x1
    ldr x1, [SP], #16
    ldr x2, =v_it7
    str x1, [x2]
    ldr x1, [SP], #16
    ldr x2, =v_it6
    str x1, [x2]
    ldr x1, [SP], #16
    ldr x2, =v_it5
    str x1, [x2]
    ldr x1, [SP], #16
    ldr x2, =v_it4
    str x1, [x2]
    ldr x1, [SP], #16
    ldr x2, =v_it3
    str x1, [x2]
    ldr x1, [SP], #16
    ldr x2, =v_it2
    str x1, [x2]
    ldr x1, [SP], #16
    ldr x2, =v_it1
    str x1, [x2]
    ldr x1, [SP], #16
    ldr x2, =v_it0
    str x1, [x2]
    ldp x29, x30, [SP], #16
    ret
    ldr x1, [SP], #16
    ldr x2, =v_it7
    str x1, [x2]
    ldr x1, [SP], #16
    ldr x2, =v_it6
    str x1, [x2]
    ldr x1, [SP], #16
    ldr x2, =v_it5
    str x1, [x2]
    ldr x1, [SP], #16
    ldr x2, =v_it4
    str x1, [x2]
    ldr x1, [SP], #16
    ldr x2, =v_it3
    str x1, [x2]
    ldr x1, [SP], #16
    ldr x2, =v_it2
    str x1, [x2]
    ldr x1, [SP], #16
    ldr x2, =v_it1
    str x1, [x2]
    ldr x1, [SP], #16
    ldr x2, =v_it0
    str x1, [x2]
    ldp x29, x30, [SP], #16
    mov x0, #0
    ret
func2_end:
    ldr x0, =func2
    str x0, [x10, #0]
// ASSIGN
    b func3_end
func3:
    stp x29, x30, [SP, #-16]!
    ldr x9, =v_it0 
    ldr x8, [x9]
    str x8, [SP, #-16]!
    str x0, [x9]
    ldr x9, =v_it1 
    ldr x8, [x9]
    str x8, [SP, #-16]!
    str x1, [x9]
    ldr x9, =v_it2 
    ldr x8, [x9]
    str x8, [SP, #-16]!
    str x2, [x9]
    ldr x9, =v_it3 
    ldr x8, [x9]
    str x8, [SP, #-16]!
    str x3, [x9]
    ldr x9, =v_it4 
    ldr x8, [x9]
    str x8, [SP, #-16]!
    str x4, [x9]
    ldr x9, =v_it5 
    ldr x8, [x9]
    str x8, [SP, #-16]!
    str x5, [x9]
    ldr x9, =v_it6 
    ldr x8, [x9]
    str x8, [SP, #-16]!
    str x6, [x9]
    ldr x9, =v_it7 
    ldr x8, [x9]
    str x8, [SP, #-16]!
    str x7, [x9]
    ldr x0, =v_it0
    ldr x0, [x0]
    str x0, [SP, #-16]!
    ldr x0, =v_it1
    ldr x0, [x0]
    mov x1, x0
    ldr x0, [SP], #16
    add x0, x0, x1
    ldr x1, [SP], #16
    ldr x2, =v_it7
    str x1, [x2]
    ldr x1, [SP], #16
    ldr x2, =v_it6
    str x1, [x2]
    ldr x1, [SP], #16
    ldr x2, =v_it5
    str x1, [x2]
    ldr x1, [SP], #16
    ldr x2, =v_it4
    str x1, [x2]
    ldr x1, [SP], #16
    ldr x2, =v_it3
    str x1, [x2]
    ldr x1, [SP], #16
    ldr x2, =v_it2
    str x1, [x2]
    ldr x1, [SP], #16
    ldr x2, =v_it1
    str x1, [x2]
    ldr x1, [SP], #16
    ldr x2, =v_it0
    str x1, [x2]
    ldp x29, x30, [SP], #16
    ret
    ldr x1, [SP], #16
    ldr x2, =v_it7
    str x1, [x2]
    ldr x1, [SP], #16
    ldr x2, =v_it6
    str x1, [x2]
    ldr x1, [SP], #16
    ldr x2, =v_it5
    str x1, [x2]
    ldr x1, [SP], #16
    ldr x2, =v_it4
    str x1, [x2]
    ldr x1, [SP], #16
    ldr x2, =v_it3
    str x1, [x2]
    ldr x1, [SP], #16
    ldr x2, =v_it2
    str x1, [x2]
    ldr x1, [SP], #16
    ldr x2, =v_it1
    str x1, [x2]
    ldr x1, [SP], #16
    ldr x2, =v_it0
    str x1, [x2]
    ldp x29, x30, [SP], #16
    mov x0, #0
    ret
func3_end:
    ldr x0, =func3
    str x0, [x10, #8]
// ASSIGN
    b func4_end
func4:
    stp x29, x30, [SP, #-16]!
    ldr x9, =v_it0 
    ldr x8, [x9]
    str x8, [SP, #-16]!
    str x0, [x9]
    ldr x9, =v_it1 
    ldr x8, [x9]
    str x8, [SP, #-16]!
    str x1, [x9]
    ldr x9, =v_it2 
    ldr x8, [x9]
    str x8, [SP, #-16]!
    str x2, [x9]
    ldr x9, =v_it3 
    ldr x8, [x9]
    str x8, [SP, #-16]!
    str x3, [x9]
    ldr x9, =v_it4 
    ldr x8, [x9]
    str x8, [SP, #-16]!
    str x4, [x9]
    ldr x9, =v_it5 
    ldr x8, [x9]
    str x8, [SP, #-16]!
    str x5, [x9]
    ldr x9, =v_it6 
    ldr x8, [x9]
    str x8, [SP, #-16]!
    str x6, [x9]
    ldr x9, =v_it7 
    ldr x8, [x9]
    str x8, [SP, #-16]!
    str x7, [x9]
    ldr x0, =v_it0
    ldr x0, [x0]
    ldr x1, [SP], #16
    ldr x2, =v_it7
    str x1, [x2]
    ldr x1, [SP], #16
    ldr x2, =v_it6
    str x1, [x2]
    ldr x1, [SP], #16
    ldr x2, =v_it5
    str x1, [x2]
    ldr x1, [SP], #16
    ldr x2, =v_it4
    str x1, [x2]
    ldr x1, [SP], #16
    ldr x2, =v_it3
    str x1, [x2]
    ldr x1, [SP], #16
    ldr x2, =v_it2
    str x1, [x2]
    ldr x1, [SP], #16
    ldr x2, =v_it1
    str x1, [x2]
    ldr x1, [SP], #16
    ldr x2, =v_it0
    str x1, [x2]
    ldp x29, x30, [SP], #16
    ret
    ldr x1, [SP], #16
    ldr x2, =v_it7
    str x1, [x2]
    ldr x1, [SP], #16
    ldr x2, =v_it6
    str x1, [x2]
    ldr x1, [SP], #16
    ldr x2, =v_it5
    str x1, [x2]
    ldr x1, [SP], #16
    ldr x2, =v_it4
    str x1, [x2]
    ldr x1, [SP], #16
    ldr x2, =v_it3
    str x1, [x2]
    ldr x1, [SP], #16
    ldr x2, =v_it2
    str x1, [x2]
    ldr x1, [SP], #16
    ldr x2, =v_it1
    str x1, [x2]
    ldr x1, [SP], #16
    ldr x2, =v_it0
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
    ldr x9, =v_it0 
    ldr x8, [x9]
    str x8, [SP, #-16]!
    str x0, [x9]
    ldr x9, =v_it1 
    ldr x8, [x9]
    str x8, [SP, #-16]!
    str x1, [x9]
    ldr x9, =v_it2 
    ldr x8, [x9]
    str x8, [SP, #-16]!
    str x2, [x9]
    ldr x9, =v_it3 
    ldr x8, [x9]
    str x8, [SP, #-16]!
    str x3, [x9]
    ldr x9, =v_it4 
    ldr x8, [x9]
    str x8, [SP, #-16]!
    str x4, [x9]
    ldr x9, =v_it5 
    ldr x8, [x9]
    str x8, [SP, #-16]!
    str x5, [x9]
    ldr x9, =v_it6 
    ldr x8, [x9]
    str x8, [SP, #-16]!
    str x6, [x9]
    ldr x9, =v_it7 
    ldr x8, [x9]
    str x8, [SP, #-16]!
    str x7, [x9]
// print
    ldr x0, =213213456
    mov x1, x0
    ldr x0, =fmt
    bl printf
    ldr x0, =3
    ldr x1, [SP], #16
    ldr x2, =v_it7
    str x1, [x2]
    ldr x1, [SP], #16
    ldr x2, =v_it6
    str x1, [x2]
    ldr x1, [SP], #16
    ldr x2, =v_it5
    str x1, [x2]
    ldr x1, [SP], #16
    ldr x2, =v_it4
    str x1, [x2]
    ldr x1, [SP], #16
    ldr x2, =v_it3
    str x1, [x2]
    ldr x1, [SP], #16
    ldr x2, =v_it2
    str x1, [x2]
    ldr x1, [SP], #16
    ldr x2, =v_it1
    str x1, [x2]
    ldr x1, [SP], #16
    ldr x2, =v_it0
    str x1, [x2]
    ldp x29, x30, [SP], #16
    ret
    ldr x1, [SP], #16
    ldr x2, =v_it7
    str x1, [x2]
    ldr x1, [SP], #16
    ldr x2, =v_it6
    str x1, [x2]
    ldr x1, [SP], #16
    ldr x2, =v_it5
    str x1, [x2]
    ldr x1, [SP], #16
    ldr x2, =v_it4
    str x1, [x2]
    ldr x1, [SP], #16
    ldr x2, =v_it3
    str x1, [x2]
    ldr x1, [SP], #16
    ldr x2, =v_it2
    str x1, [x2]
    ldr x1, [SP], #16
    ldr x2, =v_it1
    str x1, [x2]
    ldr x1, [SP], #16
    ldr x2, =v_it0
    str x1, [x2]
    ldp x29, x30, [SP], #16
    mov x0, #0
    ret
func5_end:
    ldr x0, =func5
    str x0, [x10, #24]
    ldp x29, x30, [SP], #16
    ret
class1_end:
    ldr x0, =class1
    ldr x1, =v_Adder
    str x0, [x1]
// ASSIGN
    ldr x0, =32
    bl fun_malloc
    str x0, [SP, #-16]!
    ldr x1, =v_Adder
    ldr x1, [x1]
    str x10, [SP, #-16]!
    mov x10, x0
    blr x1
    ldr x10, [SP], #16
    ldr x0, [SP], #16
    ldr x1, =v_obj
    str x0, [x1]
// print
    ldr x0, =v_obj
    ldr x0, [x0]
    str x10, [SP, #-16]!
    mov x10, x0
    ldr x0, [x0, #0]
    str x0, [SP, #-16]!
    ldr x0, =1
    str x0, [SP, #-16]!
    ldr x0, =2
    str x0, [SP, #-16]!
    ldr x0, =3
    str x0, [SP, #-16]!
    ldr x2, [SP], #16
    ldr x1, [SP], #16
    ldr x0, [SP], #16
    ldr x8, [SP], #16
    blr x8
    ldr x10, [SP], #16
    mov x1, x0
    ldr x0, =fmt
    bl printf
// print
    ldr x0, =v_obj
    ldr x0, [x0]
    str x10, [SP, #-16]!
    mov x10, x0
    ldr x0, [x0, #8]
    str x0, [SP, #-16]!
    ldr x0, =1
    str x0, [SP, #-16]!
    ldr x0, =2
    str x0, [SP, #-16]!
    ldr x1, [SP], #16
    ldr x0, [SP], #16
    ldr x8, [SP], #16
    blr x8
    ldr x10, [SP], #16
    mov x1, x0
    ldr x0, =fmt
    bl printf
// print
    ldr x0, =v_obj
    ldr x0, [x0]
    str x10, [SP, #-16]!
    mov x10, x0
    ldr x0, [x0, #16]
    str x0, [SP, #-16]!
    ldr x0, =1
    str x0, [SP, #-16]!
    ldr x0, [SP], #16
    ldr x8, [SP], #16
    blr x8
    ldr x10, [SP], #16
    mov x1, x0
    ldr x0, =fmt
    bl printf
// print
    ldr x0, =v_obj
    ldr x0, [x0]
    str x10, [SP, #-16]!
    mov x10, x0
    ldr x0, [x0, #24]
    str x0, [SP, #-16]!
    ldr x8, [SP], #16
    blr x8
    ldr x10, [SP], #16
    mov x1, x0
    ldr x0, =fmt
    bl printf
    mov x0, #0
    ldp x29, x30, [SP], #16
    ret

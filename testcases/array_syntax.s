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
v_Abc:
    .quad 0
v_Object:
    .quad 0
v_idk:
    .quad 0
v_idk2:
    .quad 0
v_idkFunc:
    .quad 0
v_self:
    .quad 0
v_Animal:
    .quad 0
v_size:
    .quad 0
v_height:
    .quad 0
v_randomFunc:
    .quad 0
v_abc:
    .quad 0
v_arr:
    .quad 0
v_i:
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
    ldr x0, =2
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
    mov x0, x10
    ldr x0, [x0, #0]
    str x0, [SP, #-16]!
    mov x0, x10
    ldr x0, [x0, #8]
    mov x1, x0
    ldr x0, [SP], #16
    add x0, x0, x1
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
func1_end:
    ldr x0, =func1
    str x0, [x10, #16]
    ldp x29, x30, [SP], #16
    ret
class0_end:
    ldr x0, =class0
    ldr x1, =v_Abc
    str x0, [x1]
    b class2_end
class2:
    stp x29, x30, [SP, #-16]!
// ASSIGN
    ldr x0, =10
    str x0, [x10, #0]
// ASSIGN
    ldr x0, =4
    str x0, [x10, #8]
// ASSIGN
    b func3_end
func3:
    stp x29, x30, [SP, #-16]!
    ldr x3, =v_it0
    ldr x2, [x3]
    str x2, [SP, #-16]!
    str x0, [x3]
    ldr x3, =v_it1
    ldr x2, [x3]
    str x2, [SP, #-16]!
    str x1, [x3]
    mov x0, x10
    ldr x0, [x0, #0]
    str x0, [SP, #-16]!
    mov x0, x10
    ldr x0, [x0, #8]
    mov x1, x0
    ldr x0, [SP], #16
    mul x0, x0, x1
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
func3_end:
    ldr x0, =func3
    str x0, [x10, #16]
// ASSIGN
    ldr x0, =24
    bl malloc
    str x0, [SP, #-16]!
    ldr x1, =v_Abc
    ldr x1, [x1]
    str x10, [SP, #-16]!
    mov x10, x0
    blr x1
    ldr x10, [SP], #16
    ldr x0, [SP], #16
    str x0, [x10, #24]
    ldp x29, x30, [SP], #16
    ret
class2_end:
    ldr x0, =class2
    ldr x1, =v_Animal
    str x0, [x1]
// ASSIGN
    ldr x0, =4
    str x0, [SP, #-16]!
    ldr x1, =8
    mul x0, x0, x1
    bl malloc
    ldr x2, [SP], #16
    str x10, [SP, #-16]!
    str x0, [SP, #-16]!
    mov x3, x0
instantiate_array_4:
    str x2, [SP, #-16]!
    str x3, [SP, #-16]!
    ldr x0, =32
    bl malloc
    ldr x3, [SP]
    str x0, [x3]
    mov x10, x0
    ldr x1, =v_Animal
    ldr x1, [x1]
    blr x1
    ldr x3, [SP], #16
    ldr x2, [SP], #16
    add x3, x3, #8
    sub x2, x2, #1
    cbnz x2, instantiate_array_4
    ldr x0, [SP], #16
    ldr x10, [SP], #16
    ldr x1, =v_arr
    str x0, [x1]
// ASSIGN
    ldr x0, =0
    ldr x1, =v_i
    str x0, [x1]
    ldr x0, =v_i
    ldr x0, [x0]
    str x0, [SP, #-16]!
    ldr x0, =4
    mov x1, x0
    ldr x0, [SP], #16
    cmp x0, x1
    blo condition5
    mov x0, #0
    b condition5_end
condition5:
    mov x0, #1
condition5_end:
    cbz x0, loop_end6
loop6:
// print
    ldr x0, =v_arr
    ldr x0, [x0]
    str x0, [SP, #-16]!
    ldr x0, =v_i
    ldr x0, [x0]
    mov x1, x0
    ldr x0, [SP], #16
    ldr x2, =8
    mul x1, x1, x2
    add x0, x0, x1
    ldr x0, [x0]
    ldr x0, [x0, #0]
    mov x1, x0
    ldr x0, =fmt
    bl printf
// print
    ldr x0, =v_arr
    ldr x0, [x0]
    str x0, [SP, #-16]!
    ldr x0, =v_i
    ldr x0, [x0]
    mov x1, x0
    ldr x0, [SP], #16
    ldr x2, =8
    mul x1, x1, x2
    add x0, x0, x1
    ldr x0, [x0]
    ldr x0, [x0, #8]
    mov x1, x0
    ldr x0, =fmt
    bl printf
// print
    ldr x0, =v_arr
    ldr x0, [x0]
    str x0, [SP, #-16]!
    ldr x0, =v_i
    ldr x0, [x0]
    mov x1, x0
    ldr x0, [SP], #16
    ldr x2, =8
    mul x1, x1, x2
    add x0, x0, x1
    ldr x0, [x0]
    str x10, [SP, #-16]!
    mov x10, x0
    ldr x0, [x0, #16]
    str x0, [SP, #-16]!
    ldr x0, =0
    ldr x2, [SP], #16
    blr x2
    ldr x10, [SP], #16
    mov x1, x0
    ldr x0, =fmt
    bl printf
// print
    ldr x0, =v_arr
    ldr x0, [x0]
    str x0, [SP, #-16]!
    ldr x0, =v_i
    ldr x0, [x0]
    mov x1, x0
    ldr x0, [SP], #16
    ldr x2, =8
    mul x1, x1, x2
    add x0, x0, x1
    ldr x0, [x0]
    ldr x0, [x0, #24]
    ldr x0, [x0, #0]
    mov x1, x0
    ldr x0, =fmt
    bl printf
// print
    ldr x0, =v_arr
    ldr x0, [x0]
    str x0, [SP, #-16]!
    ldr x0, =v_i
    ldr x0, [x0]
    mov x1, x0
    ldr x0, [SP], #16
    ldr x2, =8
    mul x1, x1, x2
    add x0, x0, x1
    ldr x0, [x0]
    ldr x0, [x0, #24]
    ldr x0, [x0, #8]
    mov x1, x0
    ldr x0, =fmt
    bl printf
// print
    ldr x0, =v_arr
    ldr x0, [x0]
    str x0, [SP, #-16]!
    ldr x0, =v_i
    ldr x0, [x0]
    mov x1, x0
    ldr x0, [SP], #16
    ldr x2, =8
    mul x1, x1, x2
    add x0, x0, x1
    ldr x0, [x0]
    ldr x0, [x0, #24]
    str x10, [SP, #-16]!
    mov x10, x0
    ldr x0, [x0, #16]
    str x0, [SP, #-16]!
    ldr x0, =0
    ldr x2, [SP], #16
    blr x2
    ldr x10, [SP], #16
    mov x1, x0
    ldr x0, =fmt
    bl printf
// ASSIGN
    ldr x0, =v_i
    ldr x0, [x0]
    str x0, [SP, #-16]!
    ldr x0, =1
    mov x1, x0
    ldr x0, [SP], #16
    add x0, x0, x1
    ldr x1, =v_i
    str x0, [x1]
    ldr x0, =v_i
    ldr x0, [x0]
    str x0, [SP, #-16]!
    ldr x0, =4
    mov x1, x0
    ldr x0, [SP], #16
    cmp x0, x1
    blo condition7
    mov x0, #0
    b condition7_end
condition7:
    mov x0, #1
condition7_end:
    cbnz x0, loop6
loop_end6:
// ASSIGN
    ldr x0, =v_arr
    ldr x0, [x0]
    str x0, [SP, #-16]!
    ldr x0, =1
    mov x1, x0
    ldr x0, [SP], #16
    ldr x2, =8
    mul x1, x1, x2
    add x0, x0, x1
    ldr x0, [x0]
    str x0, [SP, #-16]!
    ldr x0, =20
    mov x1, x0
    ldr x0, [SP], #16
    str x1, [x0, #0]
// ASSIGN
    ldr x0, =v_arr
    ldr x0, [x0]
    str x0, [SP, #-16]!
    ldr x0, =2
    mov x1, x0
    ldr x0, [SP], #16
    ldr x2, =8
    mul x1, x1, x2
    add x0, x0, x1
    ldr x0, [x0]
    str x0, [SP, #-16]!
    ldr x0, =30
    mov x1, x0
    ldr x0, [SP], #16
    str x1, [x0, #0]
// ASSIGN
    ldr x0, =0
    ldr x1, =v_i
    str x0, [x1]
    ldr x0, =v_i
    ldr x0, [x0]
    str x0, [SP, #-16]!
    ldr x0, =4
    mov x1, x0
    ldr x0, [SP], #16
    cmp x0, x1
    blo condition8
    mov x0, #0
    b condition8_end
condition8:
    mov x0, #1
condition8_end:
    cbz x0, loop_end9
loop9:
// print
    ldr x0, =v_arr
    ldr x0, [x0]
    str x0, [SP, #-16]!
    ldr x0, =v_i
    ldr x0, [x0]
    mov x1, x0
    ldr x0, [SP], #16
    ldr x2, =8
    mul x1, x1, x2
    add x0, x0, x1
    ldr x0, [x0]
    ldr x0, [x0, #0]
    mov x1, x0
    ldr x0, =fmt
    bl printf
// ASSIGN
    ldr x0, =v_i
    ldr x0, [x0]
    str x0, [SP, #-16]!
    ldr x0, =1
    mov x1, x0
    ldr x0, [SP], #16
    add x0, x0, x1
    ldr x1, =v_i
    str x0, [x1]
    ldr x0, =v_i
    ldr x0, [x0]
    str x0, [SP, #-16]!
    ldr x0, =4
    mov x1, x0
    ldr x0, [SP], #16
    cmp x0, x1
    blo condition10
    mov x0, #0
    b condition10_end
condition10:
    mov x0, #1
condition10_end:
    cbnz x0, loop9
loop_end9:
// ASSIGN
    ldr x0, =v_arr
    ldr x0, [x0]
    str x0, [SP, #-16]!
    ldr x0, =1
    str x0, [SP, #-16]!
    ldr x0, =32
    bl malloc
    str x0, [SP, #-16]!
    ldr x1, =v_Animal
    ldr x1, [x1]
    str x10, [SP, #-16]!
    mov x10, x0
    blr x1
    ldr x10, [SP], #16
    ldr x0, [SP], #16
    mov x2, x0
    ldr x1, [SP], #16
    ldr x0, [SP], #16
    ldr x4, =8
    mul x1, x1, x4
    add x0, x0, x1
    str x2, [x0]
// ASSIGN
    ldr x0, =0
    ldr x1, =v_i
    str x0, [x1]
    ldr x0, =v_i
    ldr x0, [x0]
    str x0, [SP, #-16]!
    ldr x0, =4
    mov x1, x0
    ldr x0, [SP], #16
    cmp x0, x1
    blo condition11
    mov x0, #0
    b condition11_end
condition11:
    mov x0, #1
condition11_end:
    cbz x0, loop_end12
loop12:
// print
    ldr x0, =v_arr
    ldr x0, [x0]
    str x0, [SP, #-16]!
    ldr x0, =v_i
    ldr x0, [x0]
    mov x1, x0
    ldr x0, [SP], #16
    ldr x2, =8
    mul x1, x1, x2
    add x0, x0, x1
    ldr x0, [x0]
    ldr x0, [x0, #0]
    mov x1, x0
    ldr x0, =fmt
    bl printf
// ASSIGN
    ldr x0, =v_i
    ldr x0, [x0]
    str x0, [SP, #-16]!
    ldr x0, =1
    mov x1, x0
    ldr x0, [SP], #16
    add x0, x0, x1
    ldr x1, =v_i
    str x0, [x1]
    ldr x0, =v_i
    ldr x0, [x0]
    str x0, [SP, #-16]!
    ldr x0, =4
    mov x1, x0
    ldr x0, [SP], #16
    cmp x0, x1
    blo condition13
    mov x0, #0
    b condition13_end
condition13:
    mov x0, #1
condition13_end:
    cbnz x0, loop12
loop_end12:
    mov x0, #0
    ldp x29, x30, [SP], #16
    ret

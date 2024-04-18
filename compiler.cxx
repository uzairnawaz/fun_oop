
#include "compiler.h"


/*
 Compiles the given fun program
 Compiled code is printed to stdout
*/
void FunCompiler::compile(const char* program) {
    astRoot = ast_create(program);

    // ast_display(astRoot, 0);
    // for (auto it=varTypes.begin(); it!=varTypes.end(); it++) {
    //     slice_print(it->first);
    //     printf(" ");
    //     slice_print(it->second);
    //     printf("\n");
    // }

    preprocess();    

    printf("main:\n");
    printf("    stp x29, x30, [SP, #-16]!\n");
    printf("    ldr x1, =v_argc\n");
    printf("    str x0, [x1]\n");
    compile_ast(astRoot);    
    printf("    mov x0, #0\n");
    printf("    ldp x29, x30, [SP], #16\n");
    printf("    ret\n");

    ast_free(astRoot);
}

/*
 Generate the data section of the program
*/
void FunCompiler::preprocess() {
    printf("    .data\n");
    printf("    .extern printf\n");
    printf("fmt:\n");
    printf("    .string \"%%ld\\n\"\n");
    printf("    .align 7\n");
    printf("v_argc:\n");
    printf("    .quad -1\n");
    printf("v_it:\n");
    printf("    .quad 0\n");

    std::unordered_set<Slice, slice_hash_func, slice_equals_func> varNames;
    varNames.insert(Slice{"argc", 4});
    varNames.insert(Slice{"it", 2});
    preprocessVars(astRoot, &varNames);

    printf("    .text\n");
    printf("    .global main\n");
}

/*
 Read in all the variables from the program and store them in the data section of the program
*/
void FunCompiler::preprocessVars(ASTNode* ast,
        std::unordered_set<Slice, slice_hash_func, slice_equals_func>* varNames) {
    if (ast->type == IDENTIFIER) {
        Slice varName = ast->identifier;
        if (varNames->count(varName) == 0) {
            varNames->insert(varName);
            printf("v_");
            slice_print(ast->identifier);
            printf(":\n");
            printf("    .quad 0\n");
        }
    } else {
        for (int i = 0; i < ast->numChildren; i++) {
            preprocessVars(ast->children[i], varNames);
        }
    }
}

/*
 Load the two children of an AST Node into registers x0 and x1 in that order.
*/
void FunCompiler::loadBinaryChildrenReg(ASTNode* ast) {
    compile_ast(ast->children[0]);
    printf("    str x0, [SP, #-16]!\n");
    compile_ast(ast->children[1]);
    printf("    mov x1, x0\n");
    printf("    ldr x0, [SP], #16\n");
}


/*
 Determines the type of the value at an ASTNode 
*/
ClassNode* FunCompiler::determineType(ASTNode* ast) {
    switch (ast->type)
    {
    case IDENTIFIER:
        return classNames.at(ast->identifier);
    case FUNC_CALL:
        // TODO :)
        return nullptr;
    case ACCESS:
        return determineType(ast->children[0])->memberTypes.at(ast->children[1]->identifier);
    default:
        return nullptr;
    }
}

/*
 Compiles the AST recursively
 The result of any operation is guaranteed to be stored in x0
*/
void FunCompiler::compile_ast(ASTNode* ast) {
    switch (ast->type) {
        case DECLARATION: {
            Slice type = ast->children[0]->identifier;
            ClassNode* classNode = classNames.at(type); 

            printf("    mov x0, %ld\n", classNode->getSize());
            printf("    bl malloc\n");

            printf("    ldr x1, =v_");
            slice_print(ast->children[1]->identifier);
            printf("\n");

            printf("    str x0, [x1]\n"); // variable now stores a pointer to the allocated memory
            break;
        }
        case ACCESS: {
            ClassNode* type = determineType(ast->children[0]);
            uint64_t idx = type->memberPos.at(ast->children[1]->identifier);

            compile_ast(ast->children[0]);
            printf("    ldr x0, [x0, #%ld]\n", idx);
        }
        case FUN: {
            int labelNum = labelCounter++; 
            printf("    b func%d_end\n", labelNum);
            printf("func%d:\n", labelNum);
            printf("    stp x29, x30, [SP, #-16]!\n"); // store frame pointer and link register 
            printf("    ldr x2, =v_it\n");
            printf("    ldr x1, [x2]\n");
            printf("    str x1, [SP, #-16]!\n"); // store the old "it" value on the stack
            printf("    str x0, [x2]\n"); // update the value of "it" with x0 (function input)
            compile_ast(ast->children[0]);  

            // default return  
            printf("    ldr x1, [SP], #16\n");
            printf("    ldr x2, =v_it\n");
            printf("    str x1, [x2]\n");
            printf("    ldp x29, x30, [SP], #16\n");
            printf("    mov x0, #0\n");
            printf("    ret\n");

            printf("func%d_end:\n", labelNum);
            printf("    ldr x0, =func%d\n", labelNum);
            return;
        }
        case WHILE: { 
            compile_ast(ast->children[0]); // evaluate condition 
            int labelNum = labelCounter++;    
            printf("    cbz x0, loop_end%d\n", labelNum); // skip if condition == 0
            printf("loop%d:\n", labelNum);
            compile_ast(ast->children[1]);
            compile_ast(ast->children[0]); // reevaluate condition 
            printf("    cbnz x0, loop%d\n", labelNum); // jump to start of loop if condition true
            printf("loop_end%d:\n", labelNum);
            return;
        }
        case IF: {    
            compile_ast(ast->children[0]); // evaluate condition
            int labelNum = labelCounter++;    
            printf("    cbz x0, else%d\n", labelNum); // jump to else if condition failed
            printf("if%d:\n", labelNum);
            compile_ast(ast->children[1]);
            printf("    b if_end%d\n", labelNum); 
            printf("else%d:\n", labelNum);
            if (ast->numChildren == 3) { // if this "if" statement had an "else" clause
                compile_ast(ast->children[2]); // compile the else clause 
            }
            printf("if_end%d:\n", labelNum);
            return;
        }
        case PRINT: 
            compile_ast(ast->children[0]);
            printf("    mov x1, x0\n"); // integer to print goes in x1
            printf("    ldr x0, =fmt\n"); 
            printf("    bl printf\n");
            return;
        case RETURN:
            if (ast->children[0]->type == FUNC_CALL) { // tail call optimization
                compile_ast(ast->children[0]->children[0]); // function address stored in x0
                printf("    str x0, [SP, #-16]!\n");
                compile_ast(ast->children[0]->children[1]); // function input stored in x0
                printf("    ldr x1, [SP], #16\n");

                printf("    ldr x2, [SP], #16\n");
                printf("    ldr x3, =v_it\n");
                printf("    str x2, [x3]\n");
                printf("    ldp x29, x30, [SP], #16\n");

                printf("    br x1\n"); // call function, WITHOUT linking
            } else {
                compile_ast(ast->children[0]);
                printf("    ldr x1, [SP], #16\n");
                printf("    ldr x2, =v_it\n");
                printf("    str x1, [x2]\n");
                printf("    ldp x29, x30, [SP], #16\n");
                printf("    ret\n");
            }
            return;
        case PLUS:  
            loadBinaryChildrenReg(ast);
            printf("    add x0, x0, x1\n");
            return;
        case MINUS: 
            loadBinaryChildrenReg(ast);
            printf("    sub x0, x0, x1\n");
            return;
        case MULT:   
            loadBinaryChildrenReg(ast);
            printf("    mul x0, x0, x1\n");
            return;
        case DIV:
            loadBinaryChildrenReg(ast);
            printf("    udiv x0, x0, x1\n");
            return;
        case MOD: 
            // a % b = a - b * (a / b)
            loadBinaryChildrenReg(ast);
            printf("    udiv x2, x0, x1\n");
            printf("    msub x3, x2, x1, x0\n");
            
            // if b is 0, a % b = 0
            printf("    udiv x4, x1, x1\n");
            printf("    mul x0, x3, x4\n");
            return;
        case LESS_EQUAL: { 
            loadBinaryChildrenReg(ast);
            int labelNum = labelCounter++;
            printf("    cmp x0, x1\n");
            printf("    bls condition%d\n", labelNum);
            printf("    mov x0, #0\n");
            printf("    b condition%d_end\n", labelNum);
            printf("condition%d:\n", labelNum);
            printf("    mov x0, #1\n");
            printf("condition%d_end:\n", labelNum);
            return;
        }
        case GREATER_EQUAL: {  
            loadBinaryChildrenReg(ast);
            int labelNum = labelCounter++;
            printf("    cmp x0, x1\n");
            printf("    bhs condition%d\n", labelNum);
            printf("    mov x0, #0\n");
            printf("    b condition%d_end\n", labelNum);
            printf("condition%d:\n", labelNum);
            printf("    mov x0, #1\n");
            printf("condition%d_end:\n", labelNum);
            return;
        }
        case LESS: { 
            loadBinaryChildrenReg(ast);
            int labelNum = labelCounter++;
            printf("    cmp x0, x1\n");
            printf("    blo condition%d\n", labelNum);
            printf("    mov x0, #0\n");
            printf("    b condition%d_end\n", labelNum);
            printf("condition%d:\n", labelNum);
            printf("    mov x0, #1\n");
            printf("condition%d_end:\n", labelNum);
            return;
        }
        case GREATER: { 
            loadBinaryChildrenReg(ast);
            int labelNum = labelCounter++;
            printf("    cmp x0, x1\n");
            printf("    bhi condition%d\n", labelNum);
            printf("    mov x0, #0\n");
            printf("    b condition%d_end\n", labelNum);
            printf("condition%d:\n", labelNum);
            printf("    mov x0, #1\n");
            printf("condition%d_end:\n", labelNum);
            return;
        }
        case EQUAL: { 
            loadBinaryChildrenReg(ast);
            int labelNum = labelCounter++;
            printf("    cmp x0, x1\n");
            printf("    beq condition%d\n", labelNum);
            printf("    mov x0, #0\n");
            printf("    b condition%d_end\n", labelNum);
            printf("condition%d:\n", labelNum);
            printf("    mov x0, #1\n");
            printf("condition%d_end:\n", labelNum);
            return;
        }
        case NOT_EQUAL: {
            loadBinaryChildrenReg(ast);
            int labelNum = labelCounter++;
            printf("    cmp x0, x1\n");
            printf("    bne condition%d\n", labelNum);
            printf("    mov x0, #0\n");
            printf("    b condition%d_end\n", labelNum);
            printf("condition%d:\n", labelNum);
            printf("    mov x0, #1\n");
            printf("condition%d_end:\n", labelNum);
            return;
        }
        case ASSIGN: { 
            /*
            Two options:
            var = value
                   ASSIGN
                   /    \
            var_name   value

                        ASSIGN
                        /    \
                DECLARATION  value
                /     \
            type     var_name
            */
            compile_ast(ast->children[1]);
            printf("    ldr x1, =v_");
            if (ast->children[0]->type == DECLARATION) {
                slice_print(ast->children[0]->children[1]->identifier);
            } else if (ast->children[0]->type == IDENTIFIER) {
                slice_print(ast->children[0]->identifier);
            } else {
                // BAD! ERROR!!
            }
            printf("\n");
            printf("    str x0, [x1]\n");
            return;
        }
        case LOG_AND: { 
            compile_ast(ast->children[0]);
            int labelNum = labelCounter++;
            printf("    cbz x0, and%d_0\n", labelNum);
            compile_ast(ast->children[1]);
            printf("    cbz x0, and%d_0\n", labelNum);
            printf("    mov x0, #1\n");
            printf("    b and%d_end\n", labelNum);
            printf("and%d_0:\n", labelNum); 
            printf("    mov x0, #0\n");
            printf("and%d_end:\n", labelNum);
            return;
        }
        case BIT_AND:
            loadBinaryChildrenReg(ast);
            printf("    and x0, x0, x1\n");
            return;
        case LOG_OR: { 
            compile_ast(ast->children[0]);
            int labelNum = labelCounter++;
            printf("    cbnz x0, or%d_1\n", labelNum);
            compile_ast(ast->children[1]);
            printf("    cbnz x0, or%d_1\n", labelNum);
            printf("    mov x0, #0\n");
            printf("    b or%d_end\n", labelNum);
            printf("or%d_1:\n", labelNum); 
            printf("    mov x0, #1\n");
            printf("or%d_end:\n", labelNum);
            return;
        }
        case COMMA: 
            compile_ast(ast->children[0]);
            compile_ast(ast->children[1]); // second output will overwrite first output in x0
            return;
        case SHIFT_LEFT: 
            loadBinaryChildrenReg(ast);
            printf("    lslv x0, x0, x1");
            return;
        case SHIFT_RIGHT:
            loadBinaryChildrenReg(ast);
            printf("    lsrv x0, x0, x1");
            return;
        case IDENTIFIER:
            printf("    ldr x0, =v_");
            slice_print(ast->identifier);
            printf("\n");
            printf("    ldr x0, [x0]\n");
            return;
        case LITERAL:  
            printf("    ldr x0, =%ld\n", ast->literal);
            return;
        case FUNC_CALL:  
            compile_ast(ast->children[0]); // function address in x0
            printf("    str x0, [SP, #-16]!\n");
            compile_ast(ast->children[1]); // function input in x0
            printf("    ldr x1, [SP], #16\n");
            printf("    blr x1\n"); // call function (input remains in x0)
            return;
        case BLOCK:
            // compile every statement in the block
            for (int i = 0; i < ast->numChildren; i++) {
                compile_ast(ast->children[i]); 
            }
            return;
        default:
            return;
    }
}

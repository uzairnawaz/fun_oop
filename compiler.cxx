
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
    printf("v_it0:\n");
    printf("    .quad 0\n");
    printf("v_it1:\n");
    printf("    .quad 0\n");


    std::unordered_set<std::string> varNames;
    varNames.insert("argc");
    varNames.insert("it0");
    varNames.insert("it1");
    preprocessVars(astRoot, &varNames);

    printf("    .text\n");
    printf("    .global main\n");
}

/*
 Read in all the variables from the program and store them in the data section of the program
*/
void FunCompiler::preprocessVars(ASTNode* ast,
        std::unordered_set<std::string>* varNames) {
    if (ast->type == IDENTIFIER) {
        std::string varName = ast->identifier;
        if (varNames->count(varName) == 0) {
            varNames->insert(varName);
            printf("v_");
            printf("%s", ast->identifier.c_str());
            printf(":\n");
            printf("    .quad 0\n");
        }
    } else {
        for (uint64_t i = 0; i < ast->children.size(); i++) {
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
        if (ast->identifier == "self") {
            return selfType;
        }
        return classNames.at(varTypes.at(ast->identifier));
    case FUNC_CALL:
        // TODO :)
        return nullptr;
    case ACCESS:
        return determineType(ast->children[0])->memberTypes.at(ast->children[1]->identifier);
    case ARRAY_ACCESS:
        return determineType(ast->children[0]);
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
        case ARRAY_ACCESS: {
            // ClassNode* type = determineType(ast->children[0]);
            // uint64_t size = type == 0 ? 8 : type->getSize();
            loadBinaryChildrenReg(ast);
            printf("    ldr x2, =8\n");//%ld\n", size);
            printf("    mul x1, x1, x2\n");
            printf("    add x0, x0, x1\n");
            printf("    ldr x0, [x0]\n");
            break;
        }
        case CLASS: {
            int labelNum = labelCounter++;
            selfType = classNames.at(ast->children[0]->identifier);
            inClassDefinition = true;
            printf("    b class%d_end\n", labelNum);
            printf("class%d:\n", labelNum);
            printf("    stp x29, x30, [SP, #-16]!\n"); // store frame pointer and link register 
            compile_ast(ast->children[2]);
            printf("    ldp x29, x30, [SP], #16\n"); 
            printf("    ret\n");
            printf("class%d_end:\n", labelNum);
            printf("    ldr x0, =class%d\n", labelNum);
            printf("    ldr x1, =v_%s\n", ast->children[0]->identifier.c_str());
            printf("    str x0, [x1]\n");
            inClassDefinition = false;
            break;
        }
        case NEW: {
            std::string type = ast->children[0]->children[0]->identifier;
            ClassNode* classNode = classNames.at(type); 
            if (ast->children[0]->type == FUNC_CALL) { // instantiating a new object
                printf("    ldr x0, =%ld\n", classNode->getSize());
                printf("    bl malloc\n");
                printf("    str x0, [SP, #-16]!\n");
                printf("    ldr x1, =v_%s\n", ast->children[0]->children[0]->identifier.c_str());
                printf("    ldr x1, [x1]\n");
                printf("    str x10, [SP, #-16]!\n");
                printf("    mov x10, x0\n");
                printf("    blr x1\n");
                printf("    ldr x10, [SP], #16\n");
                printf("    ldr x0, [SP], #16\n");
                break;
            } else { //instantiating an array
                compile_ast(ast->children[0]->children[1]);
                if (classNode == 0) { // if this is an int array
                    printf("    ldr x1, =8\n");
                    printf("    bl calloc\n");
                } else {
                    printf("    str x0, [SP, #-16]!\n");
                    printf("    ldr x1, =8\n");//%ld\n", classNode->getSize());
                    printf("    mul x0, x0, x1\n");
                    printf("    bl malloc\n");
                    printf("    ldr x2, [SP], #16\n"); // loading from stack the length of array
                    printf("    str x10, [SP, #-16]!\n"); // store old "self" on the stack
                    printf("    str x0, [SP, #-16]!\n"); // storing pointer to first array element
                    printf("    mov x3, x0\n");
                    
                    int labelNum = labelCounter++;
                    
                    printf("instantiate_array_%d:\n", labelNum);
                    printf("    str x2, [SP, #-16]!\n"); // x2 = loop control var (starts at arr length and goes to 0)
                    printf("    str x3, [SP, #-16]!\n"); // x3 = mem address of cur element in arr
                    printf("    ldr x0, =%ld\n", classNode->getSize());
                    printf("    bl malloc\n");
                    printf("    ldr x3, [SP]\n");
                    printf("    str x0, [x3]\n");
                    printf("    mov x10, x0\n"); // update "self"
                    printf("    ldr x1, =v_%s\n", ast->children[0]->children[0]->identifier.c_str());
                    printf("    ldr x1, [x1]\n"); //load pointer to class definition
                    printf("    blr x1\n"); // instantiate object
                    printf("    ldr x3, [SP], #16\n");
                    printf("    ldr x2, [SP], #16\n");
                    printf("    add x3, x3, #8\n");
                    printf("    sub x2, x2, #1\n");
                    printf("    cbnz x2, instantiate_array_%d\n", labelNum);
                    
                    printf("    ldr x0, [SP], #16\n"); // load back pointer to start of array
                    printf("    ldr x10, [SP], #16\n"); // load back old "self"
                }
                break;
           }
        }
        case ACCESS: {
            ClassNode* type = determineType(ast->children[0]);
            uint64_t idx = type->memberPos.at(ast->children[1]->identifier);

            compile_ast(ast->children[0]);
            printf("    ldr x0, [x0, #%ld]\n", idx);
            break;
        }
        case FUN: {
            int labelNum = labelCounter++; 
            printf("    b func%d_end\n", labelNum);
            printf("func%d:\n", labelNum);
            printf("    stp x29, x30, [SP, #-16]!\n"); // store frame pointer and link register 
            printf("    ldr x3, =v_it0\n");
            printf("    ldr x2, [x3]\n");
            printf("    str x2, [SP, #-16]!\n"); // store the old "it" value on the stack
            printf("    str x0, [x3]\n"); // update the value of "it" with x0 (function input)

            printf("    ldr x3, =v_it1\n");
            printf("    ldr x2, [x3]\n");
            printf("    str x2, [SP, #-16]!\n"); // store the old "it" value on the stack
            printf("    str x1, [x3]\n"); // update the value of "it" with x0 (function input) // this needs to be x1

            compile_ast(ast->children[0]);  

            // default return  
            printf("    ldr x1, [SP], #16\n");
            printf("    ldr x2, =v_it1\n");
            printf("    str x1, [x2]\n");
            printf("    ldp x29, x30, [SP], #16\n");
            printf("    mov x0, #0\n");

            printf("    ldr x1, [SP], #16\n");
            printf("    ldr x2, =v_it0\n");
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
            if (ast->children.size() == 3) { // if this "if" statement had an "else" clause
                compile_ast(ast->children[2]); // compile the else clause 
            }
            printf("if_end%d:\n", labelNum);
            return;
        }
        case PRINT: 
            printf("// print\n");
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
                printf("    ldr x3, =v_it1\n");
                printf("    str x2, [x3]\n");

                printf("    ldr x2, [SP], #16\n");
                printf("    ldr x3, =v_it0\n");
                printf("    str x2, [x3]\n");

                printf("    ldp x29, x30, [SP], #16\n");

                printf("    br x1\n"); // call function, WITHOUT linking
            } else {
                compile_ast(ast->children[0]);
                printf("    ldr x1, [SP], #16\n");
                printf("    ldr x2, =v_it1\n");
                printf("    str x1, [x2]\n");

                printf("    ldr x1, [SP], #16\n");
                printf("    ldr x2, =v_it0\n");
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
            printf("// ASSIGN\n"); 
            if (ast->children[0]->type == DECLARATION || ast->children[0]->type == IDENTIFIER) {
                if (!inClassDefinition) {
                    compile_ast(ast->children[1]);
                    printf("    ldr x1, =v_");
                    if (ast->children[0]->type == DECLARATION) {
                        printf("%s", ast->children[0]->children[1]->identifier.c_str());
                    } else {
                        printf("%s", ast->children[0]->identifier.c_str());
                    }
                    printf("\n");
                    printf("    str x0, [x1]\n");
                } else { // implicitly accessing self 
                    compile_ast(ast->children[1]);
                    uint64_t idx;
                    if (ast->children[0]->type == DECLARATION) {
                        idx = selfType->memberPos.at(ast->children[0]->children[1]->identifier);
                    } else {
                        idx = selfType->memberPos.at(ast->children[0]->identifier);
                    }
                    printf("    str x0, [x10, #%ld]\n", idx);
                }
            } else if (ast->children[0]->type == ACCESS) {
                ClassNode* type = determineType(ast->children[0]->children[0]);
                uint64_t idx = type->memberPos.at(ast->children[0]->children[1]->identifier);

                compile_ast(ast->children[0]->children[0]);
                printf("    str x0, [SP, #-16]!\n");
                compile_ast(ast->children[1]);
                printf("    mov x1, x0\n");
                printf("    ldr x0, [SP], #16\n");
                printf("    str x1, [x0, #%ld]\n", idx); 
            } else if (ast->children[0]->type == ARRAY_ACCESS) {
                compile_ast(ast->children[0]->children[0]);
                printf("    str x0, [SP, #-16]!\n");
                compile_ast(ast->children[0]->children[1]);
                printf("    str x0, [SP, #-16]!\n");
                compile_ast(ast->children[1]);
                printf("    mov x2, x0\n");
                printf("    ldr x1, [SP], #16\n");
                printf("    ldr x0, [SP], #16\n");
                // arr[idx] = val
                // x0 = arr, x1 = idx, x2 = val
                printf("    ldr x4, =8\n");
                printf("    mul x1, x1, x4\n");
                printf("    add x0, x0, x1\n");
                printf("    str x2, [x0]\n");
            } else {
                // BAD! ERROR!!
            }
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
            if (ast->identifier == "self") {
                printf("    mov x0, x10\n");
            } else {
                printf("    ldr x0, =v_%s\n", ast->identifier.c_str());
                printf("    ldr x0, [x0]\n");
            }
           return;
        case LITERAL:  
            printf("    ldr x0, =%ld\n", ast->literal);
            return;
        case FUNC_CALL:  
            if (ast->children[0]->type == ACCESS) {
                ClassNode* type = determineType(ast->children[0]->children[0]);
                uint64_t idx = type->memberPos.at(ast->children[0]->children[1]->identifier);

                compile_ast(ast->children[0]->children[0]);
                printf("    str x10, [SP, #-16]!\n");
                printf("    mov x10, x0\n");
                printf("    ldr x0, [x0, #%ld]\n", idx);
            } else {
                compile_ast(ast->children[0]); // function address in x0
            }
            printf("    str x0, [SP, #-16]!\n");

            if (ast->children.size() >= 3) {
                compile_ast(ast->children[2]);
            }

            printf("    str x0, [SP, #-16]!\n");

            if (ast->children.size() >= 2) {
                compile_ast(ast->children[1]); // input is automatically put in x0
            }
            printf("    ldr x1, [SP], #16\n");
            
            printf("    ldr x2, [SP], #16\n");
            printf("    blr x2\n"); // call function (input remains in x0)
            
            if (ast->children[0]->type == ACCESS) {
                printf("    ldr x10, [SP], #16\n");
            }
            return;
        case BLOCK:
            // compile every statement in the block
            for (uint64_t i = 0; i < ast->children.size(); i++) {
                compile_ast(ast->children[i]); 
            }
            return;
        default:
            return;
    }
}

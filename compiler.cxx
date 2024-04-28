
#include "compiler.h"

std::string output = "";
void print(std::string str) {
    output += str;
}

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

    print("main:\n");
    print("    stp x29, x30, [SP, #-16]!\n");
    print("    ldr x1, =v_argc\n");
    print("    str x0, [x1]\n");
    compile_ast(astRoot);    
    print("    mov x0, #0\n");
    print("    ldp x29, x30, [SP], #16\n");
    print("    ret\n");

    printf("%s", output.c_str());

    ast_free(astRoot);
}

/*
 Generate the data section of the program
*/
void FunCompiler::preprocess() {
    print("    .data\n");
    print("    .extern printf\n");
    print("fmt_int:\n");
    print("    .string \"%ld\\n\"\n");
    print("    .align 7\n");
    print("fmt_char:\n");
    print("    .string \"%c\"\n");
    print("    .align 7\n");
    print("v_argc:\n");
    print("    .quad -1\n");
    print("v_self:\n");
    print("    .quad 0\n");

    for (int i = 0; i < 8; i++) {
        print("v_it" + std::to_string(i) + ":\n");
        print("    .quad 0\n");
    }

    std::unordered_set<std::string> varNames;
    varNames.insert("argc");
    varNames.insert("it0");
    varNames.insert("it1");
    varNames.insert("it2");
    varNames.insert("it3");
    varNames.insert("it4");
    varNames.insert("it5");
    varNames.insert("it6");
    varNames.insert("it7");
    varNames.insert("self");
    
    preprocessVars(astRoot, &varNames);

    print("    .text\n");
    print("    .global main\n");
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
            print("v_" + ast->identifier + ":\n");
            print("    .quad 0\n");
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
    print("    str x0, [SP, #-16]!\n");
    compile_ast(ast->children[1]);
    print("    mov x1, x0\n");
    print("    ldr x0, [SP], #16\n");
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
        } else if (paramTypes.find(ast->identifier) != paramTypes.end() && paramTypes.at(ast->identifier).size() != 0) {
            std::vector<ClassNode*> typeStack = paramTypes.at(ast->identifier); 
            return typeStack[typeStack.size() - 1];
        }
        return classNames.at(varTypes.at(ast->identifier));
    case FUNC_CALL:
        /*
        Sample code:
        a = fun (...) -> ([RETURN_TYPE]) {
            ...
        }

        Internally, we store the type of 'a' as [RETURN_TYPE]
        */
        return determineType(ast->children[0]);
    case ACCESS:
        return determineType(ast->children[0])->getMemberType(ast->children[1]->identifier);
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
            print("    ldr x2, =8\n");//%ld\n", size);
            print("    mul x1, x1, x2\n");
            print("    add x0, x0, x1\n");
            print("    ldr x0, [x0]\n");
            break;
        }
        case CLASS: {
            int labelNum = labelCounter++;
            selfType = classNames.at(ast->children[0]->identifier);
            inClassDefinition = true;
            inPrivateClass = true;
            print("    b class" + std::to_string(labelNum) + "_end\n");
            print("class" + std::to_string(labelNum) + ":\n");
            print("    stp x29, x30, [SP, #-16]!\n"); // store frame pointer and link register 
            print("    ldr x9, =v_self\n");
            print("    ldr x8, [x9]\n");
            print("    str x8, [SP, #-16]!\n");
            print("    str x10, [x9]\n");
            if (selfType->getParent() != 0) {
                // branch to parent
                print("    ldr x0, =v_" + ast->children[1]->identifier + "\n");
                print("    ldr x0, [x0]\n");
                print("    blr x0\n");
            }
            compile_ast(ast->children[2]);
            print("    ldr x9, =v_self\n");
            print("    ldr x8, [SP], #16\n");
            print("    str x8, [x9]\n");
            print("    ldp x29, x30, [SP], #16\n"); 
            print("    ret\n");
            print("class" + std::to_string(labelNum) + "_end:\n");
            print("    ldr x0, =class" + std::to_string(labelNum) + "\n");
            print("    ldr x1, =v_" + ast->children[0]->identifier + "\n");
            print("    str x0, [x1]\n");
            inClassDefinition = false;
            inPrivateClass = false;
            break;
        }
        case NEW: {
            std::string type = ast->children[0]->children[0]->identifier;
            ClassNode* classNode = classNames.at(type); 
            if (ast->children[0]->type == FUNC_CALL) { // instantiating a new object
                print("    ldr x0, =" + std::to_string(classNode->getSize()) + "\n");
                print("    bl fun_malloc\n");
                print("    str x0, [SP, #-16]!\n");
                print("    ldr x1, =v_" + ast->children[0]->children[0]->identifier + "\n");
                print("    ldr x1, [x1]\n");
                print("    str x10, [SP, #-16]!\n");
                print("    mov x10, x0\n");
                print("    blr x1\n");
                print("    ldr x10, [SP], #16\n");
                print("    ldr x0, [SP], #16\n");
                break;
            } else { //instantiating an array
                compile_ast(ast->children[0]->children[1]);
                if (classNode == 0) { // if this is an int array
                    print("    ldr x1, =8\n");
                    print("    mul x0, x0, x1\n");
                    print("    bl fun_malloc\n");
                } else {
                    print("    str x0, [SP, #-16]!\n");
                    print("    ldr x1, =8\n");//%ld\n", classNode->getSize());
                    print("    mul x0, x0, x1\n");
                    print("    bl fun_malloc\n");
                    print("    ldr x2, [SP], #16\n"); // loading from stack the length of array
                    print("    str x10, [SP, #-16]!\n"); // store old "self" on the stack
                    print("    str x0, [SP, #-16]!\n"); // storing pointer to first array element
                    print("    mov x3, x0\n");
                    
                    int labelNum = labelCounter++;
                    
                    print("instantiate_array_" + std::to_string(labelNum) + ":\n");
                    print("    str x2, [SP, #-16]!\n"); // x2 = loop control var (starts at arr length and goes to 0)
                    print("    str x3, [SP, #-16]!\n"); // x3 = mem address of cur element in arr
                    print("    ldr x0, =" + std::to_string(classNode->getSize()) + "\n");
                    print("    bl fun_malloc\n");
                    print("    ldr x3, [SP]\n");
                    print("    str x0, [x3]\n");
                    print("    mov x10, x0\n"); // update "self"
                    print("    ldr x1, =v_" + ast->children[0]->children[0]->identifier + "\n");
                    print("    ldr x1, [x1]\n"); //load pointer to class definition
                    print("    blr x1\n"); // instantiate object
                    print("    ldr x3, [SP], #16\n");
                    print("    ldr x2, [SP], #16\n");
                    print("    add x3, x3, #8\n");
                    print("    sub x2, x2, #1\n");
                    print("    cbnz x2, instantiate_array_" + std::to_string(labelNum) + "\n");
                    
                    print("    ldr x0, [SP], #16\n"); // load back pointer to start of array
                    print("    ldr x10, [SP], #16\n"); // load back old "self"
                }
                break;
           }
        }
        case ACCESS: {
            ClassNode* type = determineType(ast->children[0]);
            uint64_t idx = type->getMemberPos(ast->children[1]->identifier);
            int access_type = type->getMemberAccess(ast->children[1]->identifier);
            if (access_type == 2 || (access_type == 1 && !(selfType == type && inPrivateClass))) {
                printf("Private access detected. Compilation rejected.");
                exit(1);
            }
            compile_ast(ast->children[0]);
            print("    ldr x0, [x0, #" + std::to_string(idx) + "]\n");
            break;
        }
        case FUN: {
            bool oldInClassDef = inClassDefinition;
            inClassDefinition = false;
            int labelNum = labelCounter++; 
            print("    b func" + std::to_string(labelNum) + "_end\n");
            print("func" + std::to_string(labelNum) + ":\n");
            print("    stp x29, x30, [SP, #-16]!\n"); // store frame pointer and link register 

            for (int i = 0; i < 8; i++) {
                print("    ldr x9, =v_it" + std::to_string(i) + " \n");
                print("    ldr x8, [x9]\n");
                print("    str x8, [SP, #-16]!\n"); // store the old "it" value on the stack
                print("    str x" + std::to_string(i) + ", [x9]\n"); // update the value of "it" with x0 (function input)
            }
            print("    ldr x9, =v_self\n");
            print("    ldr x8, [x9]\n");
            print("    str x8, [SP, #-16]!\n");
            print("    str x10, [x9]\n");
            // param types
            for (int i = 2; i < (int)ast->children.size(); i++) {
                paramTypes["it" + std::to_string(i - 2)].push_back(classNames.at(ast->children[i]->identifier));
            } 

            compile_ast(ast->children[0]);  

            for (int i = 2; i < (int)ast->children.size(); i++) {
                paramTypes["it" + std::to_string(i - 2)].pop_back();
            } 

            // default return  
            print("    ldr x1, [SP], #16\n");
            print("    ldr x2, =v_self\n");
            print("    str x1, [x2]\n");
            for (int i = 7; i >= 0; i--) {
                print("    ldr x1, [SP], #16\n");
                print("    ldr x2, =v_it" + std::to_string(i) + "\n");
                print("    str x1, [x2]\n");
            }

            print("    ldp x29, x30, [SP], #16\n");
            print("    mov x0, #0\n");
            print("    ret\n");

            print("func" + std::to_string(labelNum) + "_end:\n");
            print("    ldr x0, =func" + std::to_string(labelNum) + "\n");
            inClassDefinition = oldInClassDef;
            return;
        }
        case WHILE: { 
            compile_ast(ast->children[0]); // evaluate condition 
            int labelNum = labelCounter++;    
            print("    cbz x0, loop_end" + std::to_string(labelNum) + "\n"); // skip if condition == 0
            print("loop" + std::to_string(labelNum) + ":\n");
            compile_ast(ast->children[1]);
            compile_ast(ast->children[0]); // reevaluate condition 
            print("    cbnz x0, loop" + std::to_string(labelNum) + "\n"); // jump to start of loop if condition true
            print("loop_end" + std::to_string(labelNum) + ":\n");
            return;
        }
        case IF: {    
            compile_ast(ast->children[0]); // evaluate condition
            int labelNum = labelCounter++;    
            print("    cbz x0, else" + std::to_string(labelNum) + "\n"); // jump to else if condition failed
            print("if" + std::to_string(labelNum) + ":\n");
            compile_ast(ast->children[1]);
            print("    b if_end" + std::to_string(labelNum) + "\n"); 
            print("else" + std::to_string(labelNum) + ":\n");
            if (ast->children.size() == 3) { // if this "if" statement had an "else" clause
                compile_ast(ast->children[2]); // compile the else clause 
            }
            print("if_end" + std::to_string(labelNum) + ":\n");
            return;
        }
        case FREE:
            compile_ast(ast->children[0]);
            print("    bl fun_free\n");
            return;
        case PRINT: 
            compile_ast(ast->children[0]);
            print("    mov x1, x0\n"); // integer to print goes in x1
            print("    ldr x0, =fmt_int\n"); 
            print("    bl printf\n");
            return;
        case PRINTC: 
            print("// printc\n");
            compile_ast(ast->children[0]);
            print("    mov x1, x0\n"); // char to print goes in x1
            print("    ldr x0, =fmt_char\n"); 
            print("    bl printf\n");
            return;
        case RETURN:
            compile_ast(ast->children[0]);
            
            print("    ldr x2, [SP], #16\n");
            print("    ldr x3, =v_self\n");
            print("    str x2, [x3]\n");
            for (int i = 7; i >= 0; i--) {
                print("    ldr x1, [SP], #16\n");
                print("    ldr x2, =v_it" + std::to_string(i) + "\n");
                print("    str x1, [x2]\n");
            }
            
            print("    ldp x29, x30, [SP], #16\n");
            print("    ret\n");
            return;
        case PLUS:  
            loadBinaryChildrenReg(ast);
            print("    add x0, x0, x1\n");
            return;
        case MINUS: 
            loadBinaryChildrenReg(ast);
            print("    sub x0, x0, x1\n");
            return;
        case MULT:   
            loadBinaryChildrenReg(ast);
            print("    mul x0, x0, x1\n");
            return;
        case DIV:
            loadBinaryChildrenReg(ast);
            print("    udiv x0, x0, x1\n");
            return;
        case MOD: 
            // a % b = a - b * (a / b)
            loadBinaryChildrenReg(ast);
            print("    udiv x2, x0, x1\n");
            print("    msub x3, x2, x1, x0\n");
            
            // if b is 0, a % b = 0
            print("    udiv x4, x1, x1\n");
            print("    mul x0, x3, x4\n");
            return;
        case LESS_EQUAL: { 
            loadBinaryChildrenReg(ast);
            int labelNum = labelCounter++;
            print("    cmp x0, x1\n");
            print("    bls condition" + std::to_string(labelNum) + "\n");
            print("    mov x0, #0\n");
            print("    b condition" + std::to_string(labelNum) + "_end\n");
            print("condition" + std::to_string(labelNum) + ":\n");
            print("    mov x0, #1\n");
            print("condition" + std::to_string(labelNum) + "_end:\n");
            return;
        }
        case GREATER_EQUAL: {  
            loadBinaryChildrenReg(ast);
            int labelNum = labelCounter++;
            print("    cmp x0, x1\n");
            print("    bhs condition" + std::to_string(labelNum) + "\n");
            print("    mov x0, #0\n");
            print("    b condition" + std::to_string(labelNum) + "_end\n");
            print("condition" + std::to_string(labelNum) + ":\n");
            print("    mov x0, #1\n");
            print("condition" + std::to_string(labelNum) + "_end:\n");
            return;
        }
        case LESS: { 
            loadBinaryChildrenReg(ast);
            int labelNum = labelCounter++;
            print("    cmp x0, x1\n");
            print("    blo condition" + std::to_string(labelNum) + "\n");
            print("    mov x0, #0\n");
            print("    b condition" + std::to_string(labelNum) + "_end\n");
            print("condition" + std::to_string(labelNum) + ":\n");
            print("    mov x0, #1\n");
            print("condition" + std::to_string(labelNum) + "_end:\n");
            return;
        }
        case GREATER: { 
            loadBinaryChildrenReg(ast);
            int labelNum = labelCounter++;
            print("    cmp x0, x1\n");
            print("    bhi condition" + std::to_string(labelNum) + "\n");
            print("    mov x0, #0\n");
            print("    b condition" + std::to_string(labelNum) + "_end\n");
            print("condition" + std::to_string(labelNum) + ":\n");
            print("    mov x0, #1\n");
            print("condition" + std::to_string(labelNum) + "_end:\n");
            return;
        }
        case EQUAL: { 
            loadBinaryChildrenReg(ast);
            int labelNum = labelCounter++;
            print("    cmp x0, x1\n");
            print("    beq condition" + std::to_string(labelNum) + "\n");
            print("    mov x0, #0\n");
            print("    b condition" + std::to_string(labelNum) + "_end\n");
            print("condition" + std::to_string(labelNum) + ":\n");
            print("    mov x0, #1\n");
            print("condition" + std::to_string(labelNum) + "_end:\n");
            return;
        }
        case NOT_EQUAL: {
            loadBinaryChildrenReg(ast);
            int labelNum = labelCounter++;
            print("    cmp x0, x1\n");
            print("    bne condition" + std::to_string(labelNum) + "\n");
            print("    mov x0, #0\n");
            print("    b condition" + std::to_string(labelNum) + "_end\n");
            print("condition" + std::to_string(labelNum) + ":\n");
            print("    mov x0, #1\n");
            print("condition" + std::to_string(labelNum) + "_end:\n");
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
            print("// ASSIGN\n"); 
            if (ast->children[0]->type == DECLARATION || ast->children[0]->type == IDENTIFIER) {
                if (!inClassDefinition) {
                    compile_ast(ast->children[1]);
                    print("    ldr x1, =v_");
                    if (ast->children[0]->type == DECLARATION) {
                        int id_loc = ast->children[0]->children.size() == 2 ? 1 : 2;
                        print(ast->children[0]->children[id_loc]->identifier);

                    } else {
                        print(ast->children[0]->identifier);
                    }
                    print("\n");
                    print("    str x0, [x1]\n");
                } else { // implicitly accessing self 
                    compile_ast(ast->children[1]);
                    uint64_t idx;
                    if (ast->children[0]->type == DECLARATION) {
                        int id_loc = ast->children[0]->children.size() == 2 ? 1 : 2;
                        idx = selfType->getMemberPos(ast->children[0]->children[id_loc]->identifier);
                        if (selfType->getMemberAccess(ast->children[0]->children[id_loc]->identifier) == 2) {
                            printf("Private access detected. Compilation rejected.");
                            exit(1);
                        }
                    } else {
                        idx = selfType->getMemberPos(ast->children[0]->identifier);
                        if (selfType->getMemberAccess(ast->children[0]->identifier) == 2) {
                            printf("Private access detected. Compilation rejected.");
                            exit(1);
                        }
                    }
                    print("    ldr x10, =v_self\n");
                    print("    ldr x10, [x10]\n");
                    print("    str x0, [x10, #" + std::to_string(idx) + "]\n");
                }
            } else if (ast->children[0]->type == ACCESS) {
                ClassNode* type = determineType(ast->children[0]->children[0]);
                uint64_t idx = type->getMemberPos(ast->children[0]->children[1]->identifier);
                int access_type = type->getMemberAccess(ast->children[0]->children[1]->identifier);
                if (access_type == 2 || (access_type == 1 && !(selfType == type && inPrivateClass))) {
                    printf("Private access detected. Compilation rejected.");
                    exit(1);
                }
                compile_ast(ast->children[0]->children[0]);
                print("    str x0, [SP, #-16]!\n");
                compile_ast(ast->children[1]);
                print("    mov x1, x0\n");
                print("    ldr x0, [SP], #16\n");
                print("    str x1, [x0, #" + std::to_string(idx) + "]\n"); 
            } else if (ast->children[0]->type == ARRAY_ACCESS) {
                compile_ast(ast->children[0]->children[0]);
                print("    str x0, [SP, #-16]!\n");
                compile_ast(ast->children[0]->children[1]);
                print("    str x0, [SP, #-16]!\n");
                compile_ast(ast->children[1]);
                print("    mov x2, x0\n");
                print("    ldr x1, [SP], #16\n");
                print("    ldr x0, [SP], #16\n");
                // arr[idx] = val
                // x0 = arr, x1 = idx, x2 = val
                print("    ldr x4, =8\n");
                print("    mul x1, x1, x4\n");
                print("    add x0, x0, x1\n");
                print("    str x2, [x0]\n");
            } else {
                // BAD! ERROR!!
            }
            return;
        }
        case LOG_AND: { 
            compile_ast(ast->children[0]);
            int labelNum = labelCounter++;
            print("    cbz x0, and" + std::to_string(labelNum) + "_0\n");
            compile_ast(ast->children[1]);
            print("    cbz x0, and" + std::to_string(labelNum) + "_0\n");
            print("    mov x0, #1\n");
            print("    b and" + std::to_string(labelNum) + "_end\n");
            print("and" + std::to_string(labelNum) + "_0:\n"); 
            print("    mov x0, #0\n");
            print("and" + std::to_string(labelNum) + "_end:\n");
            return;
        }
        case BIT_AND:
            loadBinaryChildrenReg(ast);
            print("    and x0, x0, x1\n");
            return;
        case LOG_OR: { 
            compile_ast(ast->children[0]);
            int labelNum = labelCounter++;
            print("    cbnz x0, or" + std::to_string(labelNum) + "_1\n");
            compile_ast(ast->children[1]);
            print("    cbnz x0, or" + std::to_string(labelNum) + "_1\n");
            print("    mov x0, #0\n");
            print("    b or" + std::to_string(labelNum) + "_end\n");
            print("or" + std::to_string(labelNum) + "_1:\n"); 
            print("    mov x0, #1\n");
            print("or" + std::to_string(labelNum) + "_end:\n");
            return;
        }
        case COMMA: 
            compile_ast(ast->children[0]);
            compile_ast(ast->children[1]); // second output will overwrite first output in x0
            return;
        case SHIFT_LEFT: 
            loadBinaryChildrenReg(ast);
            print("    lslv x0, x0, x1");
            return;
        case SHIFT_RIGHT:
            loadBinaryChildrenReg(ast);
            print("    lsrv x0, x0, x1");
            return;
        case IDENTIFIER:
            print("    ldr x0, =v_" + ast->identifier + "\n");
            print("    ldr x0, [x0]\n");
           return;
        case LITERAL:  
            print("    ldr x0, =" + std::to_string(ast->literal) + "\n");
            return;
        case FUNC_CALL:  
            if (ast->children[0]->type == ACCESS) {
                ClassNode* type = determineType(ast->children[0]->children[0]);
                uint64_t idx = type->getMemberPos(ast->children[0]->children[1]->identifier);
                int access_type = type->getMemberAccess(ast->children[0]->children[1]->identifier);
                if (access_type == 2 || (access_type == 1 && !(selfType == type && inPrivateClass))) {
                    printf("Private access detected. Compilation rejected.");
                    exit(1);
                }
                compile_ast(ast->children[0]->children[0]);
                print("    str x10, [SP, #-16]!\n");
                print("    mov x10, x0\n");
                print("    ldr x0, [x0, #" + std::to_string(idx) +"]\n");
            } else {
                compile_ast(ast->children[0]); // function address in x0
            }
            print("    str x0, [SP, #-16]!\n");

            for (uint64_t i = 0; i < 8; i++) {
                if (ast->children.size() >= (i + 2)) { // i + 1
                    compile_ast(ast->children[i + 1]);
                    print("    str x0, [SP, #-16]!\n");
                } else {
                    break;
                }
            }

            for (int i = 7; i >= 0; i--) {
                if (((int) ast->children.size()) >= (i + 2)) { // i + 1
                    print("    ldr x" + std::to_string(i) + ", [SP], #16\n");
                }
            }
        
            print("    ldr x8, [SP], #16\n");
            print("    blr x8\n"); // call function (input remains in x0)
            
            if (ast->children[0]->type == ACCESS) {
                print("    ldr x10, [SP], #16\n");
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

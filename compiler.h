
#pragma once

#include <stdbool.h>
#include <stdlib.h>
#include <ctype.h>
#include <unordered_set>
#include <string>
#include "ast.h"

#define NUM_REGISTERS 32 

class FunCompiler {
public:
    void compile(const char* program);

private:
    ASTNode* astRoot; // ast for the program
    int labelCounter = 0;
    ClassNode* selfType;
    bool inClassDefinition = false;
    bool inPrivateClass = false;

    void compile_ast(ASTNode* ast);
    void preprocess();
    void loadBinaryChildrenReg(ASTNode* ast);
    ClassNode* determineType(ASTNode* ast);

    std::unordered_map<std::string, std::vector<ClassNode*>> paramTypes; // stores types of it1, it2, ...
    // this is a vector because functions can be declared inside of functions and we need to remember 
    // types after function declarations
    


    void preprocessVars(ASTNode* ast,
        std::unordered_set<std::string>* varNames);
};
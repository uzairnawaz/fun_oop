
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

    void compile_ast(ASTNode* ast);
    void preprocess();
    void loadBinaryChildrenReg(ASTNode* ast);

    void preprocessVars(ASTNode* ast,
        std::unordered_set<std::string>* varNames);
};
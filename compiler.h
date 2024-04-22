
#pragma once

#include <stdbool.h>
#include <stdlib.h>
#include <ctype.h>
#include <unordered_set>
#include "slice.h"
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

    void compile_ast(ASTNode* ast);
    void preprocess();
    void loadBinaryChildrenReg(ASTNode* ast);
    ClassNode* determineType(ASTNode* ast);

    void preprocessVars(ASTNode* ast,
        std::unordered_set<Slice, slice_hash_func, slice_equals_func>* varNames);
};
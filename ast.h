
#pragma once

#include <stdbool.h>
#include <stdlib.h>
#include <unordered_map>
#include "slice.h"
#include "classtree.h"

typedef enum {
    OPEN_PAREN = 0,
    CLOSE_PAREN,
    OPEN_CURLY,
    CLOSE_CURLY,
    CLASS,
    EXTENDS,
    FUN,
    WHILE,
    IF,
    ELSE,
    PRINT,
    RETURN,
    ARROW,
    PLUS,
    MINUS,
    MULT,
    DIV,
    MOD,
    SHIFT_LEFT,
    SHIFT_RIGHT,
    LESS_EQUAL,
    GREATER_EQUAL,
    LESS,
    GREATER,
    EQUAL,
    NOT_EQUAL,
    ASSIGN,
    LOG_AND,
    BIT_AND,
    LOG_OR,
    COMMA,
    ACCESS,
    IDENTIFIER,
    LITERAL,
    DECLARATION,
    FUNC_CALL,
    BLOCK,
} ASTType;

typedef struct Token {
    ASTType type;
    Slice s;
    uint64_t literal;
} Token;

typedef struct Tokens {
    Token* tokens;
    int size;
} Tokens;

typedef struct ASTNode {
    ASTType type;
    struct ASTNode** children;
    int numChildren;
    uint64_t literal;
    Slice identifier;
} ASTNode;

extern std::unordered_map<Slice, Slice, slice_hash_func, slice_equals_func> varTypes;
extern std::unordered_map<Slice, ClassNode*, slice_hash_func, slice_equals_func> classNames;

ASTNode* ast_create(const char* program);
void ast_free(ASTNode* ast);
void ast_fold(ASTNode* ast);

void ast_display(ASTNode* n, int depth);
void tokens_display(Tokens t);

ASTNode* expression(Tokens t, int* curToken);
ASTNode* block(Tokens t, int* curToken);
ASTNode* statement(Tokens t, int* curToken);

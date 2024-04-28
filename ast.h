
#pragma once

#include <stdlib.h>
#include <unordered_map>
#include <string>
#include <vector>
#include "classtree.h"

typedef enum {
    OPEN_PAREN = 0,
    CLOSE_PAREN,
    OPEN_CURLY,
    CLOSE_CURLY,
    OPEN_SQUARE,
    CLOSE_SQUARE,
    CLASS,
    EXTENDS,
    FUN,
    WHILE,
    IF,
    ELSE,
    PRINT,
    PRINTC,
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
    NEW,
    PUBLIC,
    PRIVATE,
    FREE,
    IDENTIFIER,
    LITERAL,
    ARRAY_ACCESS,
    DECLARATION,
    FUNC_CALL,
    BLOCK,
} ASTType;

typedef struct Token {
    ASTType type;
    std::string s;
    uint64_t literal;
} Token;

typedef struct Tokens {
    std::vector<Token> tokens;
    int size;
} Tokens;

typedef struct ASTNode {
    ASTType type;
    std::vector<ASTNode*> children;
    uint64_t literal;
    std::string identifier;
} ASTNode;

extern std::unordered_map<std::string, std::string> varTypes;
extern std::unordered_map<std::string, ClassNode*> classNames;

ASTNode* ast_create(const char* program);
void ast_free(ASTNode* ast);
void ast_fold(ASTNode* ast);

void ast_display(ASTNode* n, int depth);
void tokens_display(Tokens t);

ASTNode* expression(std::vector<Token>* t, uint64_t* curToken);
ASTNode* block(std::vector<Token>* t, uint64_t* curToken);
ASTNode* statement(std::vector<Token>* t, uint64_t* curToken);

extern const char* tokenNames[45];

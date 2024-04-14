#include <stdbool.h>
#include <stdlib.h>
#include "slice.h"

typedef enum {
    OPEN_PAREN = 0,
    CLOSE_PAREN,
    OPEN_CURLY,
    CLOSE_CURLY,
    FUN,
    WHILE,
    IF,
    ELSE,
    PRINT,
    RETURN,
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
    IDENTIFIER,
    LITERAL,
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


ASTNode* ast_create(const char* program);
void ast_free(ASTNode* ast);
void display(ASTNode* n, int depth);
void ast_fold(ASTNode* ast);

ASTNode* expression(Tokens t, int* curToken);
ASTNode* block(Tokens t, int* curToken);
ASTNode* statement(Tokens t, int* curToken);
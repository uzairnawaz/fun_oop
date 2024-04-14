
#include "interpreter.h"

const int STATEMENT_COMPLETED = 0; 
const int STATEMENT_RETURNED = 1;

/*
 Recursively interprets an AST given the root node 
*/
uint64_t interpret_ast(ProgramData* p, ASTNode* ast) {
    uint64_t x;
    switch (ast->type) {
        case OPEN_PAREN:
            return interpret_ast(p, ast->children[0]);
        case FUN: 
             // return pointer to function block node as a uint64_t
            return (uint64_t)(ast->children[0]);
        case WHILE:
            while (interpret_ast(p, ast->children[0])) { // condition
                if (interpret_ast(p, ast->children[1]) == STATEMENT_RETURNED) {
                    // if the statement within the while loop returned, end the while loop
                    return STATEMENT_RETURNED;
                }
            }
            return STATEMENT_COMPLETED;
        case IF:
            if (interpret_ast(p, ast->children[0])) {
                return interpret_ast(p, ast->children[1]);
            } else if (ast->numChildren == 3) { // if there is an "else" block
                return interpret_ast(p, ast->children[2]);
            }
            return STATEMENT_COMPLETED;
        case PRINT:
            printf("%ld\n", interpret_ast(p, ast->children[0]));
            return STATEMENT_COMPLETED;
        case RETURN:
            p->return_val = interpret_ast(p, ast->children[0]);
            return STATEMENT_RETURNED;
        case PLUS:
            return interpret_ast(p, ast->children[0]) + interpret_ast(p, ast->children[1]);
        case MINUS:
            return interpret_ast(p, ast->children[0]) - interpret_ast(p, ast->children[1]);
        case MULT:
            return interpret_ast(p, ast->children[0]) * interpret_ast(p, ast->children[1]);
        case DIV:
            x = interpret_ast(p, ast->children[1]);
            return x == 0 ? 0 : interpret_ast(p, ast->children[0]) / x;
        case MOD:
            x = interpret_ast(p, ast->children[1]);
            return x == 0 ? 0 : interpret_ast(p, ast->children[0]) % x;
        case LESS_EQUAL:
            return interpret_ast(p, ast->children[0]) <= interpret_ast(p, ast->children[1]);
        case GREATER_EQUAL:
            return interpret_ast(p, ast->children[0]) >= interpret_ast(p, ast->children[1]);
        case LESS:
            return interpret_ast(p, ast->children[0]) < interpret_ast(p, ast->children[1]);
        case GREATER:
            return interpret_ast(p, ast->children[0]) > interpret_ast(p, ast->children[1]);
        case EQUAL:
            return interpret_ast(p, ast->children[0]) == interpret_ast(p, ast->children[1]);
        case NOT_EQUAL:
            return interpret_ast(p, ast->children[0]) != interpret_ast(p, ast->children[1]);
        case ASSIGN:
            map_add(p->symbol_table, ast->children[0]->identifier,
                interpret_ast(p, ast->children[1]));
            return STATEMENT_COMPLETED;
        case LOG_AND:
            return interpret_ast(p, ast->children[0]) && interpret_ast(p, ast->children[1]);
        case BIT_AND:
            return interpret_ast(p, ast->children[0]) & interpret_ast(p, ast->children[1]);
        case LOG_OR:
            return interpret_ast(p, ast->children[0]) || interpret_ast(p, ast->children[1]);
        case COMMA:
            return interpret_ast(p, ast->children[0]), interpret_ast(p, ast->children[1]);
        case SHIFT_LEFT:
            return interpret_ast(p, ast->children[0]) << interpret_ast(p, ast->children[1]);
        case SHIFT_RIGHT:
            return interpret_ast(p, ast->children[0]) >> interpret_ast(p, ast->children[1]);
        case IDENTIFIER:
            return map_get(p->symbol_table, ast->identifier);
        case LITERAL:
            return ast->literal;
        case FUNC_CALL:
            x = map_get_str(p->symbol_table, "it", 2); // stores old "it" value
            ASTNode* funcBlock = (ASTNode*)interpret_ast(p, ast->children[0]);
            map_add_str(p->symbol_table, "it", interpret_ast(p, ast->children[1]), 2);
            interpret_ast(p, funcBlock);
            map_add_str(p->symbol_table, "it", x, 2); // restore the "it" value from before call
            return p->return_val;
        case BLOCK:
            for (int i = 0; i < ast->numChildren; i++) {
                p->return_val = 0;

                // statements generally don't return anything
                // if the statement returns this constant, it means that it was a return statement
                if (interpret_ast(p, ast->children[i]) == STATEMENT_RETURNED) {
                    return STATEMENT_RETURNED; 
                }
            }
            p->return_val = 0;
            return STATEMENT_COMPLETED;
        default:
            return 0;
    }
}


/*
 Executes fun code from the given string parameter
*/
void interpreter_run(const char* program) {
    ProgramData* p = malloc(sizeof (struct ProgramData));
    p->ast = ast_create(program);
    p->symbol_table = map_create(); 

    interpret_ast(p, p->ast);    

    map_free(p->symbol_table);
    ast_free(p->ast);
    free(p);
}
#include <stdbool.h>
#include <stdlib.h>
#include <ctype.h>
#include "slice.h"
#include "map.h"
#include "ast.h"

/*
 Struct to store program state
*/
typedef struct ProgramData {
    ASTNode* ast; // ast for the program
    Map* symbol_table; // Maps symbols to values
    uint64_t return_val; // Function return value
} ProgramData;

void interpreter_run(const char* program);
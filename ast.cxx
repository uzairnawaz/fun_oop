
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include "ast.h"

std::unordered_map<Slice, Slice, slice_hash_func, slice_equals_func> varTypes;
std::unordered_map<Slice, ClassNode*, slice_hash_func, slice_equals_func> classNames;


/*
 Provides an error message for debugging and ends program
*/
void fail(int* curToken) {
    printf("FAILED at token %d\n", *curToken);
    exit(1);
}

/*
 Returns the literal at the start of the given string, and increments the pointer to start after
 the literal.
*/
uint64_t consume_literal(const char** current) {
    if (isdigit(**current)) {
        uint64_t v = 0;
        do {
            // Shift the value of out one place value and add the next digit in the string
            v = 10 * v + (**current - '0');
            *current += 1;
        } while (isdigit(**current));
        return v;
    } else {
        return 0; // shouldn't ever happen
    }
}

/*
 Returns the identifier at the start of the given string, and increments the pointer to start after 
 the identifier.
 If no identifier is present, returns a slice with a null char* and length of 0.
*/
Slice consume_identifier(const char** current) {
    if (isalpha(**current)) {
        char const* start = *current;
        do {
            *current += 1;
        } while(isalnum(**current));

        Slice s = {start, (size_t)(*current - start)};
        return s;
    } else {
        Slice s = {NULL, 0};
        return s;
    }
}

/*
 Returns true if token is present at the start of current.
 If the token is a word (starts with a letter), then it ensures that it matches an entire word
 Ex: if current="ifs + ..." and token = "if", 
    match(...) will still return false since the word continues after "if".
*/
bool match(const char* current, const char* token) {
    bool alpha = isalpha(token[0]);
    int i = 0;
    while (token[i] != 0) {
        if (current[i] != token[i]) {
            return false;
        }
        i++;
    }
    return !alpha || !isalnum(current[i]);
}

/*
 Returns a struct containing an array of tokens from a given fun program
 This array is guanranteed to be tightly bound (no extra memory used).
*/
Tokens tokenize(const char* program) {
    static const char* TOKENS[] = {"(", ")", "{", "}", "class", "extends", "fun", "while", "if", "else", "print",
        "return", "->", "+", "-", "*", "/", "%", "<<", ">>", "<=", ">=", "<", ">", "==", "!=", "=", "&&",
        "&", "||", ",", "."};
    static const int TOKEN_LENGTH[] = {1, 1, 1, 1, 5, 7, 3, 5, 2, 4, 5, 6, 2, 1, 1, 1, 1, 1, 2, 2, 2, 2, 1, 
        1, 2, 2, 1, 2, 1, 2, 1, 1};
    static const int NUM_TOKEN_TYPES = 32;
    static const int STARTING_NUM_TOKENS = 100;

    int size = STARTING_NUM_TOKENS;

    Token* tokens = (Token*)malloc(size * (sizeof (Token)));
    int idx = 0;
    while (isspace(*program)) {
        program++;
    }
    while (*program != 0) {
        if (idx == size) {
            size *= 2;
            tokens = (Token*)realloc(tokens, size * (sizeof (Token)));
        }

        bool foundToken = false;
        for (int i = 0; i < NUM_TOKEN_TYPES; i++) {
            if (match(program, TOKENS[i])) { 
                tokens[idx].type = (ASTType)i;
                idx++;
                program += TOKEN_LENGTH[i];
                foundToken = true;
                break;
            }
        }

        if (!foundToken) {
            // is it the include directive?
            if (match(program, "#include")) {

                program += 8;

                // skip the spaces
                while (isspace(*program)) {
                    program++;
                }

                // read in the file string
                const char* fileNameStart = program;
                do {
                    program += 1;
                } while(isalnum(*program) || *program == '.' || *program == '/');

                Slice fileName = {fileNameStart, (size_t)(program - fileNameStart)};
                char str[fileName.len + 1];
                for (size_t i = 0; i < fileName.len; i++) {
                    str[i] = fileName.start[i];
                }
                str[fileName.len] = '\0';

                // open the file
                int fd = open(str,O_RDONLY);
                if (fd < 0) {   
                    perror("open");
                    exit(1);
                }

                // determine its size (std::filesystem::get_size?)
                struct stat file_stats;
                int rc = fstat(fd,&file_stats);
                if (rc != 0) {
                    perror("fstat");
                    exit(1);
                }

                // map the file in my address space
                char const* prog = (char const *)mmap(
                    0,
                    file_stats.st_size,
                    PROT_READ,
                    MAP_PRIVATE,
                    fd,
                    0);
                if (prog == MAP_FAILED) {
                    perror("mmap");
                    exit(1);
                }

                // prog is the new program pointer
                Tokens newProg = tokenize(prog);

                // tokens <- newProg.tokens
                while (idx + newProg.size >= size) {
                    size *= 2;
                    tokens = (Token*)realloc(tokens, size * (sizeof (Token)));
                }

                // concantonate these two
                for (int i = 0; i < newProg.size; i++) {
                    tokens[idx + i] = newProg.tokens[i];
                }

                idx += newProg.size;

                // lets free a bunch of stuff
                free(newProg.tokens);
            } else {
                // it is not a preprocessing directive
                tokens[idx].type = IDENTIFIER;
                tokens[idx].s = consume_identifier(&program);
                if (tokens[idx].s.len == 0) {
                    tokens[idx].type = LITERAL;
                    tokens[idx].literal = consume_literal(&program);
                }
                idx++;
            }
        }

        while (isspace(*program)) {
            program++;
        }
    }

    // allocate the 
    tokens = (Token*)realloc(tokens, idx * (sizeof (Token)));
    Tokens out = {tokens, idx};
    return out;
}

/*
 Creates an AST Tree from a given fun program
*/
ASTNode* ast_create(const char* program) {
    classNames.insert({{"Object", 6}, nullptr});
    classNames.insert({{"int", 3}, nullptr});

    Tokens t = tokenize(program);
    ASTNode* out = (ASTNode*)malloc(sizeof (ASTNode));
    out->type = BLOCK;
    int idx = 0;
    out->numChildren = 10;
    out->children = (ASTNode**)malloc(out->numChildren * sizeof (ASTNode*));
    int curToken = 0;
    while (curToken < t.size) {
        if (idx == out->numChildren) {
            out->numChildren *= 2;
            out->children = (ASTNode**)realloc(out->children, out->numChildren * sizeof (ASTNode*));
        }
        out->children[idx] = statement(t, &curToken);
        idx++;
    }
    out->numChildren = idx;
    out->children = (ASTNode**)realloc(out->children, out->numChildren * sizeof (ASTNode*));
    free(t.tokens);

    ast_fold(out);

    return out;
}

/*
 Sets the child of an ASTNode to another ASTNode.
*/
void setChild(ASTNode* n, ASTNode* child) {
    n->numChildren = 1;
    n->children = (ASTNode**)malloc(sizeof (ASTNode*));
    n->children[0] = child;
}

/*
 Sets the children of an ASTNode to the given ASTNodes.
*/
void setTwoChildren(ASTNode* n, ASTNode* c1, ASTNode* c2) {
    n->numChildren = 2;
    n->children = (ASTNode**)malloc(2 * sizeof (ASTNode*));
    n->children[0] = c1;
    n->children[1] = c2;
}

/*
 Sets the children of an ASTNode to the given ASTNodes.
*/
void setThreeChildren(ASTNode* n, ASTNode* c1, ASTNode* c2, ASTNode* c3) {
    n->numChildren = 3;
    n->children = (ASTNode**)malloc(3 * sizeof (ASTNode*));
    n->children[0] = c1;
    n->children[1] = c2;
    n->children[2] = c3;
}

// () [] . -> ...
ASTNode* e1(Tokens t, int* curToken) {
    ASTNode* out = (ASTNode*)malloc(sizeof (ASTNode));
    out->numChildren = 0;
    switch (t.tokens[*curToken].type) {
        case FUN:
            out->type = FUN;
            *curToken += 1;

            // this if statement should always be true but the program breaks without it
            if (t.tokens[*curToken].type == OPEN_PAREN) {
                *curToken += 1;
                Token parameterToken = t.tokens[*curToken];
                *curToken += 4; // read in type and close_paren and the arrow and the open
                Token returnToken = t.tokens[*curToken];
                *curToken += 2;

                ASTNode* paramNode = (ASTNode*)malloc(sizeof (ASTNode));
                paramNode->type = parameterToken.type;
                paramNode->identifier = parameterToken.s;

                ASTNode* returnNode = (ASTNode*)malloc(sizeof (ASTNode));
                returnNode->type = returnToken.type;
                returnNode->identifier = returnToken.s;

                setThreeChildren(out, paramNode, returnNode, block(t, curToken));
                *curToken -= 1; // will consume a token later
                break;
            }
        case IDENTIFIER:
            out->type = IDENTIFIER;
            out->identifier = t.tokens[*curToken].s;
            break;
        case LITERAL:
            out->type = LITERAL;
            out->literal = t.tokens[*curToken].literal;
            break;
        case OPEN_PAREN:
            *curToken += 1; // consume open paren
            free(out);
            out = expression(t, curToken);
            break;
        /* 
        if we see if, while, else at e1, then it must be a var, not a keyword
        this handles cases like code that looks like the following:
            test = nested - while - loops      
        The code above is valid fun since "while" is treated as a variable with the default value of 0
        */
        case IF:     
            out->type = IDENTIFIER;
            out->identifier.start = "if";
            out->identifier.len = 2;
            break;
        case WHILE:
            out->type = IDENTIFIER;
            out->identifier.start = "while";
            out->identifier.len = 5;
            break;
        case ELSE:
            out->type = IDENTIFIER;
            out->identifier.start = "else";
            out->identifier.len = 4;
            break;
        case RETURN:
            out->type = IDENTIFIER;
            out->identifier.start = "return";
            out->identifier.len = 6;
            break;
        case PRINT:
            out->type = IDENTIFIER;
            out->identifier.start = "print";
            out->identifier.len = 5;
            break;
        default:
            fail(curToken);
            break;
    }
    *curToken += 1;
    return out;
}

// access 
ASTNode* e1_5(Tokens t, int* curToken) {
    ASTNode* left = e1(t, curToken);
    ASTNode* out = left;
    while (*curToken < t.size) {
        switch (t.tokens[*curToken].type) {
            case ACCESS:
                out = (ASTNode*)malloc(sizeof (ASTNode));
                out->type = t.tokens[*curToken].type; 
                break;
            default:
                return out; 
        }
        *curToken += 1;
        setTwoChildren(out, left, e1(t, curToken));
        left = out;
    }
    return out;
}

// f(it)
ASTNode* e2(Tokens t, int* curToken) {
    ASTNode* n = e1_5(t, curToken);
    ASTNode* out = n;
    while (*curToken < t.size && t.tokens[*curToken].type == OPEN_PAREN) {
        out = (ASTNode*)malloc(sizeof (ASTNode));
        out->type = FUNC_CALL;
        *curToken += 1; // consume open paren
        setTwoChildren(out, n, expression(t, curToken));
        *curToken += 1; // consume close paren
        n = out;
    }
    return out;
}

// accessing attribute of returned object
ASTNode* e2_5(Tokens t, int* curToken) {
    ASTNode* left = e2(t, curToken);
    ASTNode* top = left;
    while (*curToken < t.size) {
        switch (t.tokens[*curToken].type) {
            case ACCESS:
                top = (ASTNode*)malloc(sizeof (ASTNode));
                top->type = t.tokens[*curToken].type; 
                break;
            default:
                return top; 
        }
        *curToken += 1;
        setTwoChildren(top, left, e2(t, curToken));
        left = top;
    }
    return top;
}

// * / % (Left)
ASTNode* e3(Tokens t, int* curToken) {
    ASTNode* left = e2_5(t, curToken);
    ASTNode* top = left; 
    while (*curToken < t.size) {
        switch (t.tokens[*curToken].type) {
            case MULT:
            case DIV:
            case MOD:
                top = (ASTNode*)malloc(sizeof (ASTNode));
                top->type = t.tokens[*curToken].type; 
                break;
            default:
                return top; 
        }
        *curToken += 1;
        setTwoChildren(top, left, e2(t, curToken));
        left = top;
    } 
    return top;
}

// (Left) + -
ASTNode* e4(Tokens t, int* curToken) {
    ASTNode* left = e3(t, curToken);
    ASTNode* top = left; 
    while (*curToken < t.size) {
        switch (t.tokens[*curToken].type) {
            case PLUS:
            case MINUS:
                top = (ASTNode*)malloc(sizeof (ASTNode));
                top->type = t.tokens[*curToken].type; 
                break;
            default:
                return top; 
        }
        *curToken += 1;
        setTwoChildren(top, left, e3(t, curToken));
        left = top;
    } 
    return top;
}

// << >>
ASTNode* e5(Tokens t, int* curToken) {
    ASTNode* left = e4(t, curToken);
    ASTNode* top = left; 
    while (*curToken < t.size) {
        switch (t.tokens[*curToken].type) {
            case SHIFT_LEFT:
            case SHIFT_RIGHT:
                top = (ASTNode*)malloc(sizeof (ASTNode));
                top->type = t.tokens[*curToken].type; 
                break;
            default:
                return top; 
        }
        *curToken += 1;
        setTwoChildren(top, left, e4(t, curToken));
        left = top;
    } 
    return top;
}

// < <= > >=
ASTNode* e6(Tokens t, int* curToken) {
    ASTNode* left = e5(t, curToken);
    ASTNode* top = left; 
    while (*curToken < t.size) {
        switch (t.tokens[*curToken].type) {
            case GREATER:
            case LESS:
            case GREATER_EQUAL:
            case LESS_EQUAL:
                top = (ASTNode*)malloc(sizeof (ASTNode));
                top->type = t.tokens[*curToken].type; 
                break;
            default:
                return top; 
        }
        *curToken += 1;
        setTwoChildren(top, left, e5(t, curToken));
        left = top;
    }
    return top;
}

// == !=
ASTNode* e7(Tokens t, int* curToken) {
    ASTNode* left = e6(t, curToken);
    ASTNode* top = left; 
    while (*curToken < t.size) {
        switch (t.tokens[*curToken].type) {
            case EQUAL:
            case NOT_EQUAL:
                top = (ASTNode*)malloc(sizeof (ASTNode));
                top->type = t.tokens[*curToken].type; 
                break;
            default:
                return top; 
        }
        *curToken += 1;
        setTwoChildren(top, left, e6(t, curToken));
        left = top;
    }
    return top;
}

// (left) &
ASTNode* e8(Tokens t, int* curToken) {
    ASTNode* left = e7(t, curToken);
    ASTNode* top = left; 
    while (*curToken < t.size) {
        switch (t.tokens[*curToken].type) {
            case BIT_AND:
                top = (ASTNode*)malloc(sizeof (ASTNode));
                top->type = t.tokens[*curToken].type; 
                break;
            default:
                return top; 
        }
        *curToken += 1;
        setTwoChildren(top, left, e7(t, curToken));
        left = top;
    }
    return top;
}
   
// &&
ASTNode* e9(Tokens t, int* curToken) {
    ASTNode* left = e8(t, curToken);
    ASTNode* top = left; 
    while (*curToken < t.size) {
        switch (t.tokens[*curToken].type) {
            case LOG_AND:
                top = (ASTNode*)malloc(sizeof (ASTNode));
                top->type = t.tokens[*curToken].type; 
                break;
            default:
                return top; 
        }
        *curToken += 1;
        setTwoChildren(top, left, e8(t, curToken));
        left = top;
    }
    return top;
}

// ||
ASTNode* e10(Tokens t, int* curToken) {
    ASTNode* left = e9(t, curToken);
    ASTNode* top = left; 
    while (*curToken < t.size) {
        switch (t.tokens[*curToken].type) {
            case LOG_OR:
                top = (ASTNode*)malloc(sizeof (ASTNode));
                top->type = t.tokens[*curToken].type; 
                break;
            default:
                return top; 
        }
        *curToken += 1;
        setTwoChildren(top, left, e9(t, curToken));
        left = top;
    }
    return top;
}

// ,
ASTNode* e11(Tokens t, int* curToken) {
    ASTNode* left = e10(t, curToken);
    ASTNode* top = left; 
    while (*curToken < t.size) {
        switch (t.tokens[*curToken].type) {
            case COMMA:
                top = (ASTNode*)malloc(sizeof (ASTNode));
                top->type = t.tokens[*curToken].type; 
                break;
            default:
                return top; 
        }
        *curToken += 1;
        setTwoChildren(top, left, e10(t, curToken));
        left = top;
    }
    return top;
}

ASTNode* expression(Tokens t, int* curToken) {
    return e11(t, curToken);
}

ASTNode* statement(Tokens t, int* curToken) {
    ASTNode* out = (ASTNode*)malloc(sizeof (ASTNode));
    switch (t.tokens[*curToken].type) {
        case PRINT:
        case RETURN:
            out->type = t.tokens[*curToken].type;
            *curToken += 1;
            setChild(out, expression(t, curToken));
            break;
        case IF: {
            out->type = IF;
            *curToken += 1;
            ASTNode* cond = expression(t, curToken);
            ASTNode* pass = block(t, curToken);
            if (*curToken < t.size && t.tokens[*curToken].type == ELSE) {
                *curToken += 1;
                setThreeChildren(out, cond, pass, block(t, curToken));
            } else {
                setTwoChildren(out, cond, pass);
            }
            break;
        }
        case WHILE: {
            out->type = WHILE;
            *curToken += 1;
            ASTNode* condition = expression(t, curToken);
            setTwoChildren(out, condition, block(t, curToken));
            break;
        }
        case IDENTIFIER: {
            ASTNode* left = (ASTNode*)malloc(sizeof (ASTNode));
            if (t.tokens[*curToken + 1].type == IDENTIFIER) {
                // if the form is [TYPE] var_name

                varTypes.insert({t.tokens[*curToken + 1].s, t.tokens[*curToken].s});
                left->type = DECLARATION;
                ASTNode* type = (ASTNode*)malloc(sizeof (ASTNode));
                type->type = IDENTIFIER;
                type->numChildren = 0;
                type->identifier = t.tokens[*curToken].s;
                
                ASTNode* varName = (ASTNode*)malloc(sizeof (ASTNode));
                varName->type = IDENTIFIER;
                varName->numChildren = 0;
                varName->identifier = t.tokens[*curToken + 1].s;

                setTwoChildren(left, type, varName);
                *curToken += 2;
            } else {
                // the form is var_name = value
                // implicit type of int
                varTypes.insert({t.tokens[*curToken].s, {"int", 3}});
                left->type = IDENTIFIER;
                left->numChildren = 0;
                left->identifier = t.tokens[*curToken].s;
                *curToken += 1;
            }
            if (t.tokens[*curToken].type == ASSIGN) {
                out->type = ASSIGN;
                *curToken += 1; // skip past =
                ASTNode* right = expression(t, curToken);
                setTwoChildren(out, left, right);
            } else {
                free(out);
                out = left;
            }
            break;
        }
        case FUN: { // allow creation of a variable named fun
            out->type = ASSIGN;
            ASTNode* l = (ASTNode*)malloc(sizeof (ASTNode));
            l->type = IDENTIFIER;
            l->identifier.start = "fun";
            l->identifier.len = 3;
            l->numChildren = 0;
            *curToken += 2; // skip past identifier and =
            ASTNode* r = expression(t, curToken);
            setTwoChildren(out, l, r);
            break;
        }
        case CLASS: {
            out->type = CLASS;
            ASTNode* name = (ASTNode*)malloc(sizeof (ASTNode));
            name->type = IDENTIFIER;
            name->identifier = t.tokens[*curToken + 1].s;
            name->numChildren = 0;
            ASTNode* parent = (ASTNode*)malloc(sizeof (ASTNode));
            parent->type = IDENTIFIER;
            parent->numChildren = 0;
            if (t.tokens[*curToken + 2].type == EXTENDS) {
                parent->identifier = t.tokens[*curToken + 3].s;
                *curToken += 4;
            } else {
                parent->identifier = {"Object", 6};
                *curToken += 2;
            }
            setThreeChildren(out, name, parent, block(t, curToken));

            classNames.insert({name->identifier, new ClassNode(out)});
            break;
        }
        default:
            free(out);
            printf("!!! %d\n", t.tokens[*curToken].type);
            fail(curToken);
            return NULL;
    }
    return out;
}

ASTNode* block(Tokens t, int* curToken) {
    ASTNode* out = (ASTNode*)malloc(sizeof (ASTNode));
    out->type = BLOCK;
    
    int idx = 0;
    if (t.tokens[*curToken].type == OPEN_CURLY) {
        out->numChildren = 10;
        out->children = (ASTNode**)malloc(out->numChildren * sizeof (ASTNode*));
        *curToken += 1;
        while (t.tokens[*curToken].type != CLOSE_CURLY) {
            if (idx == out->numChildren) {
                out->numChildren *= 2;
                out->children = (ASTNode**)realloc(out->children, out->numChildren * sizeof (ASTNode*));
            }

            out->children[idx] = statement(t, curToken);
            idx++;
        }
        out->numChildren = idx;
        out->children = (ASTNode**)realloc(out->children, out->numChildren * sizeof (ASTNode*));
        *curToken += 1;
    } else {
        setChild(out, statement(t, curToken));
    }
    return out;
}

void ast_fold(ASTNode* ast) {
    for (int i = 0; i < ast->numChildren; i++) {
        ast_fold(ast->children[i]);
    }

    if (ast->numChildren == 2 && ast->children[0]->type == LITERAL && 
        ast->children[1]->type == LITERAL) {

        switch (ast->type) {
            case PLUS:  
                ast->literal = ast->children[0]->literal + ast->children[1]->literal;
                break;
            case MINUS: 
                ast->literal = ast->children[0]->literal - ast->children[1]->literal;
                break;
            case MULT:   
                ast->literal = ast->children[0]->literal * ast->children[1]->literal;
                break;
            case DIV:
                ast->literal = ast->children[1]->literal == 0 ?
                                0 :
                                ast->children[0]->literal / ast->children[1]->literal;
                break;
            case MOD: 
                ast->literal = ast->children[1]->literal == 0 ?
                                0 :
                                ast->children[0]->literal % ast->children[1]->literal;
                break;
            case LESS_EQUAL:  
                ast->literal = ast->children[0]->literal <= ast->children[1]->literal;
                break;
            case GREATER_EQUAL:   
                ast->literal = ast->children[0]->literal >= ast->children[1]->literal;
                break;
            case LESS: 
                ast->literal = ast->children[0]->literal < ast->children[1]->literal;
                break;
            case GREATER: 
                ast->literal = ast->children[0]->literal > ast->children[1]->literal;
                break;
            case EQUAL: 
                ast->literal = ast->children[0]->literal == ast->children[1]->literal;
                break;
            case NOT_EQUAL: 
                ast->literal = ast->children[0]->literal != ast->children[1]->literal;
                break;
            case LOG_AND: 
                ast->literal = ast->children[0]->literal && ast->children[1]->literal;
                break;
            case BIT_AND:
                ast->literal = ast->children[0]->literal & ast->children[1]->literal;
                break;
            case LOG_OR: 
                ast->literal = ast->children[0]->literal || ast->children[1]->literal;
                break;
            case COMMA: 
                ast->literal = ast->children[1]->literal;
                break;
            case SHIFT_LEFT: 
                ast->literal = ast->children[0]->literal << ast->children[1]->literal;
                break;
            case SHIFT_RIGHT:
                ast->literal = ast->children[0]->literal >> ast->children[1]->literal;
                break;
            default: // shouldn't be possible
                return;
        }
        ast->type = LITERAL;
        for (int i = 0; i < ast->numChildren; i++) {
            free(ast->children[i]);
        }
        ast->numChildren = 0;
        free(ast->children);
    }
    
}


const char* tokenNames[37] = {
    "OPEN_PAREN",
    "CLOSE_PAREN",
    "OPEN_CURLY",
    "CLOSE_CURLY",
    "CLASS",
    "EXTENDS",
    "FUN",
    "WHILE",
    "IF",
    "ELSE",
    "PRINT",
    "RETURN",
    "ARROW",
    "PLUS",
    "MINUS",
    "MULT",
    "DIV",
    "MOD",
    "SHIFT_LEFT",
    "SHIFT_RIGHT",
    "LESS_EQUAL",
    "GREATER_EQUAL",
    "LESS",
    "GREATER",
    "EQUAL",
    "NOT_EQUAL",
    "ASSIGN",
    "LOG_AND",
    "BIT_AND",
    "LOG_OR",
    "COMMA",
    "ACCESS",
    "IDENTIFIER",
    "LITERAL",
    "DECLARATION",
    "FUNC_CALL",
    "BLOCK"
};

/*
 Provides a text representation of the AST for debugging purposes
 Depth should be 0 on the initial call.
*/
void ast_display(ASTNode* n, int depth) {
    for (int i = 0; i < depth; i++) {
        printf(">");
    }
    printf(" %s ", tokenNames[n->type]);
    if (n->type == IDENTIFIER) {
        for (size_t i = 0; i < n->identifier.len; i++) {
            printf("%c", n->identifier.start[i]);
        }
    } else if (n->type == LITERAL) {
        printf("%lu ", n->literal);
    }
    printf("\n");

    for (int i = 0; i < n->numChildren; i++) {
        ast_display(n->children[i], depth + 1);
    }
}

void tokens_display(Tokens t) {
    for (int i = 0; i < t.size; i++) {
        printf("%d: %s\n", i, tokenNames[t.tokens[i].type]);
    }
}

/*
 Frees the AST from memory.
*/
void ast_free(ASTNode* ast) {
    for (int i = 0; i < ast->numChildren; i++) {
        ast_free(ast->children[i]);
    }
    if (ast->numChildren > 0) {
        free(ast->children);
    }
    free(ast);
}
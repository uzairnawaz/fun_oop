
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include "ast.h"


std::unordered_map<std::string, std::string> varTypes;
std::unordered_map<std::string, ClassNode*> classNames;


/*
 Provides an error message for debugging and ends program
*/
void fail(uint64_t* curToken) {
    printf("FAILED at token %lu\n", *curToken);
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
std::string consume_identifier(const char** current) {
    if (isalpha(**current)) {
        char const* start = *current;
        do {
            *current += 1;
        } while(isalnum(**current));

        std::string slice(start, *current - start);
        return slice;
    } else {
        return "";
    }
}


/*
 Returns true if token is present at the start of current.
 If the token is a word (starts with a letter), then it ensures that it matches an entire word
 Ex: if current="ifs + ..." and token = "if", 
    match(...) will still return false since the word continues after "if".
*/
bool match(const char* current, std::string token) {
    bool alpha = isalpha(token[0]);
    uint64_t i = 0;
    while (i < token.length()) {
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

std::vector<Token>* tokenize(const char* program) {
    static const int NUM_TOKEN_TYPES = 37;
    static const std::string TOKENS[NUM_TOKEN_TYPES] = {"(", ")", "{", "}", "[", "]", "class", "extends", "fun", "while", "if", "else", "print",
        "return", "->", "+", "-", "*", "/", "%", "<<", ">>", "<=", ">=", "<", ">", "==", "!=", "=", "&&",
        "&", "||", ",", ".", "new", "public", "private"};
    
    std::vector<Token> *tokens = new std::vector<Token>;
    int idx = 0;
    while (isspace(*program)) {
        program++;
    }
               
                
    while (*program != 0) {
        //Pending testing
        //array initializing syntax
        if (match(program, TOKENS[OPEN_SQUARE]) && match(program + 1, TOKENS[CLOSE_SQUARE])) {
            program += 2;
            while (isspace(*program)) {
                program++;
            }
        }

            
        
        bool foundToken = false;
        for (int i = 0; i < NUM_TOKEN_TYPES; i++) {

            if (match(program, TOKENS[i])) {
    
                tokens->push_back({(ASTType)i, "", 0});
                idx++;
                program += TOKENS[i].length();
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

                std::string fileName(fileNameStart, (size_t)(program - fileNameStart));

                // open the file
                int fd = open(fileName.c_str(),O_RDONLY);
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
                std::vector<Token> *newProg = tokenize(prog);

                for (uint64_t i = 0; i < (*newProg).size(); i++) {
                    (*tokens).push_back((*newProg).at(i));
                }

                idx += (*newProg).size();
                
                //free issue
                // lets free a bunch of stuff
                delete newProg;
                //free(newProg.tokens;
            } else {
                // it is not a preprocessing directive
                (*tokens).push_back({IDENTIFIER, consume_identifier(&program), 0});

                if ((*tokens)[idx].s.length() == 0) {
                    (*tokens).pop_back();
                    (*tokens).push_back({LITERAL, "", consume_literal(&program)});
                }
                idx++;
            }
        }
        while (isspace(*program)) {
            program++;
        }
    }

    return tokens;
}

/*
 Creates an AST Tree from a given fun program
*/
ASTNode* ast_create(const char* program) {
    classNames.insert({"Object", nullptr});
    classNames.insert({"int", nullptr});


    std::vector<Token>* t = tokenize(program);
    ASTNode* out = new ASTNode;
    out->type = BLOCK;
    //int idx = 0;
    uint64_t curToken = 0;
    while (curToken < t->size()) {
        ASTNode* s = statement(t, &curToken);
        out->children.push_back(s);
    }
    //update freeing
    delete t;

    ast_fold(out);

    return out;
}

/*
 Sets the child of an ASTNode to another ASTNode.
*/
void setChild(ASTNode* n, ASTNode* child) {
    n->children.push_back(child);
}

/*
 Sets the children of an ASTNode to the given ASTNodes.
*/
void setTwoChildren(ASTNode* n, ASTNode* c1, ASTNode* c2) {
    n->children.push_back(c1);
    n->children.push_back(c2);
}

/*
 Sets the children of an ASTNode to the given ASTNodes.
*/
void setThreeChildren(ASTNode* n, ASTNode* c1, ASTNode* c2, ASTNode* c3) {
    n->children.push_back(c1);
    n->children.push_back(c2);
    n->children.push_back(c3);
}

// () [] . -> ...
ASTNode* e1(std::vector<Token>* t, uint64_t* curToken) {
    ASTNode* out = new ASTNode;
    switch ((*t)[*curToken].type) {
        case FUN:
            out->type = FUN;
            *curToken += 1;
            if ((*t)[*curToken].type == OPEN_PAREN) {
                *curToken += 1;
                Token parameterToken = (*t)[*curToken];
                *curToken += 4; // read in type and close_paren and the arrow and the open
                Token returnToken = (*t)[*curToken];
                *curToken += 2;
              
                ASTNode* paramNode = new ASTNode;
                paramNode->type = parameterToken.type;
                paramNode->identifier = parameterToken.s;
              
                ASTNode* returnNode = new ASTNode;
                returnNode->type = returnToken.type;
                returnNode->identifier = returnToken.s;
              
                setThreeChildren(out, block(t, curToken), paramNode, returnNode);
                *curToken -= 1; // will consume a token later
                break;
            } else {
                setChild(out, block(t, curToken));
                *curToken -= 1; // will consume a token later
                break;
            }
        case IDENTIFIER:
            out->type = IDENTIFIER;
            out->identifier = (*t)[*curToken].s;
            break;
        case LITERAL:
            out->type = LITERAL;
            out->literal = (*t)[*curToken].literal;
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
            out->identifier = "if";
            break;
        case WHILE:
            out->type = IDENTIFIER;
            out->identifier = "while";
            break;
        case ELSE:
            out->type = IDENTIFIER;
            out->identifier = "else";
            break;
        case RETURN:
            out->type = IDENTIFIER;
            out->identifier = "return";
            break;
        case PRINT:
            out->type = IDENTIFIER;
            out->identifier = "print";
            break;
        default:
            fail(curToken);
            break;
    }
    *curToken += 1;
    return out;
}

// f(it), arr[idx], .
ASTNode* e2(std::vector<Token>* t, uint64_t* curToken) {
    ASTNode* n = e1(t, curToken);
    ASTNode* out = n;
    while (*curToken < t->size()) {
        out = new ASTNode;
        switch ((*t)[*curToken].type) {
            case OPEN_PAREN:
                out->type = FUNC_CALL;
                *curToken += 1; // consume open paren
                setTwoChildren(out, n, expression(t, curToken));
                *curToken += 1; // consume close paren
                break;
            case OPEN_SQUARE: 
                out->type = ARRAY_ACCESS;
                *curToken += 1; // consume open bracket 
                setTwoChildren(out, n, expression(t, curToken));
                *curToken += 1; // consume close bracket 
                break;
            case ACCESS:
                out->type = ACCESS;
                *curToken += 1;
                setTwoChildren(out, n, e1(t, curToken));
                break;
            default:
                return n;
        }
        n = out;
    }
    return out;
}

ASTNode* e2_5(std::vector<Token>* t, uint64_t* curToken) {
    if ((*t)[*curToken].type == NEW) {
        ASTNode* out = new ASTNode;
        out->type = NEW;
        *curToken += 1;
        setChild(out, e2(t, curToken));
        return out;
    }
    return e2(t, curToken);
}

// * / % (Left)
ASTNode* e3(std::vector<Token>* t, uint64_t* curToken) {
    ASTNode* left = e2_5(t, curToken);
    ASTNode* top = left; 
    while (*curToken < (*t).size()) {
        switch ((*t)[*curToken].type) {
            case MULT:
            case DIV:
            case MOD:
                top = new ASTNode;
                top->type = (*t)[*curToken].type; 
                break;
            default:
                return top; 
        }
        *curToken += 1;
        setTwoChildren(top, left, e2_5(t, curToken));
        left = top;
    } 
    return top;
}

// (Left) + -
ASTNode* e4(std::vector<Token>* t, uint64_t* curToken) {
    ASTNode* left = e3(t, curToken);
    ASTNode* top = left; 
    while (*curToken < (*t).size()) {
        switch ((*t)[*curToken].type) {
            case PLUS:
            case MINUS:
                top = new ASTNode;
                top->type = (*t)[*curToken].type; 
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
ASTNode* e5(std::vector<Token>* t, uint64_t* curToken) {
    ASTNode* left = e4(t, curToken);
    ASTNode* top = left; 
    while (*curToken < (*t).size()) {
        switch ((*t)[*curToken].type) {
            case SHIFT_LEFT:
            case SHIFT_RIGHT:
                top = new ASTNode;
                top->type = (*t)[*curToken].type; 
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
ASTNode* e6(std::vector<Token>* t, uint64_t* curToken) {
    ASTNode* left = e5(t, curToken);
    ASTNode* top = left; 
    while (*curToken < (*t).size()) {
        switch ((*t)[*curToken].type) {
            case GREATER:
            case LESS:
            case GREATER_EQUAL:
            case LESS_EQUAL:
                top = new ASTNode;
                top->type = (*t)[*curToken].type; 
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
ASTNode* e7(std::vector<Token>* t, uint64_t* curToken) {
    ASTNode* left = e6(t, curToken);
    ASTNode* top = left; 
    while (*curToken < (*t).size()) {
        switch ((*t)[*curToken].type) {
            case EQUAL:
            case NOT_EQUAL:
                top = new ASTNode;
                top->type = (*t)[*curToken].type; 
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
ASTNode* e8(std::vector<Token>* t, uint64_t* curToken) {
    ASTNode* left = e7(t, curToken);
    ASTNode* top = left; 
    while (*curToken < (*t).size()) {
        switch ((*t)[*curToken].type) {
            case BIT_AND:
                top = new ASTNode;
                top->type = (*t)[*curToken].type; 
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
ASTNode* e9(std::vector<Token>* t, uint64_t* curToken) {
    ASTNode* left = e8(t, curToken);
    ASTNode* top = left; 
    while (*curToken < (*t).size()) {
        switch ((*t)[*curToken].type) {
            case LOG_AND:
                top = new ASTNode;
                top->type = (*t)[*curToken].type; 
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
ASTNode* e10(std::vector<Token>* t, uint64_t* curToken) {
    ASTNode* left = e9(t, curToken);
    ASTNode* top = left; 
    while (*curToken < (*t).size()) {
        switch ((*t)[*curToken].type) {
            case LOG_OR:
                top = new ASTNode;
                top->type = (*t)[*curToken].type; 
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
ASTNode* e11(std::vector<Token>* t, uint64_t* curToken) {
    ASTNode* left = e10(t, curToken);
    ASTNode* top = left; 
    while (*curToken < (*t).size()) {
        switch ((*t)[*curToken].type) {
            case COMMA:
                top = new ASTNode;
                top->type = (*t)[*curToken].type; 
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

ASTNode* expression(std::vector<Token>* t, uint64_t* curToken) {
    return e11(t, curToken);
}

ASTNode* statement(std::vector<Token>* t, uint64_t* curToken) {
    ASTNode* out = new ASTNode;
    switch ((*t)[*curToken].type) {
        case PRINT:
        case RETURN:
            out->type = (*t)[*curToken].type;
            *curToken += 1;
            setChild(out, expression(t, curToken));
            break;
        case IF: {
            out->type = IF;
            *curToken += 1;
            ASTNode* cond = expression(t, curToken);
            ASTNode* pass = block(t, curToken);
            if (*curToken < t->size() && (*t)[*curToken].type == ELSE) {
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
        case PRIVATE: {
            ASTNode* left = new ASTNode;
            bool is_private_object = (*t)[*curToken + 1].type == IDENTIFIER && (*t)[*curToken + 2].type == IDENTIFIER;
            if (is_private_object) {
                // if the form is [ACCESS] [TYPE] var_name
                varTypes.insert({(*t)[*curToken + 2].s, (*t)[*curToken + 1].s});
                left->type = DECLARATION;

                ASTNode* access = new ASTNode;
                access->type = PRIVATE;
                access->identifier = (*t)[*curToken].s;

                ASTNode* type = new ASTNode;
                type->type = IDENTIFIER;
                type->identifier = (*t)[*curToken + 1].s;
                
                ASTNode* varName = new ASTNode;
                varName->type = IDENTIFIER;
                varName->identifier = (*t)[*curToken + 2].s;

                setThreeChildren(left, type, access, varName);
                *curToken += 3;
            } else {
                // the form is var_name = value
                // implicit type of int
                
                left->type = DECLARATION;

                ASTNode* access = new ASTNode;
                access->type = PRIVATE;
                access->identifier = (*t)[*curToken].s;

                ASTNode* type = new ASTNode;
                type->type = IDENTIFIER;
                type->identifier = "int";

                ASTNode* varName = new ASTNode;
                varName->type = IDENTIFIER;
                varName->identifier = (*t)[*curToken + 1].s;
                
                *curToken += 1;
                varTypes.insert({(*t)[*curToken].s, "int"});
                //ASTNode* func_or_int = e2(t, curToken); // handle accesses (ex: abc.def = 4)
                setThreeChildren(left, type, access, varName);
            }
            if ((*t)[*curToken].type == ASSIGN) {
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
        case PUBLIC: {
            *curToken += 1;
        }
        case IDENTIFIER: {
            ASTNode* left = new ASTNode;
            if ((*t)[*curToken + 1].type == IDENTIFIER) {
                // if the form is [TYPE] var_name

                varTypes.insert({(*t)[*curToken + 1].s, (*t)[*curToken].s});
                left->type = DECLARATION;
                ASTNode* type = new ASTNode;
                type->type = IDENTIFIER;
                type->identifier = (*t)[*curToken].s;
                
                ASTNode* varName = new ASTNode;
                varName->type = IDENTIFIER;
                varName->identifier = (*t)[*curToken + 1].s;

                setTwoChildren(left, type, varName);
                *curToken += 2;
            } else {
                // the form is var_name = value
                // implicit type of int
                varTypes.insert({(*t)[*curToken].s, "int"});
                left = e2(t, curToken); // handle accesses (ex: abc.def = 4)
            }
            if ((*t)[*curToken].type == ASSIGN) {
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
            ASTNode* l = new ASTNode;
            l->type = IDENTIFIER;
            l->identifier = "fun";
            *curToken += 2; // skip past identifier and =
            ASTNode* r = expression(t, curToken);
            setTwoChildren(out, l, r);
            break;
        }
        case CLASS: {
            out->type = CLASS;
            ASTNode* name = new ASTNode;
            name->type = IDENTIFIER;
            name->identifier = (*t)[*curToken + 1].s;
            ASTNode* parent = new ASTNode;
            parent->type = IDENTIFIER;
            if ((*t)[*curToken + 2].type == EXTENDS) {
                parent->identifier = (*t)[*curToken + 3].s;
                *curToken += 4;
            } else {
                parent->identifier = "Object";
                *curToken += 2;
            }
            setThreeChildren(out, name, parent, block(t, curToken));
            classNames.insert({name->identifier, new ClassNode(out)});
            break;
        }
        default:
            delete out;
            printf("!!! %d\n", (*t)[*curToken].type);
            fail(curToken);
            return NULL;
    }
    return out;
}

ASTNode* block(std::vector<Token>* t, uint64_t* curToken) {
    ASTNode* out = new ASTNode;
    out->type = BLOCK;
    
    int idx = 0;
    if ((*t)[*curToken].type == OPEN_CURLY) {
        *curToken += 1;
        while ((*t)[*curToken].type != CLOSE_CURLY) {
            out->children.push_back(statement(t, curToken));
            idx++;
        }
        *curToken += 1;
    } else {
        setChild(out, statement(t, curToken));
    }
    return out;
}

void ast_fold(ASTNode* ast) {
    for (uint64_t i = 0; i < ast->children.size(); i++) {
        ast_fold(ast->children[i]);
    }

    if (ast->children.size() == 2 && ast->children[0]->type == LITERAL && 
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
        for (uint64_t i = 0; i < ast->children.size(); i++) {
            delete ast->children[i];
        }
        ast->children.clear();
    }
    
}


const char* tokenNames[41] = {
    "OPEN_PAREN",
    "CLOSE_PAREN",
    "OPEN_CURLY",
    "CLOSE_CURLY",
    "OPEN_SQUARE",
    "CLOSE_SQUARE",
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
    "NEW",
    "IDENTIFIER",
    "LITERAL",
    "ARRAY_ACCESS",
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
        printf("%s", n->identifier.c_str());
    } else if (n->type == LITERAL) {
        printf("%lu ", n->literal);
    }
    printf("\n");

    for (uint64_t i = 0; i < n->children.size(); i++) {
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
    for (uint64_t i = 0; i < ast->children.size(); i++) {
        ast_free(ast->children[i]);
    }
    // if (ast->children.size() > 0) {
    //     for (std::vector<ASTNode *>::iterator i = ast->children.begin(); i != ast->children.end(); ++i) {
    //         delete *i;
    //     }
    // }
    delete ast;
}

#include "classtree.h"
#include "ast.h"

ClassNode::ClassNode(ASTNode* classASTNode) {
    astNode = classASTNode;
    if (classASTNode == nullptr) {
        return;
    }
    parent = classNames.at(classASTNode->children[1]->identifier);

    ASTNode* block = classASTNode->children[2];
    for (uint64_t statementNum = 0; statementNum < block->children.size(); statementNum++) {
        ASTNode* curStatement = block->children[statementNum];

        /*
            Three cases for AST Node
            For integer types:
                Assign
                /    \
              name   value
            
            For objects that need to be instantiated:
                Declaration
                /        \
               type     name
            
            For objects that don't need to be instantiated

                    Assign
                    /    \
            Declaration  value
               /   \
            type   name
            
        */

        if (curStatement->type == DECLARATION) {
            memberTypes.insert({curStatement->children[1]->identifier, 
                                classNames.at(curStatement->children[0]->identifier)});
            memberPos.insert({curStatement->children[1]->identifier, statementNum});
            memberIsFunc.insert({curStatement->children[1]->identifier, false});
        } else if (curStatement->type == ASSIGN) { 
            if (curStatement->children[0]->type == DECLARATION) {
                memberTypes.insert({curStatement->children[0]->children[1]->identifier, classNames.at(curStatement->children[0]->children[0]->identifier)});
                memberPos.insert({curStatement->children[0]->children[1]->identifier, statementNum});
                memberIsFunc.insert({curStatement->children[0]->children[1]->identifier, false});
            } else {
                memberIsFunc.insert({curStatement->children[0]->identifier, curStatement->children[1]->type == FUN});
                memberTypes.insert({curStatement->children[0]->identifier, 
                    classNames.at("int")});
                memberPos.insert({curStatement->children[0]->identifier, statementNum});
            }
        }
    }
}

size_t ClassNode::getSize() {
    return memberPos.size();
}

ClassNode* ClassNode::getParent() {
    return parent;
}


#include "classtree.h"
#include "ast.h"

ClassNode::ClassNode(ASTNode* classASTNode) {
    astNode = classASTNode;
    if (classASTNode == nullptr) {
        return;
    }
    parent = classNames.at(classASTNode->children[1]->identifier);

    ASTNode* block = classASTNode->children[2];
    for (int statementNum = 0; statementNum < block->numChildren; statementNum++) {
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
            if (slice_equals(curStatement->children[0]->identifier, classASTNode->children[0]->identifier)) {
                // if we have an instance variable of the same type of this class
                // ex: Node class contains a Node instance variable to store a ref to next node
                memberTypes.insert({curStatement->children[1]->identifier, this});
            } else {
                memberTypes.insert({curStatement->children[1]->identifier, 
                    classNames.at(curStatement->children[0]->identifier)});
            }
           memberPos.insert({curStatement->children[1]->identifier, 8 * statementNum});
            memberIsFunc.insert({curStatement->children[1]->identifier, false});
        } else if (curStatement->type == ASSIGN) { 
            if (curStatement->children[0]->type == DECLARATION) {
                memberTypes.insert({curStatement->children[0]->children[1]->identifier,
                    classNames.at(curStatement->children[0]->children[0]->identifier)});
                memberPos.insert({curStatement->children[0]->children[1]->identifier, 8 * statementNum});
                memberIsFunc.insert({curStatement->children[0]->children[1]->identifier, false});
            } else {
                memberIsFunc.insert({curStatement->children[0]->identifier, curStatement->children[1]->type == FUN});
                memberTypes.insert({curStatement->children[0]->identifier, 
                    classNames.at({"int", 3})});
                memberPos.insert({curStatement->children[0]->identifier, 8 * statementNum});
            }
        }
    }
}

size_t ClassNode::getSize() {
    return memberPos.size() * sizeof (uint64_t);
}

ClassNode* ClassNode::getParent() {
    return parent;
}

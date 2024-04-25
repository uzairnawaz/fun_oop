
#include "classtree.h"
#include "ast.h"

void undefined_member_error(std::string str) {
    printf("UNDEFINED MEMBER ERROR\n");
    printf("----------------------\n");
    printf("an object had an undefined member trying to be accessed: ");
    printf("%s", str.c_str());
    exit(1);
}

ClassNode::ClassNode(ASTNode* classASTNode) {
    astNode = classASTNode;
    if (classASTNode == nullptr) {
        return;
    }
    parent = classNames.at(classASTNode->children[1]->identifier);

    ASTNode* block = classASTNode->children[2];
    int memPosition = parent == nullptr ? 0 : parent->getSize();
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
        if (!containsMember(curStatement->children[0]->identifier)) {
            if (curStatement->type == DECLARATION) {
                if (curStatement->children.size() == 2) { //default/explicit public case
                    if (curStatement->children[0]->identifier == classASTNode->children[0]->identifier) {
                        // if we have an instance variable of the same type of this class
                        // ex: Node class contains a Node instance variable to store a ref to next node
                        memberTypes.insert({curStatement->children[1]->identifier, this});
                    } else {
                        memberTypes.insert({curStatement->children[1]->identifier, classNames.at(curStatement->children[0]->identifier)});
                    }
                    memberPos.insert({curStatement->children[1]->identifier, memPosition});
                    memberIsPrivate.insert({curStatement->children[1]->identifier, false});
                } else { // explicit private case
                    if (curStatement->children[0]->identifier == classASTNode->children[0]->identifier) {
                        // if we have an instance variable of the same type of this class
                        // ex: Node class contains a Node instance variable to store a ref to next node
                        memberTypes.insert({curStatement->children[2]->identifier, this});
                    } else {
                        memberTypes.insert({curStatement->children[2]->identifier, classNames.at(curStatement->children[0]->identifier)});
                    }
                    memberPos.insert({curStatement->children[2]->identifier, memPosition});
                    memberIsPrivate.insert({curStatement->children[2]->identifier, true});
                }
            } else if (curStatement->type == ASSIGN) { 
                if (curStatement->children[0]->type == DECLARATION) {
                    if (curStatement->children[0]->children.size() == 2) { //default/explicit public
                        memberTypes.insert({curStatement->children[0]->children[1]->identifier, classNames.at(curStatement->children[0]->children[0]->identifier)});
                        memberPos.insert({curStatement->children[0]->children[1]->identifier, memPosition});
                        memberIsPrivate.insert({curStatement->children[0]->children[1]->identifier, false});
                    } else { //explicity private
                        memberTypes.insert({curStatement->children[0]->children[2]->identifier, classNames.at(curStatement->children[0]->children[0]->identifier)});
                        memberPos.insert({curStatement->children[0]->children[2]->identifier, memPosition});
                        memberIsPrivate.insert({curStatement->children[0]->children[2]->identifier, true});
                    }
                } else {
                    memberTypes.insert({curStatement->children[0]->identifier, classNames.at("int")});
                    memberPos.insert({curStatement->children[0]->identifier, memPosition});
                    memberIsPrivate.insert({curStatement->children[0]->identifier, false});
                }
            }
            memPosition += 8;
        }
    }
}

size_t ClassNode::getSize() {
    return (parent == nullptr ? 0 : parent->getSize()) + memberTypes.size() * sizeof (uint64_t);
}

ClassNode* ClassNode::getParent() {
    return parent;
}

bool ClassNode::containsMember(std::string member) {
    if (memberTypes.find(member) == memberTypes.end()) { 
        return parent == nullptr ? false : parent->containsMember(member);
    }
    return true;
}

ClassNode* ClassNode::getMemberType(std::string member) {
    if (memberTypes.find(member) == memberTypes.end()) { 
        
        if (parent == nullptr) {
            undefined_member_error(member);
        }

        return parent->getMemberType(member);
    }
    return memberTypes.at(member);
}

uint64_t ClassNode::getMemberPos(std::string member) {
    if (memberPos.find(member) == memberPos.end()) { 

        if (parent == nullptr) {
            undefined_member_error(member);
        }

        return parent->getMemberPos(member);
    }


    return memberPos.at(member);
}

int ClassNode::getMemberAccess(std::string member) {
    if (memberIsPrivate.find(member) == memberIsPrivate.end()) { 
        return parent->getMemberAccess(member) ? 2 : 0;
    }
    return memberIsPrivate.at(member) ? 1 : 0;
}

std::string& SSS (const char* s)
{
    return *(new std::string(s));
}
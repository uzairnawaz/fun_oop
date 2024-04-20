
#pragma once

#include <unordered_map>
#include <string>

struct ASTNode;

class ClassNode {
public:
    ClassNode(ASTNode* classASTNode);

    ClassNode* getParent();
    size_t getSize();
    std::unordered_map<std::string, ClassNode*> memberTypes;
    std::unordered_map<std::string, uint64_t> memberPos;
    std::unordered_map<std::string, bool> memberIsFunc;
    ASTNode* astNode;
private:
    ClassNode* parent;
};

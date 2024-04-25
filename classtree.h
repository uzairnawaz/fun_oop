
#pragma once

#include <unordered_map>
#include <string>

struct ASTNode;

class ClassNode {
public:
    std::unordered_map<std::string, ClassNode*> memberTypes;
    std::unordered_map<std::string, uint64_t> memberPos;
    std::unordered_map<std::string, bool> memberIsPrivate;
    ClassNode(ASTNode* classASTNode);

    ClassNode* getParent();
    size_t getSize();
    bool containsMember(std::string member);
    ClassNode* getMemberType(std::string member);
    uint64_t getMemberPos(std::string member);
    int getMemberAccess(std::string member);

    ASTNode* astNode;
private:
    ClassNode* parent;
};

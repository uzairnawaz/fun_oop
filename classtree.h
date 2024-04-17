
#pragma once

#include <unordered_map>
#include "slice.h"

struct ASTNode;

class ClassNode {
public:
    ClassNode(ASTNode* classASTNode);

    ClassNode* getParent();
    size_t getSize();
    std::unordered_map<Slice, ClassNode*, slice_hash_func, slice_equals_func> memberTypes;
    std::unordered_map<Slice, uint64_t, slice_hash_func, slice_equals_func> memberPos;
    std::unordered_map<Slice, bool, slice_hash_func, slice_equals_func> memberIsFunc;
    ASTNode* astNode;
private:
    ClassNode* parent;
};

#ifndef _VM_OBJ_LITERAL_H
#define _VM_OBJ_LITERAL_H
#pragma once

#include "value.h"
#include <string>
#include <unordered_map>

class VMObjLiteral {
public:
    void insert(const std::string& str, Value* v) {
        objLiteralProps.insert_or_assign(str, v);
    }
    Value* find(const std::string& str) const {
        auto found = objLiteralProps.find(str);
        if (found == objLiteralProps.end())
            return nullptr;
        return found->second;
    }
private:
    std::unordered_map<std::string, Value*> objLiteralProps;
};
#endif
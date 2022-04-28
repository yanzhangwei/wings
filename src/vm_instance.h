#ifndef _VM_INSTANCE_H
#define _VM_INSTANCE_H
#pragma once

#include <string>
#include <unordered_map>
#include "value.h"

// class VMClass {
// public:
//     VMClass() {}
//     void insert(const std::string& str, Value* v) {
//         data.insert_or_assign(str, v);
//     }
//     Value* find(const std::string& str) const {
//         auto found = data.find(str);
//         if (found == data.end())
//             return nullptr;
//         return found->second;
//     }
// private:
//     std::unordered_map<std::string, Value*> data;
// };

// class VMProperties {
// public:
// private:
    
// };

class VMInstance {
public:
    // VMInstance(VMClass* k): klass(k) {}
    void insert(const std::string& str, Value* v) {
        data.insert_or_assign(str, v);
    }
    Value* find(const std::string& str) const {
        auto found = data.find(str);
        if (found == data.end())
            return nullptr;
        return found->second;
    }
private:
    // VMClass* klass;
    std::unordered_map<std::string, Value*> data;   // new function()
};
#endif
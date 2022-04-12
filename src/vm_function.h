#ifndef _VM_FUNCTION_H
#define _VM_FUNCTION_H
#pragma once

#include "value.h"
#include <string>
#include <unordered_map>
#include <cstdint>
#include "chunk.h"
#include "ast_expr.h"

class VMFunction {
public:
    VMFunction(const std::vector<Identifier*>& p, Identifier* i, VMFunction* e = nullptr)
        : params(p), chunk(new Chunk), name(i), parent(e) {}
    VMFunction(): 
        chunk(new Chunk), 
        name(new Identifier("")),
        parent(nullptr) {}
    Chunk* getChunk() const { return chunk; }
    Identifier* getName() const { return name; }
    VMFunction* getParent() const { return parent; }
    std::vector<Identifier*> getParams() const { return params; }
    void insert(const std::string& str, Value *l) { hashmap.insert_or_assign(str, l); }
    Value* lookup(const std::string& key) const {
        auto tmp = hashmap;
        auto prev = this;
        while (prev) {
            auto found = tmp.find(key);
            if (found != hashmap.end())
                return found->second;
            prev = prev->parent;
            if (!prev) return nullptr;
            tmp = prev->hashmap;
        }
        return nullptr;
    }
private:
    std::vector<Identifier*> params;
    Chunk* chunk;
    Identifier* name;
    VMFunction* parent;
    std::unordered_map<std::string, Value*> hashmap;
};

struct CallFrame {
// public:
    CallFrame(VMFunction* f/*, CallFrame* e*/): fun(f), ip(0), slots(nullptr)/*, enclosing(e)*/ {}
    CallFrame(): fun(new VMFunction()), ip(0), slots(nullptr)/*, enclosing(nullptr)*/ {}
    VMFunction* getFunction() const { return fun; }
// private:
    VMFunction* fun;
    uint8_t ip;
    Value* slots;
    // CallFrame* enclosing;
};
#endif
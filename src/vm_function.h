#ifndef _VM_FUNCTION_H
#define _VM_FUNCTION_H
#pragma once

#include "value.h"
#include <string>
#include <unordered_map>
#include <cstdint>
#include "chunk.h"
#include "ast_expr.h"
#include "vm_instance.h"

struct CallFrame;

class VMFunction {
public:
    VMFunction(const std::vector<Identifier*>& p, Identifier* i, VMFunction* e = nullptr)
        : params(p), chunk(new Chunk), name(i)/*, parent(e)*/, instance(new VMInstance()), frame(nullptr) {}
    VMFunction(): 
        chunk(new Chunk), 
        name(new Identifier("")),
        /*parent(nullptr),*/ instance(new VMInstance()), frame(nullptr) {}
    Chunk* getChunk() const { return chunk; }
    Identifier* getName() const { return name; }
    // VMFunction* getParent() const { return parent; }
    std::vector<Identifier*> getParams() const { return params; }
    // void insert(const std::string& str, Value *l) { hashmap.insert_or_assign(str, l); }
    // Value* lookup(const std::string& key) const {
    //     auto tmp = hashmap;
    //     auto prev = this;
    //     while (prev) {
    //         auto found = tmp.find(key);
    //         if (found != hashmap.end())
    //             return found->second;
    //         prev = prev->parent;
    //         if (!prev) return nullptr;
    //         tmp = prev->hashmap;
    //     }
    //     return nullptr;
    // }
    void insertKlass(const std::string& str, Value* v) { instance->insert(str, v); }
    Value* find(const std::string& str) const { return instance->find(str); }
    void setFrame(CallFrame* f) { frame = f; }
    CallFrame* getFrame() const { return frame; }
    VMInstance* getInstance() const { return instance; }
private:
    std::vector<Identifier*> params;
    Chunk* chunk;
    Identifier* name;
    // VMFunction* parent;
    // std::unordered_map<std::string, Value*> hashmap;
    VMInstance* instance;
    // VMClass* klass;
    CallFrame* frame;
};

struct CallFrame {
// public:
    CallFrame(VMFunction* f, CallFrame* e = nullptr): fun(f), ip(0), slots(nullptr), prev(e) {
        fun->setFrame(this);
    }
    CallFrame(): fun(new VMFunction()), ip(0), slots(nullptr), prev(nullptr) {
        fun->setFrame(this);
    }
    VMFunction* getFunction() const { return fun; }
    void insert(const std::string& str, Value *l) { table.insert_or_assign(str, l); }
    Value* lookup(const std::string& key) const {
        auto tmp = this;
        while (tmp) {
            auto found = tmp->table.find(key);
            if (found != table.end())
                return found->second;
            tmp = tmp->prev;
        }
        return nullptr;
    }
// private:
    VMFunction* fun;
    uint8_t ip;
    Value* slots;
    CallFrame* prev;
    std::unordered_map<std::string, Value*> table;  //symtab
};
#endif
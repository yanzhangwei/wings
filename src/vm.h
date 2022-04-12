#ifndef _VM_H
#define _VM_H
#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include "chunk.h"
#include "ast_visitor.h"
#include "ast_expr_literal.h"
#include "symtab.h"
#include "vm_function.h"
#include "code_gen.h"

class VM {
public:
    VM(const ASTProgram* ast);
    ~VM();
    void run();
    
private:
    void binaryOp(const int& op);
    Value* pop();
    Value* peek(const int& distance) const;
    void setTable();
    void getTable();
    void call(Value* val, const int& args);

private:
    CodeGen *codegen;
    std::vector<Value*> stack;
    Symtab* table;
    std::vector<CallFrame*> frames; 
};
#endif
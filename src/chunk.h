#ifndef _CHUNK_H
#define _CHUNK_H
#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include "value.h"

enum OpCode: uint8_t {
    // OP_PRINT,
    OP_IDENTIFIER,
    OP_CONSTANT,
    OP_PLUS,
    OP_MINUS,
    OP_MUL,
    OP_DIV,
    OP_MOD,
    OP_GET,
    OP_SET,
    OP_CALL,
    OP_RETURN
};

class Chunk {
public:
    void pushCode(int byte) {code.emplace_back(byte); }
    void pushConstant(Value* l) { 
        constants.emplace_back(l);
        code.emplace_back(OP_CONSTANT);
        code.emplace_back(constants.size() - 1); 
    }
    Value* getConstantByIndex(const int& index) const { 
        auto p = constants[code[index]]; 
        return p;
    }
    uint8_t getCodeByIndex(const int& index) const { return index < code.size() ? code[index] : -1; }
    std::string getCodeStringByInde(const int& index) const { return opcode[index]; }
    int getCodeSize() const { return code.size(); }
    std::vector<uint8_t> getCode() const { return code; }
private:
    std::vector<uint8_t> code;
    std::vector<Value*> constants;
    static const std::vector<std::string> opcode;
};
#endif
#include "chunk.h"

const std::vector<std::string> Chunk::opcode = {
    "OP_IDENTIFIER",
    "OP_CONSTANT",
    "OP_PLUS",
    "OP_MINUS",
    "OP_MUL",
    "OP_DIV",
    "OP_MOD",
    "OP_GET",
    "OP_SET",
    "OP_CALL",
    "OP_RETURN"
};
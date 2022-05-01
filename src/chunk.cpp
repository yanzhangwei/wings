#include "chunk.h"

const std::vector<std::string> Chunk::opcode = {
    "OP_DUP",
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
    "OP_CLOSURE",
    "OP_INSTANCE",
    "OP_GET_PROPS",
    "OP_SET_PROPS",
    "OP_GET_OBJ_LITERAL_PROPS",
    "OP_SET_OBJ_LITERAL_PROPS",
    "OP_METHOD",
    "OP_THIS",
    "OP_RETURN"
};
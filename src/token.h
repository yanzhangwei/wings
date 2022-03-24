#ifndef _TOKEN_H
#define _TOKEN_H
#pragma once

#include <string>

enum class TokenType {
    // Keyword
    TOKEN_BREAK,
    TOKEN_CASE,
    TOKEN_CATCH,
    TOKEN_CONTINUE,
    TOKEN_DEBUGGER,
    TOKEN_DEFAULT,
    TOKEN_DO,
    TOKEN_ELSE,
    TOKEN_FINALLY,
    TOKEN_FOR,
    TOKEN_FUNCTION,
    TOKEN_IF,
    TOKEN_INSTANCEOF,
    TOKEN_NEW,
    TOKEN_RETURN,
    TOKEN_SWITCH,
    TOKEN_THIS,
    TOKEN_THROW,
    TOKEN_TYPEOF,
    TOKEN_VAR,
    TOKEN_VOID,
    TOKEN_WHILE,
    TOKEN_WITH,
    TOKEN_DELETE,
    TOKEN_IN,
    TOKEN_TRY,

    // FutureReservedWord
    TOKEN_CLASS,
    TOKEN_CONST,
    TOKEN_ENUM,
    TOKEN_EXPORT,
    TOKEN_EXTENDS,
    TOKEN_IMPORT,
    TOKEN_SUPER,

    TOKEN_IMPLEMENTS,
    TOKEN_INTERFACE,
    TOKEN_LET,
    TOKEN_PACKAGE,
    TOKEN_PRIVATE,
    TOKEN_PROTECTED,
    TOKEN_PUBLIC,
    TOKEN_STATIC,
    TOKEN_YIELD,

    TOKEN_LEFT_BRACE,           // {
    TOKEN_RIGHT_BRACE,          // }
    TOKEN_LEFT_PAREN,           // (
    TOKEN_RIGHT_PAREN,          // )
    TOKEN_LEFT_BRACKET,         // [
    TOKEN_RIGHT_BRACKET,        // ]
    TOKEN_DOT,                  // .
    TOKEN_SEMICOLON,            // ;
    TOKEN_COMMA,                // ,
    TOKEN_LESS,                 // <
    TOKEN_GREATER,              // >
    TOKEN_LESS_EQUAL,           // <=
    TOKEN_GREATER_EQUAL,        // >=
    TOKEN_EQUALITY,             // ==
    TOKEN_INEQUALITY,           // !=
    TOKEN_STRICT_EQUALITY,      // ===
    TOKEN_STRICT_INEQUALITY,    // !==
    TOKEN_PLUS,                 // +
    TOKEN_MINUS,                // -
    TOKEN_MUL,                  // *
    TOKEN_MOD,                  // %
    TOKEN_DPLUS,                // ++
    TOKEN_DMINUS,               // --
    TOKEN_DLESS,                // <<
    TOKEN_DGREATER,             // >>
    TOKEN_TGREATER,             // >>>
    TOKEN_BITWISE_AND,          // &
    TOKEN_BITWISE_OR,           // |
    TOKEN_BITWISE_XOR,          // ^
    TOKEN_NOT,                  // !
    TOKEN_BITWISE_NOT,          // ~
    TOKEN_LOGICAL_AND,          // &&
    TOKEN_LOGICAL_OR,           // ||
    TOKEN_CONDITIONAL,          // ?
    TOKEN_COLON,                // :
    TOKEN_EQUAL,                // =
    TOKEN_PLUS_EQUAL,           // +=
    TOKEN_MINUS_EQUAL,          // -=
    TOKEN_MUL_EQUAL,            // *=
    TOKEN_MOD_EQUAL,            // %=
    TOKEN_DLESS_EQUAL,          // <<=
    TOKEN_DGREATER_EQUAL,       // >>=
    TOKEN_TGREATER_EQUAL,       // >>>=
    TOKEN_BITWISE_AND_EQUAL,    // &=
    TOKEN_BITWISE_OR_EQUAL,     // |=
    TOKEN_BITWISE_XOR_EQUAL,    // ^=
    TOKEN_DIV,                  // /
    TOKEN_DIV_EQUAL,            // /=

    TOKEN_NULL,

    TOKEN_FALSE,
    TOKEN_TRUE,

    TOKEN_IDENTIFIER,
    TOKEN_NUMBER,
    TOKEN_STRING,

    TOKEN_ERROR,
    TOKEN_EOF,
};

enum class FutureReservedWord {
};

class Token {
public:
    Token(
        const int& c, const int& r, 
        const int& l, const char* s, 
        const TokenType t);

    TokenType getTokenType() const { return type; } 
private:
    int col;
    int row;
    int length;
    const char* start;
    TokenType type;
};
#endif
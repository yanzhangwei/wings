#include "token.h"
#include <vector>

Token::Token(const int& c, const int& r, 
        const int& l, const char* s, 
        const TokenType t)
            :col(c - l), row(r), length(l), start(s), type(t) {}

const std::vector<std::string> Token::enumString = {
    "break",
    "case",
    "catch",
    "continue",
    "debugger",
    "default",
    "do",
    "else",
    "finally",
    "for",
    "function",
    "if",
    "instanceof",
    "new",
    "return",
    "switch",
    "this",
    "throw",
    "typeof",
    "var",
    "void",
    "while",
    "with",
    "delete",
    "in",
    "try",

    // FutureReservedWord
    "class",
    "const",
    "enum",
    "export",
    "extends",
    "import",
    "super",

    "implements",
    "interface",
    "let",
    "package",
    "private",
    "protected",
    "public",
    "static",
    "yield",

    "{",           // {
    "}",          // }
    "(",           // (
    ")",          // )
    "[",         // [
    "]",        // ]
    ".",                  // .
    ";",            // ;
    ",",                // ,
    "<",                 // <
    ">",              // >
    "<=",           // <=
    ">=",        // >=
    "==",             // ==
    "!=",           // !=
    "===",      // ===
    "!==",    // !==
    "+",                 // +
    "-",                // -
    "*",                  // *
    "%",                  // %
    "++",                // ++
    "--",               // --
    "<<",                // <<
    ">>",             // >>
    ">>>",             // >>>
    "&",          // &
    "|",           // |
    "^",          // ^
    "!",                  // !
    "~",          // ~
    "&&",          // &&
    "||",           // ||
    "?",          // ?
    ":",                // :

    "=",                // =
    "+=",           // +=
    "-=",          // -=
    "*=",            // *=
    "%=",            // %=
    "<<=",          // <<=
    ">>=",       // >>=
    ">>>=",       // >>>=
    "&=",    // &=
    "|=",     // |=
    "^=",    // ^=
    "/=",            // /=
    
    "/",                  // /

    "null",

    "false",
    "true",

    "identifier",
    "number",
    "string",

    "error",
    "eof",
};
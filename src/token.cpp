#include "token.h"

Token::Token(const int& c, const int& r, 
        const int& l, const char* s, 
        const TokenType t)
            :col(c - l), row(r), length(l), start(s), type(t) {}
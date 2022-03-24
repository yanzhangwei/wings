#ifndef _SCANNER_H
#define _SCANNER_H
#pragma once

#include "token.h"
#include <string>

class Scanner {
public:
    Scanner(const std::string& path);
    ~Scanner();
    Token scanToken();
private:
    char advance();
    char peek() const;
    char* peekNext() const;
    bool isAlpha() const;
    bool isDigit(const char* c) const;
    bool isEnd() const;
    void skipWhiteSpace();

    Token makeToken(const TokenType& t) const;
    Token identifier();
    TokenType identifierType();
    TokenType checkKeyword(int pos, int length, const char* rest, const TokenType& type) const;
    Token number();
    Token errorTokne(const char* message) const;

    bool match(char expected);
private:
    int row;
    int col;
    char* start;
    char* cur;
};
#endif
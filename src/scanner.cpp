#include "scanner.h"
#include "token.h"
#include <cctype>
#include <cstring>
#include <fstream>
#include <cassert>
#include <iostream>
#include <string>

Scanner::Scanner(const std::string& path)
    : row(1), col(1) {
    std::ifstream fs(path);
    if (fs.fail()) {
        std::cout << "could not open file!" << std::endl;
        return;
    }

    fs.seekg(0, std::ios::end);
    int length = fs.tellg();
    fs.seekg(0, std::ios::beg);
    start = new char[length + 1];
    fs.read(start, length);
    fs.close();
    cur = start;
}

Scanner::~Scanner() {
    delete start;
}

Token Scanner::scanToken() {
    skipWhiteSpace();

    start = cur;

    if (isEnd()) return makeToken(TokenType::TOKEN_EOF);
    if (isAlpha()) return identifier();
    if (isDigit(cur)) return number();

    char c = advance();
    switch (c) {
        case '{': return makeToken(TokenType::TOKEN_LEFT_BRACE);
        case '}': return makeToken(TokenType::TOKEN_RIGHT_BRACE);
        case '(': return makeToken(TokenType::TOKEN_LEFT_PAREN);
        case ')': return makeToken(TokenType::TOKEN_RIGHT_PAREN);
        case '[': return makeToken(TokenType::TOKEN_LEFT_BRACKET);
        case ']': return makeToken(TokenType::TOKEN_RIGHT_BRACKET);
        case '.': return makeToken(TokenType::TOKEN_DOT);
        case ';': return makeToken(TokenType::TOKEN_SEMICOLON);
        case ',': return makeToken(TokenType::TOKEN_COMMA);
        case '<':
            if (match('='))
                return makeToken(TokenType::TOKEN_LESS_EQUAL);
            else if (match('<')) {
                if (match('='))
                    return makeToken(TokenType::TOKEN_DLESS_EQUAL);
                return makeToken(TokenType::TOKEN_DLESS);
            } else
                return makeToken(TokenType::TOKEN_LESS);
            break;
        case '>':
            if (match('='))
                return makeToken(TokenType::TOKEN_GREATER_EQUAL);
            else if (match('>')) {
                if (match('>')) {
                    if (match('='))
                        return makeToken(TokenType::TOKEN_TGREATER_EQUAL);
                    return makeToken(TokenType::TOKEN_TGREATER);
                }
                else if (match('='))
                    return makeToken(TokenType::TOKEN_DGREATER_EQUAL);
                return makeToken(TokenType::TOKEN_DGREATER);
            }
            return makeToken(TokenType::TOKEN_GREATER);
        case '=':
            if (match('=')) {
                if (match('='))
                    return makeToken(TokenType::TOKEN_STRICT_EQUALITY);
                return makeToken(TokenType::TOKEN_EQUALITY);
            }
            return makeToken(TokenType::TOKEN_EQUAL);
        case '!':
            if (match('=')) {
                if (match('='))
                    return makeToken(TokenType::TOKEN_STRICT_INEQUALITY);
                return makeToken(TokenType::TOKEN_INEQUALITY);
            }
            return makeToken(TokenType::TOKEN_NOT);
        case '+':
            if (match('+'))
                return makeToken(TokenType::TOKEN_DPLUS);
            else if (match('='))
                return makeToken(TokenType::TOKEN_PLUS_EQUAL);
            return makeToken(TokenType::TOKEN_PLUS);
        case '-':
            if (match('-'))
                return makeToken(TokenType::TOKEN_DMINUS);
            else if (match('='))
                return makeToken(TokenType::TOKEN_MINUS_EQUAL);
            return makeToken(TokenType::TOKEN_MINUS);
        case '*':
            return makeToken(match('=') ? TokenType::TOKEN_MUL_EQUAL : TokenType::TOKEN_MUL);
        case '%':
            return makeToken(match('=') ? TokenType::TOKEN_MOD_EQUAL : TokenType::TOKEN_MOD);
        case '&':
            if (match('&'))
                return makeToken(TokenType::TOKEN_LOGICAL_AND);
            else if (match('='))
                return makeToken(TokenType::TOKEN_BITWISE_AND_EQUAL);
            return makeToken(TokenType::TOKEN_BITWISE_AND);
        case '|':
            if (match('|'))
                return makeToken(TokenType::TOKEN_LOGICAL_OR);
            else if (match('='))
                return makeToken(TokenType::TOKEN_BITWISE_OR_EQUAL);
            return makeToken(TokenType::TOKEN_BITWISE_OR);
        case '^':
            return makeToken(match('=') ? TokenType::TOKEN_BITWISE_XOR_EQUAL : TokenType::TOKEN_BITWISE_XOR);
        case '~':
            return makeToken(TokenType::TOKEN_BITWISE_NOT);
        case '?':
            return makeToken(TokenType::TOKEN_CONDITIONAL);
        case ':':
            return makeToken(TokenType::TOKEN_COLON);
        case '/':
            return makeToken(match('=') ? TokenType::TOKEN_DIV_EQUAL : TokenType::TOKEN_DIV);
    }

    return errorTokne("errorToken");
}

char Scanner::advance() {
    ++col;
    ++cur;
    return cur[-1];
}

char Scanner::peek() const {
    return *cur;
}

char* Scanner::peekNext() const {
    return const_cast<char*>(cur + 1);
}

bool Scanner::isAlpha() const {
    return std::isalpha(*cur) || *cur == '_' || *cur == '$';
}

bool Scanner::isDigit(const char* c) const {
    return std::isdigit(*c);
}

bool Scanner::isEnd() const {
    return *cur == '\0';
}

void Scanner::skipWhiteSpace() {
    while (1) {
        switch (*cur) {
            case ' ':
            case '\t':
            case '\r':
                advance();
                break;
            case '\n':
                ++row;
                advance();
                break;
            default:
                return;;
        }
    }
}

Token Scanner::makeToken(const TokenType& t) const {
    return Token(col, row, cur - start, start, t);
}

Token Scanner::identifier() {
    while (isAlpha() || isDigit(cur)) advance();
    return makeToken(identifierType());
}

TokenType Scanner::identifierType() {
    switch (start[0]) {
        case 'b': return checkKeyword(1, 4, "reak", TokenType::TOKEN_BREAK);
        case 'c':
            if (cur - start > 1) {
                switch (start[1]) {             
                    case 'a': 
                        if (cur - start > 2) {
                            switch (start[2]) {
                                case 's': return checkKeyword(3, 1, "e", TokenType::TOKEN_CASE);
                                case 't': return checkKeyword(3, 2, "ch", TokenType::TOKEN_CATCH);
                            }
                        }
                    case 'o':
                        if (cur - start > 3 && start[2] == 'n') {
                            switch (start[3]) {
                                case 't': return checkKeyword(4, 4, "inue", TokenType::TOKEN_CONTINUE);
                                case 's': return checkKeyword(4, 1, "t", TokenType::TOKEN_CONST);
                            }
                        }
                    case 'l': return checkKeyword(2, 3, "ass", TokenType::TOKEN_CLASS);
                }
            }
            break;
        case 'd':
            if (cur - start > 1) {
                if (start[1] == 'o') return checkKeyword(2, 0, "", TokenType::TOKEN_DO);
                else if (start[1] == 'e')
                    if (cur - start > 2) {
                        switch (start[2]) {
                            case 'b': return checkKeyword(3, 5, "ugger", TokenType::TOKEN_DEBUGGER);
                            case 'f': return checkKeyword(3, 2, "ault", TokenType::TOKEN_DEFAULT);
                            case 'l': return checkKeyword(3, 3, "ete", TokenType::TOKEN_DELETE);
                        }
                    }
            }
            break;
        case 'e':
            if (cur - start > 1) {
                switch (start[1]) {
                    case 'l': return checkKeyword(2, 2, "se", TokenType::TOKEN_ELSE);
                    case 'n': return checkKeyword(2, 2, "um", TokenType::TOKEN_ENUM);
                    case 'x':
                        if (cur - start > 2) {
                            switch (start[2]) {
                                case 'p': return checkKeyword(3, 3, "ort", TokenType::TOKEN_EXPORT);
                                case 't': return checkKeyword(3, 4, "ends", TokenType::TOKEN_EXTENDS);
                            }
                        }
                }
            }
            break;
        case 'f':
            if (cur - start > 1) {
                switch (start[1]) {
                    case 'a': return checkKeyword(2, 3, "lse", TokenType::TOKEN_FALSE);
                    case 'i': return checkKeyword(2, 5, "nally", TokenType::TOKEN_FINALLY);
                    case 'o': return checkKeyword(2, 1, "r", TokenType::TOKEN_FOR);
                    case 'u': return checkKeyword(2, 6, "nction", TokenType::TOKEN_FUNCTION);
                }
            }
            break;
        case 'i':
            if (cur - start > 1) {
                switch (start[1]) {
                    case 'f': return checkKeyword(2, 0, "", TokenType::TOKEN_IF);
                    case 'm':
                        if (start[2] == 'p' && cur - start > 3) {
                            switch (start[3]) {
                                case 'o': return checkKeyword(4, 2, "rt", TokenType::TOKEN_IMPORT);
                                case 'l': return checkKeyword(2, 8, "plements", TokenType::TOKEN_IMPLEMENTS);
                            }               
                        }
                    case 'n':
                        if (cur - start == 2) return checkKeyword(2, 0, "", TokenType::TOKEN_IN);
                        else if (cur - start > 2) {
                            switch (start[2]) {
                                case 's': return checkKeyword(3, 7, "tanceof", TokenType::TOKEN_INSTANCEOF);
                                case 't': return checkKeyword(3, 6, "erface", TokenType::TOKEN_INTERFACE);
                            }
                        }
                }
            }
            break;
        case 'n':
            if (cur - start > 1) {
                switch (start[1]) {
                    case 'e': return checkKeyword(2, 1, "w", TokenType::TOKEN_NEW);
                    case 'u': return checkKeyword(2, 2, "ll", TokenType::TOKEN_NULL);
                }
            } 
            break;
        case 'r': return checkKeyword(1, 5, "eturn", TokenType::TOKEN_RETURN);
        case 's':
            if (cur - start > 1) {
                switch (start[1]) {
                    case 't': return checkKeyword(2, 4, "atic", TokenType::TOKEN_STATIC);
                    case 'u': return checkKeyword(2, 3, "per", TokenType::TOKEN_SUPER);
                    case 'w': return checkKeyword(2, 4, "itch", TokenType::TOKEN_SWITCH);
                }
            }
            break;
        case 't':
            if (cur - start > 1) {
                switch (start[1]) {
                    case 'r':
                        if (cur - start > 2) {
                            switch (start[2]) {
                                case 'y': return checkKeyword(3, 0, "", TokenType::TOKEN_TRY);
                                case 'u': return checkKeyword(3, 1, "e", TokenType::TOKEN_TRUE);
                            }
                        } 
                    case 'y': return checkKeyword(2, 4, "peof", TokenType::TOKEN_TYPEOF);
                    case 'h':
                        if (cur - start > 2) {
                            switch (start[2]) {
                                case 'i': return checkKeyword(3, 1, "s", TokenType::TOKEN_THIS);
                                case 'r': return checkKeyword(3, 2, "ow", TokenType::TOKEN_THROW);
                            }
                        }
                }
            }
            break;
        case 'v':
            if (cur - start > 1) {
                switch (start[1]) {
                    case 'a': return checkKeyword(2, 1, "r", TokenType::TOKEN_VAR);
                    case 'o': return checkKeyword(2, 2, "id", TokenType::TOKEN_VOID);
                }
            }
            break;
        case 'w':
            if (cur - start > 1) {
                switch (start[1]) {
                    case 'h': return checkKeyword(2, 3, "ile", TokenType::TOKEN_WHILE);
                    case 'i': return checkKeyword(2, 2, "th", TokenType::TOKEN_WITH);
                }
            }
            break;
        case 'l': return checkKeyword(1, 2, "et", TokenType::TOKEN_LET);
        case 'p':
            if (cur - start > 1) {
                switch (start[1]) {
                    case 'a': return checkKeyword(2, 5, "ckage", TokenType::TOKEN_PACKAGE);
                    case 'u': return checkKeyword(2, 4, "blic", TokenType::TOKEN_PUBLIC);
                    case 'r':
                        if (cur - start > 2) {
                            switch (start[2]) {
                                case 'i': return checkKeyword(3, 4, "vate", TokenType::TOKEN_PRIVATE);
                                case 'o': return checkKeyword(3, 6, "tected", TokenType::TOKEN_PROTECTED);
                            }
                        }
                }
            }
            break;
        case 'y': return checkKeyword(1, 4, "ield", TokenType::TOKEN_YIELD);
    }

    return TokenType::TOKEN_IDENTIFIER;
}

TokenType Scanner::checkKeyword(int start, int length, const char* rest, const TokenType& type) const {
    if (this->cur - this->start == start + length &&
        std::memcmp(this->start + start, rest, length) == 0)
        return type;
    return TokenType::TOKEN_IDENTIFIER;
}

Token Scanner::number() {
    while (isDigit(cur)) advance();

    if (peek() == '.' && isDigit(peekNext())) {
        advance();

        while (isDigit(cur)) advance();
    }

    return makeToken(TokenType::TOKEN_NUMBER);  
}

Token Scanner::errorTokne(const char* message) const {
    return Token(col,row, std::strlen(message), message, TokenType::TOKEN_ERROR);
}

bool Scanner::match(char expected) {
    if (isEnd()) return false;
    if (*cur != expected) return false;
    ++cur;
    return true;
}
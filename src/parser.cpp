#include "parser.h"

Parser::Parser(Scanner* sc): scanner(sc) {
    parseProgram();
}

Parser::~Parser() { delete scanner; }

void Parser::parseProgram() {
    Token token = scanner->scanToken();
    while (token.getTokenType() != TokenType::TOKEN_ERROR &&
            token.getTokenType() != TokenType::TOKEN_EOF) {
        token = scanner->scanToken();
    }
}
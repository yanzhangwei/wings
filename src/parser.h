#ifndef _PARSER_H
#define _PARSER_H
#pragma once

#include "scanner.h"

class Parser {
public:
    Parser(Scanner* sc);
    ~Parser();

    void parseProgram();
private:
    Scanner *scanner;
};
#endif
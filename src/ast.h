#ifndef _AST_H
#define _AST_H
#pragma once

#include <cstdint>
#include <string>
#include <vector>
#include <iostream>

enum class ASTType : uint8_t {
    AST_PROGRAM,
    AST_EXPR_IDENTIFIER,
    AST_EXPR_BINARY,
    AST_EXPR_ARRAY,
    AST_EXPR_MEMBER,
    AST_EXPR_OBJECT,
    AST_EXPR_CALL,
    AST_EXPR_LITERAL,
    AST_EXPR_CONDITION,
    AST_EXPR_ASSIGNMENT,
    AST_EXPR_UNARY,
    AST_VARIABLEDECLARATION,
    AST_VARIABLEDECLARATOR,
    AST_PROPERTY,
    AST_FUNCTIONDECLARATION,
    AST_STMT_EXPRESSION,
    AST_STMT_BLOCK,
    AST_STMT_RETURN,
    AST_STMT_IF,
    AST_STMT_LABEL,
    AST_STMT_CONTINUE,
    AST_STMT_BREAK,
    AST_STMT_FOR,
    AST_STMT_FORIN,
    AST_STMT_DOWHILE,
    AST_STMT_WHILE,
};

class Expr;

class AST {
public:
    // AST(const ASTType& t): type(t)/*, start(0), end(0)*/ {}
    virtual ASTType getType() { return ASTType::AST_PROGRAM; }
private:
    // int start;
    // int end;
};
using ASTPtrVec = std::vector<AST*>;

class ASTProgram: public AST {
public:
    void push_back(Expr* b) {
        body.emplace_back(b);
    }

    std::vector<Expr*> getBody() const { return body; }
private:
    std::vector<Expr*> body;
};
#endif
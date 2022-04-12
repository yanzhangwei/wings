#ifndef _AST_EXPR_LITERAL
#define _AST_EXPR_LITERAL
#pragma once

#include <variant>
#include <string>
#include <iostream>
#include "ast_expr.h"

// regularExpression
class Literal: public Expr {
public:
    using Value = std::variant<double, bool, std::string, std::monostate>;
    Literal(const Value& v): value(v) {}
    virtual ASTType getType() const { 
        return ASTType::AST_EXPR_LITERAL; 
    }
    virtual void accept(ASTVisitor* visitor);
    void print() const {
        if (std::holds_alternative<double>(value)) {
            std::cout << std::get<double>(value);
        } else if (std::holds_alternative<bool>(value)) {
            std::cout << std::get<bool>(value);
        } else if (std::holds_alternative<std::string>(value)) {
            std::cout << std::get<std::string>(value);
        } else {
            std::cout << "undefined";
        }
    }
    // bool isNumber() const { return std::holds_alternative<double>(value); }
    // double getNumber() const { return std::get<double>(value); }
    // std::string getString() const { return std::get<std::string>(value); } 
    Value getValue() const { return value; }
private:
    Value value;
};

// class LiteralNull: public Literal {

// };

// class LiteralBoolean: public Literal {

// };

// class LiteralNumeric: public Literal {

// };

// class LiteralString: public Literal {

// };

// class LiteralRegularExpression: public Literal {

// };
#endif
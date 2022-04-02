#ifndef _AST_DECL
#define _AST_DECL
#include "ast.h"
#pragma once

#include <string>
#include <ostream>
#include <vector>
#include "token.h"
#include "ast_expr_literal.h"

class ASTVariableDeclaration: public Expr {
public:
    ASTVariableDeclaration(const Token& t): kind(t) {}
    Token getKind() const { return kind; }
    void push_back(Expr* ast) { 
        declarations.emplace_back(ast); 
    }
    ExprPtrVec getDeclarations() const { return declarations; }
    virtual ASTType getType() const { 
        return ASTType::AST_VARIABLEDECLARATION; 
    }
private:
    Token kind;
    ExprPtrVec declarations;
};

class ASTVariableDeclarator: public Expr {
public:
    ~ASTVariableDeclarator() {
        delete id;
    }

    void setId(Identifier* i) { id = i; }
    void setInit(Expr* e) { init = e; }
    Identifier* getId() const { return id; }
    Expr* getInit() const { return init; }
    virtual ASTType getType() const { 
        return ASTType::AST_VARIABLEDECLARATOR; 
    }
private:
    Expr*    init;
    Identifier* id;
};

class ASTFunctionDeclaration: public Expr {
public:
    Identifier* getId() const { return id; }
    std::vector<Identifier*> getParams() const { return params; }
    Expr* getBlock() const { return block; }
    void setId(Identifier* i) { id = i; }
    void setParams(const std::vector<Identifier*>& p) { params = p; }
    void setBlock(Expr* stmt) { block = stmt; }
    virtual ASTType getType() const { 
        return ASTType::AST_FUNCTIONDECLARATION; 
    }
private:
    Identifier* id;
    std::vector<Identifier*> params;
    Expr* block;
};
#endif
#ifndef _AST_EXPR_H
#define _AST_EXPR_H
#pragma once

#include "ast.h"
#include "token.h"
#include <vector>
#include <string>

class Expr: public AST {
public:
    virtual ASTType getType() const = 0;
};
using ExprPtrVec = std::vector<Expr*>;

class Identifier: public Expr {
public:
    Identifier(const std::string& n): name(n) {}
    virtual ASTType getType() const { 
        return ASTType::AST_EXPR_IDENTIFIER; 
    }
    std::string getName() const { return name; }
private:
    std::string name;
};

class BinaryExpr: public Expr {
public:
    BinaryExpr(Expr* L, const Token& t, Expr* R):
        LHS(L), op(t), RHS(R){}
    virtual ASTType getType() const { 
        return ASTType::AST_EXPR_BINARY; 
    }
    Expr* getLHS() const { return LHS; }
    Expr* getRHS() const { return RHS; }
    std::string getOp() const { return op.enumToString(); }
private:
    Expr *LHS, *RHS;
    Token op;
};

class ArrayExpr: public Expr {
public:
    virtual ASTType getType() const { 
        return ASTType::AST_EXPR_ARRAY; 
    }

    void push_back(Expr* e) { elements.emplace_back(e); }
private:
    ExprPtrVec elements;
};

class Property: public Expr {
public:
    Property(): kind(0) {}
    virtual ASTType getType() const { 
        return ASTType::AST_PROPERTY; 
    }
    void setKey(Expr* k) { key = k; }
    void setValue(Expr* v) { value = v; }
    Expr* getKey() const { return key; }
    Expr* getValue() const { return value; }
    void setKind(const int& index) { kind = index; }
private:
    Expr* key;
    Expr* value;
    static std::vector<std::string> kinds;
    int kind;   
};

class ObjectExpr: public Expr {
public:
    virtual ASTType getType() const { 
        return ASTType::AST_EXPR_OBJECT; 
    }

private:
    ExprPtrVec properties;
};

class CallExpr: public Expr {
public:
    virtual ASTType getType() const { 
        return ASTType::AST_EXPR_CALL; 
    }
    void setCallee(Expr* c) { callee = c; }
    void setArgs(const ExprPtrVec& vec) {
        if (vec.size()) 
            arguments = vec; 
    }
    Expr* getCallee() const { return callee; }
    ExprPtrVec getArgs() const { return arguments; }
private:
    Expr* callee;
    ExprPtrVec arguments;
};

class MemberExpr: public Expr {
public:
    MemberExpr(): object(nullptr), property(nullptr) {}
    ~MemberExpr() {
        delete object;
        delete property;
    }
    void setObject(Expr* o) { object = o; }
    void setProperty(Expr* p) { property = p; }
    Expr* getObject() const { return object; }
    Expr* getProperty() const { return property; }
    virtual ASTType getType() const { 
        return ASTType::AST_EXPR_MEMBER; 
    }
private:
    Expr* object;
    Expr* property;
};

class ConditionExpr {
public:
    virtual ASTType getType() const { 
        return ASTType::AST_EXPR_CONDITION; 
    }
private:

};

class AssignmentExpr: public Expr {
public:
    AssignmentExpr(Expr* L, const Token& t, Expr* R):
        LHS(L), op(t), RHS(R){}
    Expr* getLHS() const { return LHS; }
    Expr* getRHS() const { return RHS; }
    std::string getOp() const { return op.enumToString(); }
    virtual ASTType getType() const { 
        return ASTType::AST_EXPR_ASSIGNMENT; 
    }
private:
    Expr *LHS, *RHS;
    Token op;
};

class UnaryExpr: public Expr {
public:
    UnaryExpr(const Token& t, Expr* a, bool pre = true):
        argument(a), op(t), prefix(pre) {}
    virtual ASTType getType() const { 
        return ASTType::AST_EXPR_UNARY; 
    }
    bool getPrefix() const { return prefix; }
    std::string getOp() const { return op.enumToString(); }
    Expr* getArgument() const { return argument; }
private:
    bool prefix;
    Token op;
    Expr* argument;
};
#endif
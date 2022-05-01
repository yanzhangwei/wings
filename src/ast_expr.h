#ifndef _AST_EXPR_H
#define _AST_EXPR_H
#pragma once

#include "ast.h"
#include "token.h"
#include <vector>
#include <string>
#include "ast_visitor.h"
class Expr: public AST {
public:
    virtual ASTType getType() const = 0;
    virtual void accept(ASTVisitor* visitor) = 0;
};
using ExprPtrVec = std::vector<Expr*>;

class Identifier: public Expr {
public:
    Identifier(const std::string& n): name(n) {}
    virtual ASTType getType() const { 
        return ASTType::AST_EXPR_IDENTIFIER; 
    }
    virtual void accept(ASTVisitor* visitor);
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
    virtual void accept(ASTVisitor* visitor);
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
    virtual void accept(ASTVisitor* visitor);
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
    virtual void accept(ASTVisitor* visitor);
    void setKey(Expr* k) { key = k; }
    void setValue(Expr* v) { value = v; }
    Expr* getKey() const { return key; }
    Expr* getValue() const { return value; }
    std::string getKind() const { return kinds[kind]; }
    void setKind(const int& index) { kind = index; }
private:
    Expr* key;
    Expr* value;
    static std::vector<std::string> kinds;
    int kind;   
};

class ObjectExpr: public Expr {
public:
    void setProperties(const std::vector<Property*>& p) { properties = p; }
    std::vector<Property*> getProperties() const { return properties; }
    virtual ASTType getType() const { 
        return ASTType::AST_EXPR_OBJECT; 
    }
    virtual void accept(ASTVisitor* visitor);
private:
    std::vector<Property*> properties;
};

class CallExpr: public Expr {
public:
    virtual ASTType getType() const { 
        return ASTType::AST_EXPR_CALL; 
    }
    virtual void accept(ASTVisitor* visitor);
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
    MemberExpr(): object(nullptr), property(nullptr), 
                  isLeftHand(false), isFun(false) {}
    // ~MemberExpr() {
    //     delete object;
    //     delete property;
    // }
    void setObject(Expr* o) { object = o; }
    void setProperty(Expr* p) { property = p; }
    void setIsLeftHand() { isLeftHand = true; }
    bool getIsLeftHand() const { return isLeftHand; }
    Expr* getObject() const { return object; }
    Expr* getProperty() const { return property; }
    void setIsFun() { isFun = true; }
    bool getIsFun() const { return isFun; }
    virtual ASTType getType() const { 
        return ASTType::AST_EXPR_MEMBER; 
    }
    virtual void accept(ASTVisitor* visitor);
private:
    Expr* object;
    Expr* property;
    bool isLeftHand;
    bool isFun;
};

class ConditionExpr {
public:
    virtual ASTType getType() const { 
        return ASTType::AST_EXPR_CONDITION; 
    }
    virtual void accept(ASTVisitor* visitor);
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
    virtual void accept(ASTVisitor* visitor);
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
    virtual void accept(ASTVisitor* visitor);
    bool getPrefix() const { return prefix; }
    std::string getOp() const { return op.enumToString(); }
    Expr* getArgument() const { return argument; }
private:
    bool prefix;
    Token op;
    Expr* argument;
};

class ThisExpr: public Expr {
public:
    virtual ASTType getType() const { 
        return ASTType::AST_EXPR_THIS; 
    }
    virtual void accept(ASTVisitor* visitor);
};

class NewExpr: public Expr {
public:
    virtual ASTType getType() const { 
        return ASTType::AST_EXPR_NEW; 
    }
    virtual void accept(ASTVisitor* visitor);
    void setCallee(Expr* e) { callee = e; }
    void setArguments(const std::vector<Expr*>& vec) { arguments =vec; }
    Expr* getCallee() const { return callee; }
    std::vector<Expr*> getArguments() const { return arguments; }
private:
    Expr* callee;
    std::vector<Expr*> arguments;
};

class FunctionExpr: public Expr {
public:
    FunctionExpr(): id(nullptr), block(nullptr) {}
    virtual ASTType getType() const { 
        return ASTType::AST_EXPR_FUN; 
    }
    virtual void accept(ASTVisitor* visitor);
    void setId(Identifier* i) { id = i; }
    void setParams(const std::vector<Identifier*>& p) { params = p; }
    void setBlock(Expr* b) { block = b; }
    Identifier* getId() const { return id; }
    std::vector<Identifier*> getParams() const { return params; }
    Expr* getBlock() const { return block; }
private:
    Identifier* id;
    std::vector<Identifier*> params;
    Expr* block;
};
#endif
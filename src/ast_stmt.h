#ifndef _AST_STMT_H
#define _AST_STMT_H
#include "ast.h"
#pragma once

#include <vector>
#include "ast_expr.h"

class BlockStmt: public Expr {
public:
    virtual ASTType getType() const { 
        return ASTType::AST_STMT_BLOCK; 
    }
    virtual void accept(ASTVisitor* visitor);
    void push_back(Expr* e) { body.emplace_back(e); }
    ExprPtrVec getBody() const { return body; }
private:
    ExprPtrVec body;
};

class ReturnStmt: public Expr {
public:
    ReturnStmt(): argument(nullptr) {}
    virtual ASTType getType() const { 
        return ASTType::AST_STMT_RETURN; 
    }
    virtual void accept(ASTVisitor* visitor);
    void setExpr(Expr* e) { argument = e; }
    Expr* getArgument() const { return argument; }
private:
    Expr* argument;
};

class IfStmt: public Expr {
public:
    void setTest(Expr* e) { test = e; }
    void setConsequent(Expr* e) { consequent = e; }
    void setAlternate(Expr* e) { alternate = e; }
    Expr* getTest() const { return test; }
    Expr* getConsequent() const { return consequent; }
    Expr* getAlternate() const { return alternate; }
    virtual ASTType getType() const { 
        return ASTType::AST_STMT_IF; 
    }
    virtual void accept(ASTVisitor* visitor);
private:
    Expr* test;
    Expr* consequent;
    Expr* alternate;
};

class LabelStmt: public Expr {
public:
    LabelStmt(Expr* l, Expr* b): label(l), body(b) {}
    Expr* getLabel() const { return label; }
    Expr* getBody() const { return body; }
    virtual ASTType getType() const { 
        return ASTType::AST_STMT_LABEL; 
    }
    virtual void accept(ASTVisitor* visitor);
private:
    Expr* label;
    Expr* body;
};

class ContinueStmt: public Expr {
public:
    ContinueStmt(Identifier* l = nullptr): label(l) {}
    virtual ASTType getType() const { 
        return ASTType::AST_STMT_CONTINUE; 
    }
    virtual void accept(ASTVisitor* visitor);
    Identifier* getLabel() const { return label; }
private:
    Identifier* label;
};

class BreakStmt:  public Expr {
public:
    BreakStmt(Identifier* l = nullptr): label(l) {}
    virtual ASTType getType() const { 
        return ASTType::AST_STMT_BREAK; 
    }
    virtual void accept(ASTVisitor* visitor);
    Identifier* getLabel() const { return label; }
private:
    Identifier* label;
};

class ForStmt: public Expr {
public:
    ForStmt(Expr* i, Expr* t, Expr* u, Expr* b)
        :init(i), test(t), update(u), body(b) {}
    Expr* getInit() const { return init; }
    Expr* getTest() const { return test; }
    Expr* getUpdate() const { return update; }
    Expr* getBody() const { return body; }
    virtual ASTType getType() const { 
        return ASTType::AST_STMT_FOR; 
    }
    virtual void accept(ASTVisitor* visitor);
private:
    Expr *init, *test, *update;
    Expr *body;
};

class ForInStmt: public Expr {
public:
    ForInStmt(Expr* l, Expr* r, Expr* b):
        LHS(l), RHS(r), body(b) {}
    Expr* getLHS() const { return LHS; }
    Expr* getRHS() const { return RHS; }
    Expr* getBody() const { return body; }
    virtual ASTType getType() const { 
        return ASTType::AST_STMT_FORIN; 
    }
    virtual void accept(ASTVisitor* visitor);
private:
    Expr *LHS, *RHS;
    Expr *body;
};

class DoWhileStmt: public Expr {
public:
    DoWhileStmt(Expr* e, Expr* b): test(e), body(b) {}
    virtual ASTType getType() const { 
        return ASTType::AST_STMT_DOWHILE; 
    }
    virtual void accept(ASTVisitor* visitor);
    Expr* getTest() const { return test; }
    Expr* getBody() const { return body; }
private:
    Expr* test;
    Expr* body;
};

class WhileStmt: public Expr {
public:
    WhileStmt(Expr* e, Expr* b): test(e), body(b) {}
    virtual ASTType getType() const { 
        return ASTType::AST_STMT_WHILE; 
    }
    virtual void accept(ASTVisitor* visitor);
    Expr* getTest() const { return test; }
    Expr* getBody() const { return body; }
private:
    Expr* test;
    Expr* body;
};
#endif
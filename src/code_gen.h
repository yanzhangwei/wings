#ifndef _CODE_GEN_H
#define _CODE_GEN_H
#pragma once

#include "symtab.h"
#include "value.h"
#include <vector>
#include "ast_visitor.h"
#include "vm_function.h"

class CodeGen: public ASTVisitor {
public:
    // CodeGen(): fun(new VMFunction)/*, table(new Symtab)*/ {}
    CodeGen(): frame(new CallFrame()) {}
    void visitASTProgram(const ASTProgram* node);
    virtual void visitVariableDeclaration(const ASTVariableDeclaration* node);
    virtual void visitVariableDeclarator(const ASTVariableDeclarator* node);
    virtual void visitIdentifier(const Identifier* node);
    virtual void visitMemberExpr(const MemberExpr* node);
    virtual void visitLiteral(const Literal* node);
    virtual void visitCallExpr(const CallExpr* node);
    virtual void visitBinaryExpr(const BinaryExpr* node);
    virtual void visitFunctionDeclaration(const ASTFunctionDeclaration* node);
    virtual void visitBlockStmt(const BlockStmt* node);
    virtual void visitReturnStmt(const ReturnStmt* node);
    virtual void visitIfStmt(const IfStmt* node);
    virtual void visitAssignmentExpr(const AssignmentExpr* node);
    virtual void visitLabelStmt(const LabelStmt* node);
    virtual void visitContinueStmt(const ContinueStmt* node);
    virtual void visitBreakStmt(const BreakStmt* node);
    virtual void visitForStmt(const ForStmt* node);
    virtual void visitForInStmt(const ForInStmt* node);
    virtual void visitWhileStmt(const WhileStmt* node);
    virtual void visitDoWhileStmt(const DoWhileStmt* node);
    virtual void visitUnaryExpr(const UnaryExpr* node);
    virtual void visitArrayExpr(const ArrayExpr* node);
    virtual void visitThisExpr(const ThisExpr* node);
    virtual void visitNewExpr(const NewExpr* node);
    virtual void visitFunctionExpr(const FunctionExpr* node);
    virtual void visitProperty(const Property* node);
    virtual void visitObjectExpr(const ObjectExpr* node);

    void dumpCallFrames(const VMFunction* fun) const;
    // VMFunction* getFunction() const { return fun; }
    // Symtab* getSymtab() const { return table; }
    CallFrame* getFrame() const { return frame; }
private:
    // VMFunction* fun;
    // Symtab* table;
    CallFrame* frame; 
};
#endif
#ifndef _AST_VISITOR_H
#define _AST_VISITOR_H
#pragma once

class ASTVariableDeclaration;
class ASTVariableDeclarator;
class Identifier;
class MemberExpr;
class Literal;
class CallExpr;
class BinaryExpr;
class ASTFunctionDeclaration;
class BlockStmt;
class ReturnStmt;
class IfStmt;
class AssignmentExpr;
class LabelStmt;
class ContinueStmt;
class BreakStmt;
class ForStmt;
class ForInStmt;
class DoWhileStmt;
class WhileStmt;
class UnaryExpr;
class ArrayExpr;
class ThisExpr;
class NewExpr;
class FunctionExpr;
class Property;
class ObjectExpr;

class ASTVisitor {
public:
    // virtual void visitASTProgram(const ASTProgram* node) = 0;
    virtual void visitVariableDeclaration(const ASTVariableDeclaration* node) = 0;
    virtual void visitVariableDeclarator(const ASTVariableDeclarator* node) = 0;
    virtual void visitIdentifier(const Identifier* node) = 0;
    virtual void visitMemberExpr(const MemberExpr* node) = 0;
    virtual void visitLiteral(const Literal* node) = 0;
    virtual void visitCallExpr(const CallExpr* node) = 0;
    virtual void visitBinaryExpr(const BinaryExpr* node) = 0;
    virtual void visitFunctionDeclaration(const ASTFunctionDeclaration* node) = 0;
    virtual void visitBlockStmt(const BlockStmt* node) = 0;
    virtual void visitReturnStmt(const ReturnStmt* node) = 0;
    virtual void visitIfStmt(const IfStmt* node) = 0;
    virtual void visitAssignmentExpr(const AssignmentExpr* node) = 0;
    virtual void visitLabelStmt(const LabelStmt* node) = 0;
    virtual void visitContinueStmt(const ContinueStmt* node) = 0;
    virtual void visitBreakStmt(const BreakStmt* node) = 0;
    virtual void visitForStmt(const ForStmt* node) = 0;
    virtual void visitForInStmt(const ForInStmt* node) = 0;
    virtual void visitWhileStmt(const WhileStmt* node) = 0;
    virtual void visitDoWhileStmt(const DoWhileStmt* node) = 0;
    virtual void visitUnaryExpr(const UnaryExpr* node) = 0;
    virtual void visitArrayExpr(const ArrayExpr* node) = 0;
    virtual void visitThisExpr(const ThisExpr* node) = 0;
    virtual void visitNewExpr(const NewExpr* node) = 0;
    virtual void visitFunctionExpr(const FunctionExpr* node) = 0;
    virtual void visitProperty(const Property* node) = 0;
    virtual void visitObjectExpr(const ObjectExpr* node) = 0;
};
#endif
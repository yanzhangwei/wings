#include "ast_expr.h"
#include "ast_decl.h"
#include "ast_stmt.h"
#include "ast_expr_literal.h"
// #include "ast_visitor.h"

void Identifier::accept(ASTVisitor *visitor) {
    visitor->visitIdentifier(this);
}

void BinaryExpr::accept(ASTVisitor *visitor) {
    visitor->visitBinaryExpr(this);
}

void ArrayExpr::accept(ASTVisitor *visitor) {
    visitor->visitArrayExpr(this);
}

void Property::accept(ASTVisitor *visitor) {
    // visitor->
}

void ObjectExpr::accept(ASTVisitor *visitor) {

}

void CallExpr::accept(ASTVisitor *visitor) {
    visitor->visitCallExpr(this);
}

void MemberExpr::accept(ASTVisitor *visitor) {
    visitor->visitMemberExpr(this);
}

void ConditionExpr::accept(ASTVisitor *visitor) {

}

void AssignmentExpr::accept(ASTVisitor *visitor) {
    visitor->visitAssignmentExpr(this);
}

void UnaryExpr::accept(ASTVisitor *visitor) {
    visitor->visitUnaryExpr(this);
}

void BlockStmt::accept(ASTVisitor *visitor) {
    visitor->visitBlockStmt(this);
}

void ReturnStmt::accept(ASTVisitor *visitor) {
    visitor->visitReturnStmt(this);
}

void IfStmt::accept(ASTVisitor *visitor) {
    visitor->visitIfStmt(this);
}

void LabelStmt::accept(ASTVisitor *visitor) {
    visitor->visitLabelStmt(this);
}

void ContinueStmt::accept(ASTVisitor *visitor) {
    visitor->visitContinueStmt(this);
}

void BreakStmt::accept(ASTVisitor *visitor) {
    visitor->visitBreakStmt(this);
}

void ForStmt::accept(ASTVisitor *visitor) {
    visitor->visitForStmt(this);
}

void ForInStmt::accept(ASTVisitor *visitor) {
    visitor->visitForInStmt(this);
}

void DoWhileStmt::accept(ASTVisitor *visitor) {
    visitor->visitDoWhileStmt(this);
}

void WhileStmt::accept(ASTVisitor *visitor) {
    visitor->visitWhileStmt(this);
}

void Literal::accept(ASTVisitor *visitor) {
    visitor->visitLiteral(this);
}

void ASTVariableDeclaration::accept(ASTVisitor *visitor) {
    visitor->visitVariableDeclaration(this);
}

void ASTVariableDeclarator::accept(ASTVisitor *visitor) {
    visitor->visitVariableDeclarator(this);
}

void ASTFunctionDeclaration::accept(ASTVisitor *visitor) {
    visitor->visitFunctionDeclaration(this);
}
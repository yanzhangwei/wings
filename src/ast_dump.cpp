#include "ast_dump.h"
#include "ast.h"
#include "ast_expr.h"
#include "ast_decl.h"
#include "ast_stmt.h"
#include <iostream>
#include <ostream>

std::vector<std::string> Property::kinds = {
    "init",
    "get",
    "set"
};

void ASTDump::visitASTProgram(const ASTProgram* node) {
    printType("Program");

    printTab();
    std::cout << "body: ";
    printLeftBracket();
    for (const auto& e: node->getBody()) {
        e->accept(this);
    }
    printRightBracket();
    printRightBrace();
}

void ASTDump::visitVariableDeclaration(const ASTVariableDeclaration *node) {
    printType("VariableDeclaration");

    printTab();
    std::cout << "declarations: ";
    printLeftBracket();
    for (const auto& e: node->getDeclarations()) {
        e->accept(this);
    }
    printRightBracket();
    printTab();
    std::cout << "kind: " << node->getKind().enumToString() << std::endl;
    printRightBrace();
}

void ASTDump::visitVariableDeclarator(const ASTVariableDeclarator* node) {
    printType("VariableDeclarator");

    printTab();
    std::cout << "id: ";
    printLeftBrace();
    // id
    node->getId()->accept(this);

    printRightBrace();
    printTab();
    std::cout << "init: ";
    printLeftBrace();
    // init
    node->getInit()->accept(this);
    printRightBrace();
    printRightBrace();
}

void ASTDump::visitIdentifier(const Identifier* node) {
    printType("Identifier");

    printTab();
    std::cout << "name: " << node->getName() << std::endl;
    printRightBrace();
}

void ASTDump::visitMemberExpr(const MemberExpr* node) {
    printType("MemberExpr");

    printTab();
    std::cout << "object: ";
    printLeftBrace();

    auto p = node->getObject()->getType();
    node->getObject()->accept(this);
    printRightBrace();

    printTab();
    std::cout << "property: ";
    printLeftBrace();

    node->getProperty()->accept(this);
    printRightBrace();
    printRightBrace(); 
}

void ASTDump::visitLiteral(const Literal *node) {
    printType("Literal");

    printTab();
    std::cout << "value: ";
    node->print();
    
    std::cout << std::endl;
    printRightBrace();
}

void ASTDump::visitCallExpr(const CallExpr* node) {
    printType("CallExpr");

    printTab();
    std::cout << "callee: ";
    printLeftBrace();
    node->getCallee()->accept(this);
    printRightBrace();

    printTab();
    std::cout << "arguments: ";
    printLeftBracket();
    for (const auto& arg: node->getArgs()) {
        arg->accept(this);
    }
    printRightBracket();

    printRightBrace();
}

void ASTDump::visitBinaryExpr(const BinaryExpr* node) {
    printType("BinaryExpr");

    printTab();
    std::cout << "left ";
    printLeftBrace();
    node->getLHS()->accept(this);
    printRightBrace();

    printTab();
    std::cout << "operator: " << node->getOp() << std::endl;

    printTab();
    std::cout << "right ";
    printLeftBrace();
    node->getRHS()->accept(this);
    printRightBrace();
    printRightBrace();
}

void ASTDump::visitFunctionDeclaration(const ASTFunctionDeclaration* node) {
    printType("FunctionDeclaration");

    printTab();
    std::cout<< "id: ";
    printLeftBrace();
    node->getId()->accept(this);
    printRightBrace();

    printTab();
    std::cout<< "params: ";
    printLeftBracket();
    for (const auto& p: node->getParams())
        p->accept(this);
    printRightBracket();

    printTab();
    std::cout<< "body: ";
    printLeftBracket();
    node->getBlock()->accept(this);
    printRightBracket();

    printRightBrace();
}

void ASTDump::visitBlockStmt(const BlockStmt* node) {
    for (const auto& e: node->getBody())
        e->accept(this);
}

void ASTDump::visitReturnStmt(const ReturnStmt* node) {
    printType("ReturnStmt");

    printTab();
    std::cout << "body ";
    printLeftBrace();
    node->getArgument()->accept(this);
    printRightBrace();
    printRightBrace();
}

void ASTDump::visitIfStmt(const IfStmt* node) {
    printType("IfStmt");

    printTab();
    std::cout << "test: ";
    printLeftBrace();
    node->getTest()->accept(this);
    printRightBrace();

    printTab();
    std::cout << "consequent: ";
    printLeftBrace();
    node->getConsequent()->accept(this);
    printRightBrace();

    printTab();
    std::cout << "alternate: ";
    if (node->getAlternate()) {
        printLeftBrace();
        node->getAlternate()->accept(this);
        printRightBrace();
    }
    else 
        std::cout << "null" << std::endl;

    printRightBrace();
}

void ASTDump::visitAssignmentExpr(const AssignmentExpr* node) {
    printType("AssignmentExpr");

    printTab();
    std::cout << "left ";
    printLeftBrace();
    node->getLHS()->accept(this);
    printRightBrace();

    printTab();
    std::cout << "operator: " << node->getOp() << std::endl;

    printTab();
    std::cout << "right ";
    printLeftBrace();
    node->getRHS()->accept(this);
    printRightBrace();
    printRightBrace();
}

void ASTDump::visitLabelStmt(const LabelStmt* node) {
    printType("LabelStmt");

    printTab();
    std::cout << "body ";
    printLeftBrace();
    node->getBody()->accept(this);
    printRightBrace();

    printTab();
    std::cout << "label ";
    printLeftBrace();
    node->getLabel()->accept(this);
    printRightBrace();

    printRightBrace();
}

void ASTDump::visitContinueStmt(const ContinueStmt* node) {
    printType("ContinueStmt");

    printTab();
    std::cout << "label: ";
    if (node->getLabel()) {
        printLeftBrace();
        node->getLabel()->accept(this);
        printRightBrace();
    }
    else
        std::cout << "null";
    printRightBrace();
}

void ASTDump::visitBreakStmt(const BreakStmt* node) {
    printType("BreakStmt");

    printTab();
    std::cout << "label: ";
    if (node->getLabel()) {
        printLeftBrace();
        node->getLabel()->accept(this);
        printRightBrace();
    }
    else
        std::cout << "null";
    printRightBrace();
}

void ASTDump::visitForStmt(const ForStmt* node) {
    printType("ForStmt");

    printTab();
    std::cout << "init ";
    printLeftBrace();
    node->getInit()->accept(this);
    printRightBrace();

    printTab();
    std::cout << "test ";
    printLeftBrace();
    node->getTest()->accept(this);
    printRightBrace();

    printTab();
    std::cout << "update ";
    printLeftBrace();
    node->getUpdate()->accept(this);
    printRightBrace();

    printTab();
    std::cout << "body ";
    printLeftBrace();
    node->getBody()->accept(this);
    printRightBrace();

    printRightBrace();
}

void ASTDump::visitForInStmt(const ForInStmt* node) {

}

void ASTDump::visitWhileStmt(const WhileStmt *node) {
    printType("WhileStmt");

    printTab();
    std::cout << "test ";
    printLeftBrace();
    node->getTest()->accept(this);
    printRightBrace();

    printTab();
    std::cout << "body ";
    printLeftBrace();
    node->getBody()->accept(this);
    printRightBrace();

    printRightBrace();
}

void ASTDump::visitDoWhileStmt(const DoWhileStmt *node) {
    printType("DoWhileStmt");

    printTab();
    std::cout << "body ";
    printLeftBrace();
    node->getBody()->accept(this);
    printRightBrace();

    printTab();
    std::cout << "test ";
    printLeftBrace();
    node->getTest()->accept(this);
    printRightBrace();

    printRightBrace();
}

void ASTDump::visitUnaryExpr(const UnaryExpr* node) {
    printType("UnaryExpr");

    printTab();
    std::cout << "operator: " << node->getOp() << std::endl;

    printTab();
    std::cout << std::boolalpha << "prefix: " << node->getPrefix() << std::endl;

    printTab();
    std::cout << "argument:";
    printLeftBrace();
    node->getArgument()->accept(this);
    printRightBrace();

    printRightBrace();
}

void ASTDump::visitArrayExpr(const ArrayExpr* node) {

}

void ASTDump::printTab() const {
    for (int i = 0; i < deep; ++i) {
        std::cout << "|-";
    }
}

void ASTDump::printLeftBrace() {
    std::cout << "{" << std::endl;
    ++deep;
}

void ASTDump::printRightBrace() {
    --deep;
    printTab();
    std::cout << "}" << std::endl;
}

void ASTDump::printLeftBracket() {
    std::cout << "[" << std::endl;
    ++deep;
}

void ASTDump::printRightBracket() {
    --deep;
    printTab();
    std::cout << "]" << std::endl;
}

void ASTDump::printType(const std::string& type) {
    printTab();
    std::cout << type << " ";
    printLeftBrace();
    printTab();
    std::cout << "type: " << type << std::endl;
}

void ASTDump::visit(Expr* e) {
    // switch (e->getType()) {
    //     case ASTType::AST_VARIABLEDECLARATION:
    //         visitVariableDeclaration(dynamic_cast<ASTVariableDeclaration*>(e));
    //         break;
    //     case ASTType::AST_VARIABLEDECLARATOR:
    //         visitVariableDeclarator(dynamic_cast<ASTVariableDeclarator*>(e));
    //         break;
    //     case ASTType::AST_EXPR_IDENTIFIER:
    //         visitIdentifier(dynamic_cast<Identifier*>(e));
    //         break;
    //     case ASTType::AST_EXPR_MEMBER:
    //         visitMemberExpr(dynamic_cast<MemberExpr*>(e));
    //         break;
    //     case ASTType::AST_EXPR_LITERAL:
    //         visitLiteral(dynamic_cast<Literal*>(e));
    //         break;
    //     case ASTType::AST_EXPR_CALL:
    //         visitCallExpr(dynamic_cast<CallExpr*>(e));
    //         break;
    //     case ASTType::AST_EXPR_BINARY:
    //         visitBinaryExpr(dynamic_cast<BinaryExpr*>(e));
    //         break;
    //     case ASTType::AST_FUNCTIONDECLARATION:
    //         visitFunctionDeclaration(dynamic_cast<ASTFunctionDeclaration*>(e));
    //         break;
    //     case ASTType::AST_STMT_BLOCK:
    //         visitBlockStmt(dynamic_cast<BlockStmt*>(e));
    //         break;
    //     case ASTType::AST_STMT_RETURN:
    //         visitReturnStmt(dynamic_cast<ReturnStmt*>(e));
    //         break;
    //     case ASTType::AST_STMT_IF:
    //         visitIfStmt(dynamic_cast<IfStmt*>(e));
    //         break;
    //     case ASTType::AST_EXPR_ASSIGNMENT:
    //         visitAssignmentExpr(dynamic_cast<AssignmentExpr*>(e));
    //         break;
    //     case ASTType::AST_STMT_LABEL:
    //         visitLabelStmt(dynamic_cast<LabelStmt*>(e));
    //         break;
    //     case ASTType::AST_STMT_FOR:
    //         visitForStmt(dynamic_cast<ForStmt*>(e));
    //         break;
    //     case ASTType::AST_STMT_FORIN:
    //         visitForInStmt(dynamic_cast<ForInStmt*>(e));
    //         break;
    //     case ASTType::AST_STMT_WHILE:
    //         visitWhileStmt(dynamic_cast<WhileStmt*>(e));
    //         break;
    //     case ASTType::AST_STMT_DOWHILE:
    //         visitDoWhileStmt(dynamic_cast<DoWhileStmt*>(e));
    //         break;
    //     case ASTType::AST_STMT_CONTINUE:
    //         visitContinueStmt(dynamic_cast<ContinueStmt*>(e));
    //         break;
    //     case ASTType::AST_EXPR_UNARY:
    //         visitUnaryExpr(dynamic_cast<UnaryExpr*>(e));
    //         break;
    //     default: break;;
    // }
}
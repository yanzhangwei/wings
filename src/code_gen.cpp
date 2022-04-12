#include "code_gen.h"
#include "ast.h"
#include "ast_expr.h"
#include "ast_decl.h"
#include "ast_expr_literal.h"
#include "ast_stmt.h"
#include "ast_visitor.h"
#include "value.h"
#include <iostream>
#include <string>
#include <variant>

void CodeGen::visitASTProgram(const ASTProgram* node) {
    for (const auto& e: node->getBody())
        e->accept(this);
}

void CodeGen::visitVariableDeclaration(const ASTVariableDeclaration *node) {
    for (const auto& e: node->getDeclarations())
        e->accept(this);
}

void CodeGen::visitVariableDeclarator(const ASTVariableDeclarator *node) {
    table->insert(node->getId()->getName(), nullptr);
    node->getInit()->accept(this);
    node->getId()->accept(this);
    fun->getChunk()->pushCode(OP_SET);
}

void CodeGen::visitIdentifier(const Identifier *node) {
    fun->getChunk()->pushConstant(new Value(node->getName()));
}

void CodeGen::visitMemberExpr(const MemberExpr* node) {
    int a = 0;
}

void CodeGen::visitLiteral(const Literal *node) {
    auto data = node->getValue();
    if (std::holds_alternative<double>(data)) {
        fun->getChunk()->pushConstant(new Value(std::get<double>(data)));
    } else if (std::holds_alternative<bool>(data)) {
        fun->getChunk()->pushConstant(new Value(std::get<bool>(data)));
    } else if (std::holds_alternative<std::string>(data)) {
        fun->getChunk()->pushConstant(new Value(std::get<std::string>(data)));
    } else {
        fun->getChunk()->pushConstant(new Value(std::monostate()));
    }
}

void CodeGen::visitCallExpr(const CallExpr *node) {
    node->getCallee()->accept(this);
    fun->getChunk()->pushCode(OP_GET);

    for (const auto& arg: node->getArgs())
        arg->accept(this);
    fun->getChunk()->pushCode(OP_CALL);
    fun->getChunk()->pushCode(node->getArgs().size());
}

void CodeGen::visitBinaryExpr(const BinaryExpr *node) {
    node->getLHS()->accept(this);
    if (node->getLHS()->getType() == ASTType::AST_EXPR_IDENTIFIER)
        fun->getChunk()->pushCode(OP_GET);
    node->getRHS()->accept(this);
    if (node->getRHS()->getType() == ASTType::AST_EXPR_IDENTIFIER)
        fun->getChunk()->pushCode(OP_GET);
    std::string op = node->getOp();
    if (op == "+") {
        fun->getChunk()->pushCode(OP_PLUS);
    } else if (op == "-") {
        fun->getChunk()->pushCode(OP_MINUS);
    } else if (op == "*") {
        fun->getChunk()->pushCode(OP_MUL);
    } else if (op == "/") {
        fun->getChunk()->pushCode(OP_DIV);
    } else if (op == "%") {
        fun->getChunk()->pushCode(OP_MOD);    
    }
}

void CodeGen::visitFunctionDeclaration(const ASTFunctionDeclaration *node) {
    table->insert(node->getId()->getName(), nullptr);
    table->initialize();
    // table->insert(const std::string &str, Value *l)
    for (const auto& param: node->getParams()) {
        table->insert(param->getName(), nullptr);
    }
    VMFunction* vFun = new VMFunction(node->getParams(), node->getId(), fun);
    fun->getChunk()->pushConstant(new Value(vFun));
    fun->getChunk()->pushCode(OP_SET);
    fun = vFun;
    node->getBlock()->accept(this);
    fun = vFun->getParent();
    table->finalize();
}

void CodeGen::visitBlockStmt(const BlockStmt* node) {
    for (const auto& e: node->getBody())
        e->accept(this);
}
void CodeGen::visitReturnStmt(const ReturnStmt* node) {
    node->getArgument()->accept(this);
    fun->getChunk()->pushCode(OP_RETURN);
}
void CodeGen::visitIfStmt(const IfStmt* node) {

}
void CodeGen::visitAssignmentExpr(const AssignmentExpr* node) {
    node->getRHS()->accept(this);
    if (node->getRHS()->getType() == ASTType::AST_EXPR_IDENTIFIER)
        fun->getChunk()->pushCode(OP_GET);
    if (node->getOp() == "+=") {
        node->getLHS()->accept(this);
        fun->getChunk()->pushCode(OP_GET);
        fun->getChunk()->pushCode(OP_PLUS);
    }
    node->getLHS()->accept(this);
    fun->getChunk()->pushCode(OP_SET);
}
void CodeGen::visitLabelStmt(const LabelStmt* node) {

}
void CodeGen::visitContinueStmt(const ContinueStmt* node) {

}
void CodeGen::visitBreakStmt(const BreakStmt* node) {

}
void CodeGen::visitForStmt(const ForStmt* node) {

}
void CodeGen::visitForInStmt(const ForInStmt* node) {

}
void CodeGen::visitWhileStmt(const WhileStmt* node) {

}
void CodeGen::visitDoWhileStmt(const DoWhileStmt* node) {

}
void CodeGen::visitUnaryExpr(const UnaryExpr* node) {

}
void CodeGen::visitArrayExpr(const ArrayExpr* node) {

}

void CodeGen::dumpCallFrames(const VMFunction* fun) const {
    // for (const auto& frame: frames) {
        std::cout << fun->getName()->getName() << std::endl;
        std::cout << "===================" << std::endl;
        auto chunk = fun->getChunk();

        auto code = chunk->getCode();
        
        for (int i = 0; i < code.size(); ++i) {
            std::cout << chunk->getCodeStringByInde(code[i]) << "\t";
            if (code[i] == OP_CONSTANT) {
                ++i;
                chunk->getConstantByIndex(i)->print();
            }
            std::cout << std::endl;
        }
        std::cout << "===================" << std::endl << std::endl;;
    // }

}
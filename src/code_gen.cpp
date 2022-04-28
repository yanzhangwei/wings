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
#include "vm_instance.h"

void CodeGen::visitASTProgram(const ASTProgram* node) {
    for (const auto& e: node->getBody())
        e->accept(this);
}

void CodeGen::visitVariableDeclaration(const ASTVariableDeclaration *node) {
    for (const auto& e: node->getDeclarations())
        e->accept(this);
}

void CodeGen::visitVariableDeclarator(const ASTVariableDeclarator *node) {
    // table->insert(node->getId()->getName(), nullptr);
    node->getInit()->accept(this);
    if (node->getInit()->getType() == ASTType::AST_EXPR_IDENTIFIER)
        frame->fun->getChunk()->pushCode(OP_GET);
    node->getId()->accept(this);
    frame->fun->getChunk()->pushCode(OP_SET);
    frame->insert(node->getId()->getName(), nullptr);
}

void CodeGen::visitIdentifier(const Identifier *node) {
    frame->fun->getChunk()->pushConstant(new Value(node->getName()));
}

void CodeGen::visitMemberExpr(const MemberExpr* node) {
    node->getObject()->accept(this);
    node->getProperty()->accept(this);
    frame->fun->getChunk()->pushCode(node->getIsLeftHand() ? OP_SET_PROPS : OP_GET_PROPS);
}

void CodeGen::visitLiteral(const Literal *node) {
    auto data = node->getValue();
    if (std::holds_alternative<double>(data)) {
        frame->fun->getChunk()->pushConstant(new Value(std::get<double>(data)));
    } else if (std::holds_alternative<bool>(data)) {
        frame->fun->getChunk()->pushConstant(new Value(std::get<bool>(data)));
    } else if (std::holds_alternative<std::string>(data)) {
        frame->fun->getChunk()->pushConstant(new Value(std::get<std::string>(data)));
    } else {
        frame->fun->getChunk()->pushConstant(new Value(std::monostate()));
    }
}

void CodeGen::visitCallExpr(const CallExpr *node) {
    node->getCallee()->accept(this);
    frame->fun->getChunk()->pushCode(OP_GET);

    for (const auto& arg: node->getArgs())
        arg->accept(this);
    frame->fun->getChunk()->pushCode(OP_CALL);
    frame->fun->getChunk()->pushCode(node->getArgs().size());
}

void CodeGen::visitBinaryExpr(const BinaryExpr *node) {
    node->getLHS()->accept(this);
    if (node->getLHS()->getType() == ASTType::AST_EXPR_IDENTIFIER)
        frame->fun->getChunk()->pushCode(OP_GET);
    node->getRHS()->accept(this);
    if (node->getRHS()->getType() == ASTType::AST_EXPR_IDENTIFIER)
        frame->fun->getChunk()->pushCode(OP_GET);
    std::string op = node->getOp();
    if (op == "+") {
        frame->fun->getChunk()->pushCode(OP_PLUS);
    } else if (op == "-") {
        frame->fun->getChunk()->pushCode(OP_MINUS);
    } else if (op == "*") {
        frame->fun->getChunk()->pushCode(OP_MUL);
    } else if (op == "/") {
        frame->fun->getChunk()->pushCode(OP_DIV);
    } else if (op == "%") {
        frame->fun->getChunk()->pushCode(OP_MOD);    
    }
}

void CodeGen::visitFunctionDeclaration(const ASTFunctionDeclaration *node) {
    VMFunction* vFun = new VMFunction(node->getParams(), node->getId());
    CallFrame *newFrame = new CallFrame(vFun, frame);
    frame->fun->getChunk()->pushConstant(new Value(vFun));
    frame->fun->getChunk()->pushCode(OP_SET);
    // fun = vFun;
    frame = newFrame;
    node->getBlock()->accept(this);
    // if (!dynamic_cast<BlockStmt*>(node->getBlock())->getReturnStmtStatus()) {
    //     frame->fun->getChunk()->pushCode(OP_RETURN);
    // }
    // fun = vFun->getParent();
    frame = newFrame->prev;
    frame->insert(node->getId()->getName(), nullptr);
}

void CodeGen::visitBlockStmt(const BlockStmt* node) {
    for (const auto& e: node->getBody())
        e->accept(this);
}

void CodeGen::visitReturnStmt(const ReturnStmt* node) {
    node->getArgument()->accept(this);
    frame->fun->getChunk()->pushCode(OP_RETURN);
}

void CodeGen::visitIfStmt(const IfStmt* node) {

}

void CodeGen::visitAssignmentExpr(const AssignmentExpr* node) {
    node->getRHS()->accept(this);
    if (node->getRHS()->getType() == ASTType::AST_EXPR_IDENTIFIER)
        frame->fun->getChunk()->pushCode(OP_GET);
    if (node->getOp() == "+=") {
        node->getLHS()->accept(this);
        frame->fun->getChunk()->pushCode(OP_GET);
        frame->fun->getChunk()->pushCode(OP_PLUS);
    }
    node->getLHS()->accept(this);
    if (node->getLHS()->getType() != ASTType::AST_EXPR_MEMBER) 
        frame->fun->getChunk()->pushCode(OP_SET);
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

void CodeGen::visitThisExpr(const ThisExpr *node) {
    frame->fun->getChunk()->pushCode(OP_THIS);
}

void CodeGen::visitNewExpr(const NewExpr *node) {
    node->getCallee()->accept(this);
    frame->fun->getChunk()->pushCode(OP_GET);
    for (const auto& arg: node->getArguments()) {
        arg->accept(this);
    }
    frame->fun->getChunk()->pushCode(OP_CALL);
    frame->fun->getChunk()->pushCode(node->getArguments().size());

    frame->fun->getChunk()->pushCode(OP_INSTANCE);
    frame->fun->getChunk()->pushCode(node->getArguments().size());
}

void CodeGen::visitFunctionExpr(const FunctionExpr *node) {
    
}

void CodeGen::visitProperty(const Property *node) {
    node->getKey()->accept(this);
    node->getValue()->accept(this);
    frame->fun->getChunk()->pushCode(OP_SET_OBJ_LITERAL_PROPS);
}

void CodeGen::visitObjectExpr(const ObjectExpr* node) {
    for (const auto& p: node->getProperties())
        p->accept(this);
}

void CodeGen::dumpCallFrames(const VMFunction* fun) const {
    // for (const auto& frame: frames) {
        std::cout << fun->getName()->getName() << std::endl;
        std::cout << "===================" << std::endl;
        auto chunk = fun->getChunk();

        auto code = chunk->getCode();
        
        for (int i = 0; i < code.size(); ++i) {
            std::cout << chunk->getCodeStringByInde(code[i]) << "\t";
            if (code[i] == OP_CALL) {
                ++i;
            } else if (code[i] == OP_CONSTANT) {
                ++i;
                chunk->getConstantByIndex(i)->print();
            }
            std::cout << std::endl;
        }
        std::cout << "===================" << std::endl << std::endl;;
    // }

}
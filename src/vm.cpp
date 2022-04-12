#include "vm.h"
#include "ast.h"
#include "ast_expr.h"
#include "ast_decl.h"
#include "ast_stmt.h"
#include "ast_visitor.h"
#include "token.h"
#include "value.h"
#include "vm_function.h"
#include <string>
#include <variant>


VM::VM(const ASTProgram* ast): 
    codegen(new CodeGen)/*, table(new Symtab)*/ {
    codegen->visitASTProgram(ast);
    frames.emplace_back(new CallFrame(codegen->getFunction()));
    // auto asd = codegen->getSymtab();
    
    int a = 0;
}

VM::~VM() {
    // delete codegen;
}

void VM::run() {

    auto frame = frames.back();
    codegen->dumpCallFrames(frame->fun);
    
    auto readCode = [&]() { 
        return frame->fun->getChunk()->getCode()[frame->ip++]; 
    };

    auto readConstant = [&]() {
        // auto chunk = frame->fun->getChunk();
        return frame->fun->getChunk()->getConstantByIndex(frame->ip++);
    };

    while (1) {
        int instruction = readCode();
        switch (instruction) {
            case OP_CONSTANT:
                stack.emplace_back(readConstant());
                break;
            case OP_PLUS:
            case OP_MINUS:
            case OP_MUL:
            case OP_DIV:
            case OP_MOD:
                binaryOp(instruction);
                break;
            case OP_GET:
                getTable();
                break;
            case OP_SET:
                setTable();
                break;
            case OP_CALL: {
                int count = readCode();
                call(peek(count), count);
                frame = frames.back();
                break;
            }
            case OP_RETURN: {
                Value* result = pop();

                while (peek(0) != frame->slots) {
                    pop();
                }
                pop();
                if (std::holds_alternative<std::string>(result->getData())) {
                    if (auto val = frame->fun->lookup(std::get<std::string>(result->getData()))) {
                        stack.emplace_back(val);
                    } else {
                        stack.emplace_back(result);
                    }
                } else {
                    stack.emplace_back(result);
                }
                frames.pop_back();
                // table->finalize();
                frame = frames.back();

                break;
            }
            default: 
                // table->finalize();
                auto z = frame->fun->lookup("z");
                return;
        }
    }
}

void VM::binaryOp(const int& op) {
    if (peek(0)->isNumber() && peek(1)->isNumber()) {
        auto b = std::get<double>(pop()->getData());
        auto a = std::get<double>(pop()->getData());

        switch (op) {
            case OP_PLUS:
                stack.emplace_back(new Value(a + b));
                break;
            case OP_MINUS:
                stack.emplace_back(new Value(a - b));
                break;
            case OP_MUL:
                stack.emplace_back(new Value(a * b));
                break;
            case OP_DIV:
                stack.emplace_back(new Value(a / b));
                break;
            case OP_MOD:
                // stack.emplace_back(a % b);
                break;
            default: break;
        }
    }
}

Value* VM::pop() {
    Value* tmp = stack.back();
    stack.pop_back();
    return tmp;
}

Value* VM::peek(const int& distance) const {
    return stack[stack.size() - 1 - distance];
}

void VM::setTable() {
    auto data = peek(0)->getData();
    if (auto value = std::get_if<std::string>(&data)) {
        frames.back()->fun->insert(std::get<std::string>(peek(0)->getData()), peek(1));
        pop();
        pop();
    } else if (auto value = std::get_if<VMFunction*>(&data)) {
        frames.back()->fun->insert((*value)->getName()->getName(), new Value(*value));
        pop();
    }
}

void VM::getTable() {
    Value* tmp = frames.back()->fun->lookup(std::get<std::string>(peek(0)->getData()));
    pop();
    stack.emplace_back(tmp);
}

void VM::call(Value* val, const int& args) {
    if (!std::holds_alternative<VMFunction*>(val->getData()))
        return;
    VMFunction* fun = std::get<VMFunction*>(val->getData());
    // if (fun->getName()->getName() == "inner") {
    //     auto asd = fun->lookup("y");
    // }

    // table->initialize();

    for (int i = 0; i < fun->getParams().size(); ++i) {
        fun->insert(fun->getParams()[i]->getName(), peek(args - i - 1));
    }

    CallFrame* frame = new CallFrame(fun);
    frame->slots = stack[stack.size() - 1 -args];
    frames.emplace_back(frame);

    codegen->dumpCallFrames(frame->fun);
}

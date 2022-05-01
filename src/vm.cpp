#include "vm.h"
#include "ast.h"
#include "ast_expr.h"
#include "ast_decl.h"
#include "ast_stmt.h"
#include "ast_visitor.h"
#include "token.h"
#include "value.h"
#include "vm_function.h"
#include "vm_instance.h"
#include "vm_obj_literal.h"
#include <optional>
#include <string>
#include <variant>


VM::VM(const ASTProgram* ast): 
    codegen(new CodeGen)/*, table(new Symtab)*/ {
    codegen->visitASTProgram(ast);
    frame = codegen->getFrame();
    frames.emplace_back(frame);
    // frames.emplace_back(new CallFrame(codegen->getFunction()));
}

VM::~VM() {
    // delete codegen;
}

void VM::run() {
    codegen->dumpCallFrames(frame->fun);
    
    auto readCode = [&]() {
        if (frame->fun->getChunk()->getCode().size() <= frame->ip && frames.size() > 1) {
            frames.pop_back();
            frame = frames.back();
        }
        return frame->fun->getChunk()->getCode()[frame->ip++];
    };

    auto readConstant = [&]() {
        return frame->fun->getChunk()->getConstantByIndex(frame->ip++);
    };

    while (1) {
        int instruction = readCode();
        switch (instruction) {
            case OP_DUP:
                stack.emplace_back(stack.back());
                break;
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
                break;
            }
            case OP_INSTANCE: {
                int count = readCode();
                auto fun = std::get<VMFunction*>(peek(count)->getData());
                auto instance = fun->getInstance();
                while (count--) {
                    stack.pop_back();
                }
                stack.pop_back();
                stack.emplace_back(new Value(instance));
                break;
            }
            case OP_GET_PROPS: {
                auto instance = peek(1)->getData();
                if (std::holds_alternative<std::string>(instance)) {
                    std::string fields = std::get<std::string>(instance);
                    if (fields.compare("this") == 0) {
                        std::string v = frame->fun->getCaller();
                        VMObjLiteral* objLiteral = std::get<VMObjLiteral*>(frame->lookup(v)->getData());
                        // stack.emplace_back(objLiteral->find(peek(0)));
                        std::string filed = std::get<std::string>(peek(0)->getData());
                        pop();
                        pop();
                        stack.emplace_back(objLiteral->find(filed));
                    } else {
                        Value* v = frame->lookup(fields);
                        if (std::holds_alternative<VMInstance*>(v->getData())) {
                            VMInstance* vi = std::get<VMInstance*>(v->getData());
                            std::string filed = std::get<std::string>(peek(0)->getData());
                            pop();
                            pop();
                            stack.emplace_back(vi->find(filed));
                        } else if (std::holds_alternative<VMObjLiteral*>(v->getData())) {
                            VMObjLiteral* vo = std::get<VMObjLiteral*>(v->getData());
                            std::string filed = std::get<std::string>(peek(0)->getData());
                            pop();
                            pop();
                            stack.emplace_back(vo->find(filed));
                        }
                    }                  
                }
                break;
            }
            case OP_SET_PROPS: {
                auto instance = peek(1)->getData();
                if (std::get<std::string>(instance) == "this") {
                    frame->fun->insertKlass(std::get<std::string>(peek(0)->getData()), peek(2));
                } else {
                    VMInstance* v = std::get<VMInstance*>(frame->lookup(std::get<std::string>(instance))->getData());
                    std::string fields = std::get<std::string>(peek(0)->getData());
                    pop();
                    pop();
                    v->insert(fields, peek(0));
                    pop();
                }
                pop();
                pop();
                pop();
                break;
            }
            case OP_GET_OBJ_LITERAL_PROPS: {
                // stack.emplace_back(new Value(new VM))
                break;
            }
            case OP_SET_OBJ_LITERAL_PROPS: {
                Value* v = peek(2);
                if (v && std::holds_alternative<VMObjLiteral*>(v->getData())) {
                    VMObjLiteral* objLiteral = std::get<VMObjLiteral*>(v->getData());
                    objLiteral->insert(std::get<std::string>(peek(1)->getData()), peek(0));
                    pop();
                    pop();
                } else {
                    VMObjLiteral* objLiteral = new VMObjLiteral();
                    objLiteral->insert(std::get<std::string>(peek(1)->getData()), peek(0));
                    pop();
                    pop();
                    stack.emplace_back(new Value(objLiteral));
                }
                break;
            }
            case OP_THIS: {
                stack.emplace_back(new Value(std::string("this")));
                break;
            }
            case OP_RETURN: {
                Value* result = pop();

                while (peek(0) != frame->slots) {
                    pop();
                }
                pop();
                if (std::holds_alternative<std::string>(result->getData())) {
                    if (auto val = frame->lookup(std::get<std::string>(result->getData()))) {
                        stack.emplace_back(val);
                    } else {
                        stack.emplace_back(result);
                    }
                } else {
                    stack.emplace_back(result);
                }
                frames.pop_back();
                frame = frames.back();

                break;
            }
            default: 
                auto z = frame->lookup("z");
                if (std::holds_alternative<VMInstance*>(z->getData())) {
                    int a = 0;
                }
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
    return stack.size() > distance ? stack[stack.size() - 1 - distance] : nullptr;
}

void VM::setTable() {
    auto data = peek(0)->getData();
    if (auto value = std::get_if<std::string>(&data)) {
        // frames.back()->insert(std::get<std::string>(peek(0)->getData()), peek(1));
        frame->insert(std::get<std::string>(peek(0)->getData()), peek(1));
        pop();
        pop();
    } else if (auto value = std::get_if<VMFunction*>(&data)) {
        // frames.back()->insert((*value)->getName()->getName(), new Value(*value));
        frame->insert((*value)->getName()->getName(), new Value(*value));
        pop();
    }
}

void VM::getTable() {
    // Value* tmp = frames.back()->lookup(std::get<std::string>(peek(0)->getData()));
    Value* tmp = frame->lookup(std::get<std::string>(peek(0)->getData()));
    pop();
    stack.emplace_back(tmp);
}

void VM::call(Value* val, const int& args) {
    if (!std::holds_alternative<VMFunction*>(val->getData()))
        return;
    VMFunction* fun = std::get<VMFunction*>(val->getData());
    fun->setCaller(std::get<std::string>(peek(args + 1)->getData()));

    // CallFrame* frame = new CallFrame(fun);
    // frame->slots = stack[stack.size() - 1 -args];
    // frames.emplace_back(frame);
    frame = fun->getFrame();
    frame->slots = stack[stack.size() - 1 -args - 1];

    for (int i = 0; i < fun->getParams().size(); ++i) {
        frame->insert(fun->getParams()[i]->getName(), peek(args - i - 1));
    }
    frames.emplace_back(frame);
    codegen->dumpCallFrames(frame->fun);
}

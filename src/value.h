#ifndef _VALUE_H
#define _VALUE_H
#pragma once

#include <iostream>
#include <string>
#include <variant>

class VMFunction;
class VMInstance;
class VMObjLiteral;

class Value {
public:
    using value = std::variant<
        double, 
        bool, 
        std::string, 
        std::monostate, 
        VMFunction*,
        VMInstance*,
        VMObjLiteral*
    >;
    Value(const value& v): data(v) {}
    value getData() const { return data; }
    bool isNumber() const { return std::holds_alternative<double>(data); }
    void print() const ;
private:
    value data;
};
#endif
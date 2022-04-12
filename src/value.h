#ifndef _VALUE_H
#define _VALUE_H
#include <iostream>
#pragma once

#include <string>
#include <variant>

class VMFunction;

class Value {
public:
    using value = std::variant<
        double, 
        bool, 
        std::string, 
        std::monostate, 
        VMFunction*
    >;
    Value(const value& v): data(v) {}
    value getData() const { return data; }
    bool isNumber() const { return std::holds_alternative<double>(data); }
    void print() const ;
private:
    value data;
};
#endif
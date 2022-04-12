#ifndef _SYMTAB_H
#define _SYMTAB_H
#pragma once

#include <string>
#include <unordered_map>
#include <optional>
#include "value.h"


class Symtab {
public:
    Symtab(): currentTable(nullptr) {}
    void insert(const std::string& str, Value* l);
    void initialize();
    void finalize();
    Value* lookup(const std::string& key) const;
private:
    struct table {
        std::unordered_map<std::string, Value*> hashmap;
        table* prev;
    };
    table* currentTable;
};
#endif
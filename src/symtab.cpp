#include "symtab.h"
#include <optional>

void Symtab::insert(const std::string& str, Value *l) {
    if (!currentTable) initialize();
    currentTable->hashmap.insert_or_assign(str, l);
}

void Symtab::initialize() {
    table* tmp = new table;
    if (currentTable) 
        tmp->prev = currentTable;
    currentTable = tmp;
}

void Symtab::finalize() {
    currentTable = currentTable->prev;
}

Value* Symtab::lookup(const std::string &key) const {
    auto found = currentTable->hashmap.find(key);
    table* tmp = currentTable;
    while (tmp) {
        auto found = tmp->hashmap.find(key);
        if(found != tmp->hashmap.end())
            return found->second;
        tmp = tmp->prev;
    }
    return nullptr;
}
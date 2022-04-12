#include "value.h"
#include "vm_function.h"

void Value::print() const {
    if (auto val = std::get_if<double>(&data)) {
        std::cout << *val;
    } else if (auto val = std::get_if<std::string>(&data)) {
        std::cout << *val;
    } else if (auto val = std::get_if<bool>(&data)) {
        std::cout << *val;
    } else if (auto val = std::get_if<VMFunction*>(&data)) {
        std::cout << "<fun " << (*val)->getName()->getName() << ">";
    } else {
        std::cout << "null";
    }
}
#include <iostream>
#include <cstring>
#include "scanner.h"
#include "parser.h"
#include "ast_decl.h"
#include "ast_dump.h"
#include "vm.h"

int main(int argc, char* argv[]) {
    if (argc == 1) return 0;
    Parser parser(new Scanner(argv[1]));
    auto p = parser.parseProgram();
    if (argv[2] && std::memcmp(argv[2], "-ast-dump", 9) == 0) {
        ASTDump dump;
        dump.visitASTProgram(p);
    }
    VM* vm = new VM(p);
    vm->run();
    return 0;
}
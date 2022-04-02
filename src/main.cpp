#include <iostream>
#include <limits>
#include "scanner.h"
#include "parser.h"
#include "ast_decl.h"
#include "ast_dump.h"

int main(int argc, char* argv[]) {
    if (argc == 1) return 0;
    Parser parser(new Scanner(argv[1]));
    auto p = parser.parseProgram();
    // std::cout << p << std::endl;
    ASTDump dump;
    dump.visitASTProgram(p);
    return 0;
}
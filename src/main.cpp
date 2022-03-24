#include <iostream>
#include "scanner.h"
#include "parser.h"

int main(int argc, char* argv[]) {
    if (argc == 1) return 0;
    Parser parser(new Scanner(argv[1]));
    return 0;
}
#include "../Lexer.hpp"
#include "../Token.hpp"
#include "test_lexer.hpp"

#include <iostream>

void test_lexing() {

    std::string code = "func main() { var x := 10; print(\"Hello\"); }";

    Lexer lexer(code);

    std::vector<Token> tokens = lexer.scanTokens();

    for (Token token : tokens) {
        std::cout << token.toString() << "\n";
    }
}
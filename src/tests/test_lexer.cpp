
#include "test_lexer.hpp"

#include <iostream>

void test_lexing() {

  std::string code =
      "func main(ref int my_var) { var x := 10; print(\"Hello world\"); }";

  Lexer lexer(code);

  std::vector<Token> tokens = lexer.scanTokens();

  for (Token token : tokens) {
    std::cout << token << "\n";
  }

  std::cout << "\nTesting next set\n";

  code = "owned ref .. ..= . ++ -- += -=, atomic int i /= *=+% _ _**";

  Lexer lexer2(code);
  tokens = lexer2.scanTokens();

  for (Token token : tokens) {
    std::cout << token << "\n";
  }
}

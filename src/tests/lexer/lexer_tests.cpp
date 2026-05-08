#include "lexer/Lexer.hpp"
#include "lexer/Token.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("Lexer tokenizes integers", "[lexer]") {
  Lexer lexer("42");
  auto tokens = lexer.scan_tokens();

  REQUIRE(tokens.size() == 2);
  REQUIRE(tokens[0].type == TokenType::INT_LITERAL);
  REQUIRE(tokens[0].lexeme == "42");
}

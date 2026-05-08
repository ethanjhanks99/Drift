#include "lexer/Lexer.hpp"
#include "lexer/Token.hpp"
#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators.hpp>

TEST_CASE("Lexer tokenizes integer literals", "[lexer]") {
  Lexer lexer("42");
  auto tokens = lexer.scan_tokens();

  REQUIRE(tokens[0].type == TokenType::INT_LITERAL);
  REQUIRE(tokens[0].lexeme == "42");
}

TEST_CASE("Lexer tokenizes float literals", "[lexer]") {
  Lexer lexer("5248.2641");
  auto tokens = lexer.scan_tokens();

  REQUIRE(tokens[0].type == TokenType::FLOAT_LITERAL);
  REQUIRE(tokens[0].lexeme == "5248.2641");
}

TEST_CASE("Lexer tokenizes string literals", "[lexer]") {
  Lexer lexer("\"Hello World\"");
  auto tokens = lexer.scan_tokens();

  REQUIRE(tokens[0].type == TokenType::STR_LIT);
  REQUIRE(tokens[0].lexeme == "Hello World");
}

TEST_CASE("Lexer tokenizes single character tokens") {
  auto chars = GENERATE("(", ")", "[", "]", "{", "}", ";", ",", ".", ":", "?");
  auto types =
      GENERATE(TokenType::LPAREN, TokenType::RPAREN, TokenType::LBRACKET,
               TokenType::RBRACKET, TokenType::LBRACE, TokenType::RBRACE,
               TokenType::SEMICOLON, TokenType::COMMA, TokenType::DOT,
               TokenType::COLON, TokenType::QMARK);

  Lexer lexer(chars);
  auto tokens = lexer.scan_tokens();

  REQUIRE(tokens[0].type == types);
}

TEST_CASE("Lexer tokenizes types", "[lexer]") {
  SECTION("Integer Types") {
    SECTION("int and uint") {
      auto types = GENERATE("int", "uint");
      auto token_types = GENERATE(TokenType::I32, TokenType::U32);
      Lexer lexer(types);
      auto tokens = lexer.scan_tokens();

      REQUIRE(tokens[0].type == token_types);
    }

    SECTION("Signed Integers") {
      auto types = GENERATE("i8", "i16", "i32", "i64");
      auto token_types = GENERATE(TokenType::I8, TokenType::I16, TokenType::I32,
                                  TokenType::I64);
      Lexer lexer(types);
      auto tokens = lexer.scan_tokens();

      REQUIRE(tokens[0].type == token_types);
    }

    SECTION("Unsigned Integers") {
      auto types = GENERATE("u8", "u16", "u32", "u64");
      auto token_types = GENERATE(TokenType::U8, TokenType::U16, TokenType::U32,
                                  TokenType::U64);
      Lexer lexer(types);
      auto tokens = lexer.scan_tokens();

      REQUIRE(tokens[0].type == token_types);
    }
  }

  SECTION("Float") {
    Lexer lexer("float");
    auto tokens = lexer.scan_tokens();

    REQUIRE(tokens[0].type == TokenType::FLOAT);
  }

  SECTION("Void") {
    Lexer lexer("void");
    auto tokens = lexer.scan_tokens();

    REQUIRE(tokens[0].type == TokenType::VOID);
  }

  SECTION("Boolean") {
    Lexer lexer("bool");
    auto tokens = lexer.scan_tokens();

    REQUIRE(tokens[0].type == TokenType::BOOL);
  }

  SECTION("String") {
    Lexer lexer("string");
    auto tokens = lexer.scan_tokens();

    REQUIRE(tokens[0].type == TokenType::STRING);
  }

  SECTION("Character") {
    Lexer lexer("char");
    auto tokens = lexer.scan_tokens();

    REQUIRE(tokens[0].type == TokenType::CHAR);
  }
}

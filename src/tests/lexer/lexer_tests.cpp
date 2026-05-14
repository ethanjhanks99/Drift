#include "lexer/Lexer.hpp"
#include "lexer/Token.hpp"
#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators.hpp>
#include <string>
#include <unordered_map>

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

TEST_CASE("Lexer tokenizes single character tokens", "[lexer]") {
  SECTION("Left Parenthese") {
    Lexer lexer("(");
    auto tokens = lexer.scan_tokens();

    REQUIRE(tokens[0].type == TokenType::LPAREN);
  }

  SECTION("Right Parenthase") {
    Lexer lexer(")");
    auto tokens = lexer.scan_tokens();

    REQUIRE(tokens[0].type == TokenType::RPAREN);
  }

  SECTION("Left Bracket") {
    Lexer lexer("[");
    auto tokens = lexer.scan_tokens();

    REQUIRE(tokens[0].type == TokenType::LBRACKET);
  }

  SECTION("Right Bracket") {
    Lexer lexer("]");
    auto tokens = lexer.scan_tokens();

    REQUIRE(tokens[0].type == TokenType::RBRACKET);
  }

  SECTION("Left Brace") {
    Lexer lexer("{");
    auto tokens = lexer.scan_tokens();

    REQUIRE(tokens[0].type == TokenType::LBRACE);
  }

  SECTION("Right Brace") {
    Lexer lexer("}");
    auto tokens = lexer.scan_tokens();

    REQUIRE(tokens[0].type == TokenType::RBRACE);
  }

  SECTION("Semicolon") {
    Lexer lexer(";");
    auto tokens = lexer.scan_tokens();

    REQUIRE(tokens[0].type == TokenType::SEMICOLON);
  }

  SECTION("Comma") {
    Lexer lexer(",");
    auto tokens = lexer.scan_tokens();

    REQUIRE(tokens[0].type == TokenType::COMMA);
  }

  SECTION("Dot") {
    Lexer lexer(".");
    auto tokens = lexer.scan_tokens();

    REQUIRE(tokens[0].type == TokenType::DOT);
  }

  SECTION("Colon") {
    Lexer lexer(":");
    auto tokens = lexer.scan_tokens();

    REQUIRE(tokens[0].type == TokenType::COLON);
  }

  SECTION("Question Mark") {
    Lexer lexer("?");
    auto tokens = lexer.scan_tokens();

    REQUIRE(tokens[0].type == TokenType::QMARK);
  }
}

TEST_CASE("Lexer tokenizes multi-character tokens", "[lexer]") {
  SECTION("Return arrow") {
    Lexer lexer("->");
    auto tokens = lexer.scan_tokens();

    REQUIRE(tokens[0].type == TokenType::RETURN_POINT);
  }

  SECTION("Double Colon") {
    Lexer lexer("::");
    auto tokens = lexer.scan_tokens();

    REQUIRE(tokens[0].type == TokenType::COLON_COLON);
  }

  SECTION("Match Arrow") {
    Lexer lexer("=>");
    auto tokens = lexer.scan_tokens();

    REQUIRE(tokens[0].type == TokenType::MATCH_ARROW);
  }

  SECTION("Range (non-inclusive)") {
    Lexer lexer("..");
    auto tokens = lexer.scan_tokens();

    REQUIRE(tokens[0].type == TokenType::RANGE);
  }

  SECTION("Range (inclusive)") {
    Lexer lexer("..=");
    auto tokens = lexer.scan_tokens();

    REQUIRE(tokens[0].type == TokenType::RANGE_INCLUSIVE);
  }
}

TEST_CASE("Lexer tokenizes logical operators", "[lexer]") {
  SECTION("Assignment") {
    Lexer lexer(":=");
    auto tokens = lexer.scan_tokens();

    REQUIRE(tokens[0].type == TokenType::ASSIGN);
  }

  SECTION("Compare Equal") {
    Lexer lexer("==");
    auto tokens = lexer.scan_tokens();

    REQUIRE(tokens[0].type == TokenType::EQUAL);
  }

  SECTION("Not Equal") {
    Lexer lexer("!=");
    auto tokens = lexer.scan_tokens();

    REQUIRE(tokens[0].type == TokenType::NOT_EQUAL);
  }

  SECTION("Less than") {
    Lexer lexer("<");
    auto tokens = lexer.scan_tokens();

    REQUIRE(tokens[0].type == TokenType::LESS);
  }

  SECTION("Less than or equal too") {
    Lexer lexer("<=");
    auto tokens = lexer.scan_tokens();

    REQUIRE(tokens[0].type == TokenType::LESS_EQUAL);
  }

  SECTION("Greater than") {
    Lexer lexer(">");
    auto tokens = lexer.scan_tokens();

    REQUIRE(tokens[0].type == TokenType::GREAT);
  }

  SECTION("Greather than or equal too") {
    Lexer lexer(">=");
    auto tokens = lexer.scan_tokens();

    REQUIRE(tokens[0].type == TokenType::GREAT_EQUAL);
  }

  SECTION("And") {
    Lexer lexer("&&");
    auto tokens = lexer.scan_tokens();

    REQUIRE(tokens[0].type == TokenType::AND);
  }

  SECTION("Or") {
    Lexer lexer("||");
    auto tokens = lexer.scan_tokens();

    REQUIRE(tokens[0].type == TokenType::OR);
  }

  SECTION("Not") {
    Lexer lexer("!");
    auto tokens = lexer.scan_tokens();

    REQUIRE(tokens[0].type == TokenType::NOT);
  }
}

TEST_CASE("Lexer tokenizes Mathematical and Bit Operators", "[lexer]") {
  SECTION("Addition") {
    Lexer lexer("+");
    auto tokens = lexer.scan_tokens();

    REQUIRE(tokens[0].type == TokenType::PLUS);
  }

  SECTION("Subtraction") {
    Lexer lexer("-");
    auto tokens = lexer.scan_tokens();

    REQUIRE(tokens[0].type == TokenType::MINUS);
  }

  SECTION("Multiplication") {
    Lexer lexer("*");
    auto tokens = lexer.scan_tokens();

    REQUIRE(tokens[0].type == TokenType::STAR);
  }

  SECTION("Division") {
    Lexer lexer("/");
    auto tokens = lexer.scan_tokens();

    REQUIRE(tokens[0].type == TokenType::SLASH);
  }

  SECTION("Power") {
    Lexer lexer("**");
    auto tokens = lexer.scan_tokens();

    REQUIRE(tokens[0].type == TokenType::POWER);
  }

  SECTION("Modulo") {
    Lexer lexer("%");
    auto tokens = lexer.scan_tokens();

    REQUIRE(tokens[0].type == TokenType::MODULO);
  }

  SECTION("Plus equals") {
    Lexer lexer("+=");
    auto tokens = lexer.scan_tokens();

    REQUIRE(tokens[0].type == TokenType::PLUS_EQUALS);
  }

  SECTION("Minus equals") {
    Lexer lexer("-=");
    auto tokens = lexer.scan_tokens();

    REQUIRE(tokens[0].type == TokenType::MINUS_EQUALS);
  }

  SECTION("Multiply equals") {
    Lexer lexer("*=");
    auto tokens = lexer.scan_tokens();

    REQUIRE(tokens[0].type == TokenType::MULT_EQUALS);
  }

  SECTION("Divide equals") {
    Lexer lexer("/=");
    auto tokens = lexer.scan_tokens();

    REQUIRE(tokens[0].type == TokenType::DIVIDE_EQUALS);
  }

  SECTION("Plus plus") {
    Lexer lexer("++");
    auto tokens = lexer.scan_tokens();

    REQUIRE(tokens[0].type == TokenType::PLUS_PLUS);
  }

  SECTION("Minus minus") {
    Lexer lexer("--");
    auto tokens = lexer.scan_tokens();

    REQUIRE(tokens[0].type == TokenType::MINUS_MINUS);
  }

  SECTION("Bitwise and") {
    Lexer lexer("&");
    auto tokens = lexer.scan_tokens();

    REQUIRE(tokens[0].type == TokenType::BIT_AND);
  }

  SECTION("Bitwise or") {
    Lexer lexer("|");
    auto tokens = lexer.scan_tokens();

    REQUIRE(tokens[0].type == TokenType::BIT_OR);
  }

  SECTION("Bitwise xor") {
    Lexer lexer("^");
    auto tokens = lexer.scan_tokens();

    REQUIRE(tokens[0].type == TokenType::BIT_XOR);
  }

  SECTION("Bitwise not") {
    Lexer lexer("~");
    auto tokens = lexer.scan_tokens();

    REQUIRE(tokens[0].type == TokenType::BIT_NOT);
  }

  SECTION("Left bit shift") {
    Lexer lexer("<<");
    auto tokens = lexer.scan_tokens();

    REQUIRE(tokens[0].type == TokenType::LBIT_SHIFT);
  }

  SECTION("Right bit shift") {
    Lexer lexer(">>");
    auto tokens = lexer.scan_tokens();

    REQUIRE(tokens[0].type == TokenType::RBIT_SHIFT);
  }
}

TEST_CASE("Lexer tokenizes types", "[lexer]") {
  SECTION("Integer Types") {
    SECTION("I8") {
      Lexer lexer("i8");
      auto tokens = lexer.scan_tokens();

      REQUIRE(tokens[0].type == TokenType::I8);
    }

    SECTION("I16") {
      Lexer lexer("i16");
      auto tokens = lexer.scan_tokens();

      REQUIRE(tokens[0].type == TokenType::I16);
    }

    SECTION("I32") {
      auto types = GENERATE("int", "i32");
      Lexer lexer(types);
      auto tokens = lexer.scan_tokens();

      REQUIRE(tokens[0].type == TokenType::I32);
    }

    SECTION("I64") {
      Lexer lexer("i64");
      auto tokens = lexer.scan_tokens();

      REQUIRE(tokens[0].type == TokenType::I64);
    }

    SECTION("u8") {
      Lexer lexer("u8");
      auto tokens = lexer.scan_tokens();
      REQUIRE(tokens[0].type == TokenType::U8);
    }

    SECTION("U16") {
      Lexer lexer("u16");
      auto tokens = lexer.scan_tokens();

      REQUIRE(tokens[0].type == TokenType::U16);
    }

    SECTION("U32") {
      auto types = GENERATE("uint", "u32");
      Lexer lexer(types);
      auto tokens = lexer.scan_tokens();

      REQUIRE(tokens[0].type == TokenType::U32);
    }

    SECTION("U64") {
      Lexer lexer("u64");
      auto tokens = lexer.scan_tokens();

      REQUIRE(tokens[0].type == TokenType::U64);
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

TEST_CASE("Lexer tokenizes keywords") {
  std::unordered_map<std::string, TokenType> keywords;

  keywords["if"] = TokenType::IF;
  keywords["while"] = TokenType::WHILE;
  keywords["else"] = TokenType::ELSE;
  keywords["func"] = TokenType::FUNC;
  keywords["struct"] = TokenType::STRUCT;
  keywords["true"] = TokenType::TRUE;
  keywords["false"] = TokenType::FALSE;
  keywords["then"] = TokenType::THEN;
  keywords["do"] = TokenType::DO;
  keywords["return"] = TokenType::RETURN;
  keywords["var"] = TokenType::VAR;
  keywords["for"] = TokenType::FOR;
  keywords["ref"] = TokenType::REF;
  keywords["owned"] = TokenType::OWNED;
  keywords["shared"] = TokenType::SHARED;
  keywords["const"] = TokenType::CONST;
  keywords["enum"] = TokenType::ENUM;
  keywords["trait"] = TokenType::TRAIT;
  keywords["impl"] = TokenType::IMPL;
  keywords["match"] = TokenType::MATCH;
  keywords["loop"] = TokenType::LOOP;
  keywords["break"] = TokenType::BREAK;
  keywords["continue"] = TokenType::CONTINUE;
  keywords["import"] = TokenType::IMPORT;
  keywords["pub"] = TokenType::PUB;
  keywords["priv"] = TokenType::PRIV;
  keywords["unsafe"] = TokenType::UNSAFE;
  keywords["static"] = TokenType::STATIC;
  keywords["panic"] = TokenType::PANIC;
  keywords["channel"] = TokenType::CHANNEL;
  keywords["in"] = TokenType::IN;
  keywords["asm"] = TokenType::ASM;
  keywords["Region"] = TokenType::REGION;
  keywords["Option"] = TokenType::OPTION;
  keywords["Result"] = TokenType::RESULT;

  SECTION("keywords") {
    auto words =
        GENERATE("if", "while", "else", "func", "struct", "true", "false",
                 "then", "do", "return", "var", "for", "ref", "owned", "shared",
                 "const", "enum", "trait", "impl", "match", "loop", "break",
                 "continue", "import", "pub", "priv", "unsafe", "static",
                 "panic", "channel", "in", "asm", "Region", "Option", "Result");

    Lexer lexer(words);
    auto tokens = lexer.scan_tokens();

    REQUIRE(keywords.find(tokens[0].lexeme) != keywords.end());
  }

  SECTION("Identifiers") {
    auto vars = GENERATE("num1", "lsia", "_cache_", "_", "plub");
    Lexer lexer(vars);
    auto tokens = lexer.scan_tokens();

    REQUIRE(keywords.find(tokens[0].lexeme) == keywords.end());
    REQUIRE(tokens[0].type == TokenType::IDENTIFIER);
  }

  SECTION("Invalid") {
    auto invalid = GENERATE("$", "#", "`", "=", "\"hello world", "1520.-");
    Lexer lexer(invalid);
    auto tokens = lexer.scan_tokens();

    REQUIRE(tokens[0].type == TokenType::ERROR_TOKEN);
  }
}

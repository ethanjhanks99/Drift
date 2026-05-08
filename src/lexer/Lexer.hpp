#pragma once

#include "Token.hpp"
#include <string>
#include <unordered_map>
#include <vector>

class Lexer {
public:
  Lexer(std::string source);
  std::vector<Token> scan_tokens();

private:
  std::string source;
  std::vector<Token> tokens;
  std::unordered_map<std::string, TokenType> keywords;

  int start = 0;
  int current = 0;
  int line = 1;

  bool is_at_end();
  char advance();
  char peek();
  char peek_next();
  bool match(char expected);
  void add_token(TokenType type);
  void add_token(TokenType type, std::string literal);
  void scan_token();

  void handle_attribute();
  void string();
  void number();
  void identifier();
  void handle_range();
};

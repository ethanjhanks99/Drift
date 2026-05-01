#pragma once

#include "Token.hpp"
#include <string>
#include <unordered_map>
#include <vector>

class Lexer {
public:
  Lexer(std::string source);
  std::vector<Token> scanTokens();

private:
  std::string source;
  std::vector<Token> tokens;
  std::unordered_map<std::string, TokenType> keywords;

  int start = 0;
  int current = 0;
  int line = 1;

  bool isAtEnd();
  char advance();
  char peek();
  char peekNext();
  bool match(char expected);
  void addToken(TokenType type);
  void addToken(TokenType type, std::string literal);
  void scanToken();

  void handle_attribute();
  void string();
  void number();
  void identifier();
  void handle_range();
};

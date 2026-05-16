#pragma once

#include "ASTNode.hpp"
#include "lexer/Token.hpp"
#include <vector>

class Parser {
public:
  Parser(std::vector<Token> token_stream);
  ASTNode *parse();
};

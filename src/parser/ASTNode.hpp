#pragma once

#include "lexer/Token.hpp"

class ASTNode {
public:
  ASTNode(Token token);
  ASTNode *add_child(Token token);
};

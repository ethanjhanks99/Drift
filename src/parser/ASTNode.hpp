#pragma once

#include "lexer/Token.hpp"

class ASTNode {
public:
  ASTNode(Token token);
  ASTNode *add_child(Token token);
  ASTNode *get_parent();

private:
  Token m_token;
  ASTNode *m_child;
  ASTNode *m_parent;
};

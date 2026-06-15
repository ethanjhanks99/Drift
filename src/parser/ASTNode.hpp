#pragma once

#include <memory>
#include <vector>

#include "lexer/Token.hpp"

class ASTNode {
public:
  ASTNode(Token token);
  ASTNode *add_child(Token token);
  ASTNode *get_parent();

private:
  Token m_token;
  std::vector<std::unique_ptr<ASTNode>> m_children;
  ASTNode *m_parent;
};

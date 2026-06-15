#include "ASTNode.hpp"
#include <memory>

ASTNode::ASTNode(Token token)
    : m_token(token), m_children({}), m_parent(nullptr) {}

ASTNode *ASTNode::add_child(Token token) {
  auto &child = m_children.emplace_back(std::make_unique<ASTNode>(token));
  child->m_parent = this;
  return child.get();
}

ASTNode *ASTNode::get_parent() { return m_parent; }

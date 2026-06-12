#include "ASTNode.hpp"

ASTNode::ASTNode(Token token)
    : m_token(token), m_child({}), m_parent(nullptr) {}

ASTNode *ASTNode::add_child(Token token){ASTNode *new_child = }

ASTNode *ASTNode::get_parent() {}

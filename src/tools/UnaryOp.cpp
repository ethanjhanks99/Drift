#include "UnaryOp.hpp"
#include "lexer/Token.hpp"

UnaryOp convert_unary(TokenType token) {
  if (token == TokenType::NOT)
    return UnaryOp::NOT;
  if (token == TokenType::BIT_NOT)
    return UnaryOp::BNOT;
  if (token == TokenType::PLUS_PLUS)
    return UnaryOp::PPLUS;
  if (token == TokenType::MINUS_MINUS)
    return UnaryOp::SSUB;
  if (token == TokenType::MINUS)
    return UnaryOp::NEG;
  if (token == TokenType::QMARK)
    return UnaryOp::QMARK;

  return UnaryOp::ERROR;
}

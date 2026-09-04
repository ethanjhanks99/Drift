#include "tools/BinaryOp.hpp"

BinaryOp convert_binary(TokenType token) {
  if (token == TokenType::PLUS)
    return BinaryOp::ADD;
  if (token == TokenType::MINUS)
    return BinaryOp::SUB;
  if (token == TokenType::STAR)
    return BinaryOp::MULT;
  if (token == TokenType::SLASH)
    return BinaryOp::DIV;
  if (token == TokenType::POWER)
    return BinaryOp::POW;
  if (token == TokenType::MODULO)
    return BinaryOp::MOD;
  if (token == TokenType::LBIT_SHIFT)
    return BinaryOp::LSHIFT;
  if (token == TokenType::RBIT_SHIFT)
    return BinaryOp::RSHIFT;
  if (token == TokenType::AND)
    return BinaryOp::AND;
  if (token == TokenType::OR)
    return BinaryOp::OR;
  if (token == TokenType::GREAT)
    return BinaryOp::GREAT;
  if (token == TokenType::GREAT_EQUAL)
    return BinaryOp::GREAT_EQUAL;
  if (token == TokenType::LESS)
    return BinaryOp::LESS;
  if (token == TokenType::LESS_EQUAL)
    return BinaryOp::LESS_EQUAL;
  if (token == TokenType::NOT_EQUAL)
    return BinaryOp::NOT_EQUAL;
  if (token == TokenType::EQUAL)
    return BinaryOp::EQUAL;
  if (token == TokenType::BIT_XOR)
    return BinaryOp::XOR;
  if (token == TokenType::BIT_AND)
    return BinaryOp::BAND;
  if (token == TokenType::BIT_OR)
    return BinaryOp::BOR;

  return BinaryOp::ERROR;
}

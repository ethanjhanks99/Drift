#include "tools/BinaryOp.hpp"

#include <string>

BinaryOp convert_binary(std::string lexeme) {
  if (lexeme == "+")
    return BinaryOp::ADD;
  if (lexeme == "-")
    return BinaryOp::SUB;
  if (lexeme == "*")
    return BinaryOp::MULT;
  if (lexeme == "/")
    return BinaryOp::DIV;
  if (lexeme == "**")
    return BinaryOp::POW;
  if (lexeme == "%")
    return BinaryOp::MOD;
  if (lexeme == "<<")
    return BinaryOp::LSHIFT;
  if (lexeme == ">>")
    return BinaryOp::RSHIFT;
  if (lexeme == "&&")
    return BinaryOp::AND;
  if (lexeme == "||")
    return BinaryOp::OR;
  if (lexeme == ">")
    return BinaryOp::GREAT;
  if (lexeme == ">=")
    return BinaryOp::GREAT_EQUAL;
  if (lexeme == "<")
    return BinaryOp::LESS;
  if (lexeme == "<=")
    return BinaryOp::LESS_EQUAL;
  if (lexeme == "!=")
    return BinaryOp::NOT_EQUAL;
  if (lexeme == "==")
    return BinaryOp::EQUAL;
  if (lexeme == "^")
    return BinaryOp::XOR;
  if (lexeme == "&")
    return BinaryOp::BAND;
  if (lexeme == "|")
    return BinaryOp::BOR;

  return BinaryOp::ERROR;
}

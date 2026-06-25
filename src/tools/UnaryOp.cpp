#include "UnaryOp.hpp"

#include <string>

UnaryOp convert_unary(std::string lexeme) {
  if (lexeme == "!")
    return UnaryOp::NOT;
  if (lexeme == "~")
    return UnaryOp::BNOT;
  if (lexeme == "++")
    return UnaryOp::PPLUS;
  if (lexeme == "--")
    return UnaryOp::SSUB;
  if (lexeme == "-")
    return UnaryOp::NEG;
  if (lexeme == "?")
    return UnaryOp::QMARK;

  return UnaryOp::ERROR;
}

#include "AssignOp.hpp"

AssignOp convert_assign(TokenType assign) {
  if (assign == TokenType::ASSIGN)
    return AssignOp::Assign;
  if (assign == TokenType::PLUS_EQUALS)
    return AssignOp::PlusEqual;
  if (assign == TokenType::MINUS_EQUALS)
    return AssignOp::MinusEqual;
  if (assign == TokenType::MULT_EQUALS)
    return AssignOp::MultEqual;
  if (assign == TokenType::DIVIDE_EQUALS)
    return AssignOp::DivideEqual;
  else
    return AssignOp::Error;
}

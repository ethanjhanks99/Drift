#pragma once

#include "lexer/Token.hpp"

enum class AssignOp {
  Assign,
  PlusEqual,
  MinusEqual,
  MultEqual,
  DivideEqual,
  Error
};

AssignOp convert_assign(TokenType assign);

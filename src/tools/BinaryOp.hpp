#pragma once

#include <string>

enum class BinaryOp {
  ADD,
  SUB,
  MULT,
  DIV,
  POW,
  MOD,
  LSHIFT,
  RSHIFT,
  AND,
  OR,
  GREAT,
  GREAT_EQUAL,
  LESS,
  LESS_EQUAL,
  NOT_EQUAL,
  EQUAL,
  XOR,
  BAND,
  BOR,
  ERROR
};

BinaryOp convert_binary(std::string lexeme);

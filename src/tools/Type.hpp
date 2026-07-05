#pragma once

#include "lexer/Token.hpp"

enum class Type {
  I8,
  I16,
  I32,
  I64,
  U8,
  U16,
  U32,
  U64,
  FLOAT,
  STRING,
  CHAR,
  BOOL,
  VOID,
  CUSTOM,
  FUNC,
  ERROR
};

Type convert_type(TokenType type);

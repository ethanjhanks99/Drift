#include "Type.hpp"
#include "lexer/Token.hpp"

Type convert_type(TokenType type) {
  switch (type) {
  case TokenType::I8:
    return Type::I8;
  case TokenType::I16:
    return Type::I16;
  case TokenType::I32:
    return Type::I32;
  case TokenType::I64:
    return Type::I64;
  case TokenType::U8:
    return Type::U8;
  case TokenType::U16:
    return Type::U16;
  case TokenType::U32:
    return Type::U32;
  case TokenType::U64:
    return Type::U64;
  case TokenType::FLOAT:
    return Type::FLOAT;
  case TokenType::STRING:
    return Type::STRING;
  case TokenType::CHAR:
    return Type::CHAR;
  case TokenType::BOOL:
    return Type::BOOL;
  case TokenType::VOID:
    return Type::VOID;
  case TokenType::IDENTIFIER:
    return Type::CUSTOM;
  default:
    return Type::ERROR;
  }
}

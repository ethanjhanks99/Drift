#pragma once

#include <ostream>
#include <string>


/**
 * Copilot: Organize the token types into categories and assign them specific integer values for better readability and maintainability.
 */

enum class TokenType {
  /* Single Character Token Types */
  LPAREN,
  RPAREN,
  LBRACKET,
  RBRACKET,
  LBRACE,
  RBRACE,
  SEMICOLON,
  COMMA,
  DOT,
  RETURN_POINT,
  COLON,
  QMARK,

  /* Logical Operators */
  ASSIGN,
  EQUAL,
  NOT_EQUAL,
  LESS,
  LESS_EQUAL,
  GREAT,
  GREAT_EQUAL,
  AND,
  OR,
  NOT,

  /* Mathematical and Bit Operators */
  PLUS,
  MINUS,
  STAR,
  SLASH,
  MODULO,
  BIT_AND,
  BIT_OR,
  BIT_XOR,
  BIT_NOT,
  LBIT_SHIFT,
  RBIT_SHIFT,
  PLUS_EQUALS,
  MINUS_EQUALS,
  MULT_EQUALS,
  DIVIDE_EQUALS,
  POWER,
  PLUS_PLUS,
  MINUS_MINUS,

  /* Values and User Provided */
  IDENTIFIER,
  INT_LITERAL,
  FLOAT_LITERAL,
  STR_LIT,
  CHAR_LIT,

  /* Other shtuff */
  ATTRIBUTE,
  COLON_COLON,
  MATCH_ARROW,
  RANGE,
  RANGE_INCLUSIVE,
  STATIC,

  /* Keywords */
  // types
  VOID,
  INT,
  FLOAT,
  I8,
  I16,
  I32,
  I64,
  UINT,
  U8,
  U16,
  U32,
  U64,
  F8,
  F16,
  F32,
  F64,
  BOOL,
  STRING,
  CHAR,
  STRUCT,
  ENUM,
  ATOMIC,
  // control flow
  IF,
  ELSE,
  WHILE,
  FUNC,
  THEN,
  DO,
  RETURN,
  TRUE,
  FALSE,
  REF,
  OWNED,
  SHARED,
  CONST,
  TRAIT,
  IMPL,
  MATCH,
  LOOP,
  BREAK,
  CONTINUE,
  IMPORT,
  PUB,
  PRIV,
  PANIC,
  UNSAFE,
  FOR,
  CHANNEL,
  ASM,
  IN,

  /* Reserved Words 52-63 */
  VAR,

  /* Special 64-65 */
  END_OF_FILE,
  ERROR_TOKEN
};

struct Token {
  TokenType type;
  std::string lexeme;
  uint line;
  uint column;

  Token(TokenType t, std::string le, uint l, uint c)
      : type(t), lexeme(le), line(l), column(c) {}

  std::string toString() const {
    return "Token(" + std::to_string((int)type) + ", '" + lexeme + "')";
  }

  friend std::ostream &operator<<(std::ostream &os, const Token &t);
};

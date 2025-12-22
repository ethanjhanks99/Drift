#pragma once

#include <string>

enum class TokenType {
  /* Single Character Token Types 0-9 */
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

  /* Logical Operators 10-19 */
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

  /* Mathematical and Bit Operators 20-30 */
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

  /* Values and User Provided 31-35 */
  IDENTIFIER,
  INT_LITERAL,
  FLOAT_LITERAL,
  STR_LIT,
  CHAR_LIT,

  /* Keywords 36-51 */
  IF,
  WHILE,
  ELSE,
  FUNC,
  INT,
  VOID,
  STRING,
  CHAR,
  BOOL,
  STRUCT,
  TRUE,
  FALSE,
  THEN,
  DO,
  RETURN,
  REF,

  /* Reserved Words 52-63 */
  CLASS,
  PLUS_EQUALS,
  MINUS_EQUALS,
  MULT_EQUALS,
  DIVIDE_EQUALS,
  POWER,
  FOR,
  VAR,
  COLON,
  PLUS_PLUS,
  MINUS_MINUS,

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
};

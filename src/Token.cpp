#include "Token.hpp"

std::ostream &operator<<(std::ostream &os, const Token &t) {
  std::string typeName;
  switch (t.type) {
  case TokenType::LPAREN:
    typeName = "LPAREN";
    break;
  case TokenType::RPAREN:
    typeName = "RPAREN";
    break;
  case TokenType::LBRACKET:
    typeName = "LBRACKET";
    break;
  case TokenType::RBRACKET:
    typeName = "RBRACKET";
    break;
  case TokenType::LBRACE:
    typeName = "LBRACE";
    break;
  case TokenType::RBRACE:
    typeName = "RBRACE";
    break;
  case TokenType::SEMICOLON:
    typeName = "SEMICOLON";
    break;
  case TokenType::COMMA:
    typeName = "COMMA";
    break;
  case TokenType::DOT:
    typeName = "DOT";
    break;
  case TokenType::RETURN_POINT:
    typeName = "RETURN_POINT";
    break;
  case TokenType::COLON:
    typeName = "COLON";
    break;
  case TokenType::QMARK:
    typeName = "QMARK";
    break;
  case TokenType::ASSIGN:
    typeName = "ASSIGN";
    break;
  case TokenType::EQUAL:
    typeName = "EQUAL";
    break;
  case TokenType::NOT_EQUAL:
    typeName = "NOT_EQUAL";
    break;
  case TokenType::LESS:
    typeName = "LESS";
    break;
  case TokenType::LESS_EQUAL:
    typeName = "LESS_EQUAL";
    break;
  case TokenType::GREAT:
    typeName = "GREAT";
    break;
  case TokenType::GREAT_EQUAL:
    typeName = "GREAT_EQUAL";
    break;
  case TokenType::AND:
    typeName = "AND";
    break;
  case TokenType::OR:
    typeName = "OR";
    break;
  case TokenType::NOT:
    typeName = "NOT";
    break;
  case TokenType::PLUS:
    typeName = "PLUS";
    break;
  case TokenType::MINUS:
    typeName = "MINUS";
    break;
  case TokenType::STAR:
    typeName = "STAR";
    break;
  case TokenType::SLASH:
    typeName = "SLASH";
    break;
  case TokenType::MODULO:
    typeName = "MODULO";
    break;
  case TokenType::BIT_AND:
    typeName = "BIT_AND";
    break;
  case TokenType::BIT_OR:
    typeName = "BIT_OR";
    break;
  case TokenType::BIT_XOR:
    typeName = "BIT_XOR";
    break;
  case TokenType::BIT_NOT:
    typeName = "BIT_NOT";
    break;
  case TokenType::LBIT_SHIFT:
    typeName = "LBIT_SHIFT";
    break;
  case TokenType::RBIT_SHIFT:
    typeName = "RBIT_SHIFT";
    break;
  case TokenType::PLUS_EQUALS:
    typeName = "PLUS_EQUALS";
    break;
  case TokenType::MINUS_EQUALS:
    typeName = "MINUS_EQUALS";
    break;
  case TokenType::MULT_EQUALS:
    typeName = "MULT_EQUALS";
    break;
  case TokenType::DIVIDE_EQUALS:
    typeName = "DIVIDE_EQUALS";
    break;
  case TokenType::POWER:
    typeName = "POWER";
    break;
  case TokenType::PLUS_PLUS:
    typeName = "PLUS_PLUS";
    break;
  case TokenType::MINUS_MINUS:
    typeName = "MINUS_MINUS";
    break;
  case TokenType::IDENTIFIER:
    typeName = "IDENTIFIER";
    break;
  case TokenType::INT_LITERAL:
    typeName = "INT_LITERAL";
    break;
  case TokenType::FLOAT_LITERAL:
    typeName = "FLOAT_LITERAL";
    break;
  case TokenType::STR_LIT:
    typeName = "STR_LIT";
    break;
  case TokenType::CHAR_LIT:
    typeName = "CHAR_LIT";
    break;
  case TokenType::ATTRIBUTE:
    typeName = "ATTRIBUTE";
    break;
  case TokenType::COLON_COLON:
    typeName = "COLON_COLON";
    break;
  case TokenType::MATCH_ARROW:
    typeName = "MATCH_ARROW";
    break;
  case TokenType::RANGE:
    typeName = "RANGE";
    break;
  case TokenType::RANGE_INCLUSIVE:
    typeName = "RANGE_INCLUSIVE";
    break;
  case TokenType::STATIC:
    typeName = "STATIC";
    break;
  case TokenType::IF:
    typeName = "IF";
    break;
  case TokenType::WHILE:
    typeName = "WHILE";
    break;
  case TokenType::ELSE:
    typeName = "ELSE";
    break;
  case TokenType::FUNC:
    typeName = "FUNC";
    break;
  case TokenType::I8:
    typeName = "I8";
    break;
  case TokenType::I16:
    typeName = "I16";
    break;
  case TokenType::I32:
    typeName = "I32";
    break;
  case TokenType::I64:
    typeName = "I64";
    break;
  case TokenType::U8:
    typeName = "U8";
    break;
  case TokenType::U16:
    typeName = "U16";
    break;
  case TokenType::U32:
    typeName = "U32";
    break;
  case TokenType::U64:
    typeName = "U64";
    break;
  case TokenType::FLOAT:
    typeName = "FLOAT";
    break;
  case TokenType::VOID:
    typeName = "VOID";
    break;
  case TokenType::STRING:
    typeName = "STRING";
    break;
  case TokenType::CHAR:
    typeName = "CHAR";
    break;
  case TokenType::BOOL:
    typeName = "BOOL";
    break;
  case TokenType::STRUCT:
    typeName = "STRUCT";
    break;
  case TokenType::TRUE:
    typeName = "TRUE";
    break;
  case TokenType::FALSE:
    typeName = "FALSE";
    break;
  case TokenType::THEN:
    typeName = "THEN";
    break;
  case TokenType::DO:
    typeName = "DO";
    break;
  case TokenType::RETURN:
    typeName = "RETURN";
    break;
  case TokenType::REF:
    typeName = "REF";
    break;
  case TokenType::OWNED:
    typeName = "OWNED";
    break;
  case TokenType::SHARED:
    typeName = "SHARED";
    break;
  case TokenType::CONST:
    typeName = "CONST";
    break;
  case TokenType::ENUM:
    typeName = "ENUM";
    break;
  case TokenType::TRAIT:
    typeName = "TRAIT";
    break;
  case TokenType::IMPL:
    typeName = "IMPL";
    break;
  case TokenType::MATCH:
    typeName = "MATCH";
    break;
  case TokenType::LOOP:
    typeName = "LOOP";
    break;
  case TokenType::BREAK:
    typeName = "BREAK";
    break;
  case TokenType::CONTINUE:
    typeName = "CONTINUE";
    break;
  case TokenType::IMPORT:
    typeName = "IMPORT";
    break;
  case TokenType::PUB:
    typeName = "PUB";
    break;
  case TokenType::PRIV:
    typeName = "PRIV";
    break;
  case TokenType::PANIC:
    typeName = "PANIC";
    break;
  case TokenType::UNSAFE:
    typeName = "UNSAFE";
    break;
  case TokenType::FOR:
    typeName = "FOR";
    break;
  case TokenType::ATOMIC:
    typeName = "ATOMIC";
    break;
  case TokenType::CHANNEL:
    typeName = "CHANNEL";
    break;
  case TokenType::ASM:
    typeName = "ASM";
    break;
  case TokenType::IN:
    typeName = "IN";
    break;
  case TokenType::VAR:
    typeName = "VAR";
    break;
  case TokenType::END_OF_FILE:
    typeName = "END_OF_FILE";
    break;
  case TokenType::ERROR_TOKEN:
    typeName = "ERROR_TOKEN";
    break;
  default:
    typeName = "UNKNOWN";
    break;
  }
  os << "Token { type: " << typeName << ", value: " << t.lexeme << " }";
  return os;
}

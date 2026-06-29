#include "Parser.hpp"
#include "lexer/Token.hpp"
#include "tools/AST.hpp"
#include "tools/SourceLocation.hpp"

bool Parser::consume(Token &token) {
  if (is_at_end())
    return false;
  if (peek().type != token.type)
    return false;

  current++;
  return true;
}

bool Parser::expect(Token &token) {
  if (is_at_end())
    return false;
  return peek().type == token.type;
}

Token Parser::advance() { return m_token_stream[current++]; }

bool Parser::is_at_end() {
  return m_token_stream[current].type == TokenType::END_OF_FILE;
}

Token Parser::peek() { return m_token_stream[current]; }

AST *Parser::parse_program() {
  SourceLocation loc(peek().loc);
  Program *program = new Program(loc);

  while (!is_at_end()) {
    AST *top = parse_top_level_decl();
    program->top_level_decls.emplace_back(top);
  }

  return program;
}

AST *Parser::parse_top_level_decl() {
  switch (peek().type) {
  case TokenType::IMPORT:
    return parse_import();
  case TokenType::FUNC:
    return parse_function_definition();
  case TokenType::ATTRIBUTE:
    return parse_function_definition();
  case TokenType::STRUCT:
    return parse_struct_definition();
  case TokenType::ENUM:
    return parse_enum_definition();
  case TokenType::TRAIT:
    return parse_trait_definition();
  case TokenType::IMPL:
    return parse_impl_definition();
  case TokenType::STATIC:
    return parse_variable_definition();
  case TokenType::OWNED:
    return parse_variable_definition();
  case TokenType::REF:
    return parse_variable_definition();
  case TokenType::SHARED:
    return parse_variable_definition();
  case TokenType::CONST:
    return parse_variable_definition();
  case TokenType::IDENTIFIER:
    return parse_variable_definition();
  }
  return handle_error();
}

AST *Parser::parse_attribute() {}

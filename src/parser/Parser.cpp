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

AST *Parser::parse_top_level_decl() {}

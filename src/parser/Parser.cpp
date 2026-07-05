#include "Parser.hpp"
#include "error/ErrorHandler.hpp"
#include "lexer/Token.hpp"
#include "tools/AST.hpp"
#include "tools/ParseError.hpp"
#include "tools/SourceLocation.hpp"
#include "tools/VisMod.hpp"
#include <expected>
#include <memory>

bool Parser::expect(TokenType type) {
  if (is_at_end())
    return false;
  return curr_token.type == type;
}

void Parser::consume() { curr_token = m_token_stream[current++]; }

bool Parser::is_at_end() { return peek().type == TokenType::END_OF_FILE; }

Token Parser::peek() { return m_token_stream[current]; }

std::expected<AST *, ParseError> Parser::parse_program() {
  SourceLocation loc(peek().loc);
  Program *program = new Program(loc);

  while (!is_at_end()) {
    consume();
    auto top = parse_top_level_decl();

    if (!top)
      handle_parser_error(top.error(), curr_token);

    program->top_level_decls.emplace_back(*top);
  }

  return program;
}

std::expected<AST *, ParseError> Parser::parse_top_level_decl() {
  switch (curr_token.type) {
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
  default:
    break;
  }
  return handle_error();
}

std::expected<AST *, ParseError> Parser::parse_import() {
  consume();

  if (expect(TokenType::END_OF_FILE))
    return std::unexpected(ParseError::UnexpectedEOF);

  if (!expect(TokenType::IDENTIFIER))
    return std::unexpected(ParseError::UnexpectedToken);

  ImportStatement *import = new ImportStatement(curr_token.loc);
  import->module = curr_token.lexeme;
  return import;
}

std::expected<AST *, ParseError> Parser::parse_function_definition() {
  FunctionDef *func = new FunctionDef(curr_token.loc);

  while (curr_token.type == TokenType::ATTRIBUTE) {
    func->attributes.emplace_back(
        new Attribute(curr_token.loc, curr_token.lexeme));
    consume();
  }

  if (is_at_end()) {
    return handle_error();
  }

  if (expect(TokenType::PUB)) {
    func->vis_mod = VisMod::PUB;
    consume();
  } else {
    func->vis_mod = VisMod::PRIV;
    if (expect(TokenType::PRIV))
      consume();
  }

  if (expect(TokenType::END_OF_FILE))
    return std::unexpected(ParseError::UnexpectedEOF);

  if (!expect(TokenType::FUNC))
    return std::unexpected(ParseError::UnexpectedToken);

  consume();

  if (!expect(TokenType::IDENTIFIER))
    return std::unexpected(ParseError::UnexpectedToken);

  func->name = curr_token.lexeme;

  if (expect(TokenType::LESS)) {
    consume();

    if (expect(TokenType::GREAT))
      return std::unexpected(ParseError::UnexpectedToken);

    while (!expect(TokenType::GREAT)) {
      if (expect(TokenType::END_OF_FILE))
        return std::unexpected(ParseError::UnexpectedEOF);
      auto gen_dec = parse_generic_declaration();
      func->generics.emplace_back(gen_dec.value());
    }
  }

  consume();

  if (!expect(TokenType::LPAREN)) {
    return std::unexpected(ParseError::UnexpectedToken);
  }

  consume();

  while (!expect(TokenType::RPAREN)) {
    auto param = parse_param();
    func->param_list.emplace_back(param.value());
  }

  consume();

  auto ret = parse_function_return();
  func->function_return = std::unique_ptr<AST>(ret.value());

  if (!expect(TokenType::LBRACE))
    return std::unexpected(ParseError::UnexpectedToken);

  consume();

  while (!expect(TokenType::RBRACE)) {
    auto block = parse_block();
    func->block.emplace_back(block.value());
  }
  consume();

  return func;
}

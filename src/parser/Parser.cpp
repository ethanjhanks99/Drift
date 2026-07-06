#include "Parser.hpp"
#include "error/ErrorHandler.hpp"
#include "lexer/Token.hpp"
#include "tools/AST.hpp"
#include "tools/ParseError.hpp"
#include "tools/SourceLocation.hpp"
#include "tools/VisMod.hpp"
#include <expected>
#include <memory>
#include <utility>

bool Parser::expect(TokenType type) {
  if (is_at_end())
    return false;
  return curr_token.type == type;
}

void Parser::consume() { curr_token = m_token_stream[current++]; }

bool Parser::is_at_end() { return peek().type == TokenType::END_OF_FILE; }

Token Parser::peek() { return m_token_stream[current]; }

AST *Parser::parse() {
  consume();

  return parse_program().value();
}

std::expected<AST *, ParseError> Parser::parse_program() {
  SourceLocation loc(peek().loc);
  Program *program = new Program(loc);

  while (!is_at_end()) {
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
  case TokenType::PRIV:
  case TokenType::PUB:
  default:
    return std::unexpected(ParseError::UnexpectedToken);
  }
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

  if (is_at_end())
    return std::unexpected(ParseError::UnexpectedEOF);

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

  auto gen_dec = parse_generic_declaration();
  if (!gen_dec) {
    handle_parser_error(gen_dec.error(), curr_token);
  }
  func->generics = std::move(*gen_dec);

  consume();

  auto param_list = parse_param_list();

  if (!param_list)
    handle_parser_error(param_list.error(), curr_token);
  func->param_list = std::move(*param_list);

  consume();

  auto ret = parse_function_return();
  func->function_return = std::unique_ptr<AST>(ret.value());

  auto block = parse_block();
  if (!block)
    handle_parser_error(block.error(), curr_token);
  func->block = std::move(*block);
  consume();

  return func;
}

std::expected<AST *, ParseError> Parser::parse_struct_definition() {
  StructDef *stct = new StructDef(curr_token.loc);

  if (expect(TokenType::PUB)) {
    stct->vis_mod = VisMod::PUB;
    consume();
  } else {
    stct->vis_mod = VisMod::PRIV;
    if (expect(TokenType::PRIV))
      consume();
  }

  consume(); // struct keyword

  if (expect(TokenType::END_OF_FILE))
    handle_parser_error(ParseError::UnexpectedEOF, curr_token);
  if (!expect(TokenType::IDENTIFIER))
    handle_parser_error(ParseError::UnexpectedToken, curr_token);

  stct->name = curr_token.lexeme;

  auto gen_dec = parse_generic_declaration();
  if (!gen_dec)
    handle_parser_error(gen_dec.error(), curr_token);
  stct->generics = std::move(*gen_dec);

  auto fields = parse_struct_fields();
  if (!fields)
    handle_parser_error(fields.error(), curr_token);
  stct->fields = std::move(*fields);

  return stct;
}

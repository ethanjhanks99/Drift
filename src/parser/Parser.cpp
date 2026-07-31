#include "Parser.hpp"
#include "error/ErrorHandler.hpp"
#include "lexer/Token.hpp"
#include "tools/AST.hpp"
#include "tools/OwnershipMod.hpp"
#include "tools/ParseError.hpp"
#include "tools/SourceLocation.hpp"
#include "tools/Type.hpp"
#include "tools/VisMod.hpp"
#include <expected>
#include <memory>
#include <utility>
#include <vector>

bool Parser::expect(TokenType type) {
  if (is_at_end())
    return false;
  return peek().type == type;
}

Token Parser::consume() { return m_token_stream[current++]; }

bool Parser::is_at_end() { return peek().type == TokenType::END_OF_FILE; }

Token Parser::peek() { return m_token_stream[current]; }

Token Parser::look_ahead() { return m_token_stream[current + 1]; }

VisMod Parser::get_visibility() {
  if (expect(TokenType::PUB)) {
    consume();
    return VisMod::PUB;
  }
  if (expect(TokenType::PRIV)) {
    consume();
  }
  return VisMod::PRIV;
}

OwnershipMod Parser::get_ownership() {
  if (expect(TokenType::REF)) {
    consume();
    return OwnershipMod::REF;
  } else if (expect(TokenType::CONST)) {
    consume();
    return OwnershipMod::CONST;
  } else if (expect(TokenType::SHARED)) {
    consume();
    return OwnershipMod::SHARED;
  } else if (expect(TokenType::OWNED)) {
    consume();
  }
  return OwnershipMod::OWNED;
}

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
  case TokenType::PRIV:
  case TokenType::PUB:
    return parse_vismod();
  default:
    return std::unexpected(ParseError::UnexpectedToken);
  }
}

std::expected<AST *, ParseError> Parser::parse_vismod() {
  switch (look_ahead().type) {
  case TokenType::FUNC:
    return parse_function_definition();
  case TokenType::STRUCT:
    return parse_struct_definition();
  case TokenType::ENUM:
    return parse_enum_definition();
  case TokenType::TRAIT:
    return parse_trait_definition();
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
    return std::unexpected(ParseError::UnexpectedToken);
  }
}

std::expected<AST *, ParseError> Parser::parse_import() {
  consume(); // skip IMPORT keyword

  if (expect(TokenType::END_OF_FILE))
    return std::unexpected(ParseError::UnexpectedEOF);

  if (!expect(TokenType::IDENTIFIER))
    return std::unexpected(ParseError::UnexpectedToken);

  Token import_name = consume();

  ImportStatement *import = new ImportStatement(import_name.loc);
  import->module = import_name.lexeme;
  return import;
}

std::expected<AST *, ParseError> Parser::parse_function_definition() {
  FunctionDef *func = new FunctionDef(peek().loc);

  while (peek().type == TokenType::ATTRIBUTE) {
    Token att = consume();
    func->attributes.emplace_back(new Attribute(att.loc, att.lexeme));
  }

  if (is_at_end())
    return std::unexpected(ParseError::UnexpectedEOF);

  func->vis_mod = get_visibility();

  if (expect(TokenType::END_OF_FILE))
    return std::unexpected(ParseError::UnexpectedEOF);

  if (!expect(TokenType::FUNC))
    return std::unexpected(ParseError::UnexpectedToken);
  consume(); // skip FUNC keyword

  if (!expect(TokenType::IDENTIFIER))
    return std::unexpected(ParseError::UnexpectedToken);

  Token name = consume();

  func->name = name.lexeme;

  auto gen_dec = parse_generic_declaration();
  if (!gen_dec)
    return std::unexpected(gen_dec.error());

  func->generics = std::move(*gen_dec);

  auto param_list = parse_param_list();

  if (!param_list)
    return std::unexpected(param_list.error());
  func->param_list = std::move(*param_list);

  auto ret = parse_function_return();
  if (!ret)
    return std::unexpected(ret.error());

  func->function_return = std::unique_ptr<AST>(ret.value());

  auto block = parse_block();
  if (!block)
    return std::unexpected(block.error());
  func->block = std::move(*block);

  return func;
}

std::expected<AST *, ParseError> Parser::parse_function_return() {
  FunctionReturn *ret = new FunctionReturn(peek().loc);

  OwnershipMod owner = get_ownership();
  ret->ownership = owner;

  if (is_at_end())
    return std::unexpected(ParseError::UnexpectedEOF);

  Token ret_type = consume();
  Type type = convert_type(ret_type.type);

  if (type == Type::ERROR)
    return std::unexpected(ParseError::UnexpectedToken);
  ret->type = type;

  return ret;
}

std::expected<std::vector<std::unique_ptr<AST>>, ParseError>
Parser::parse_param_list() {
  std::vector<std::unique_ptr<AST>> param_list;

  if (expect(TokenType::RPAREN))
    return param_list;

  auto param = parse_param();
  if (!param)
    return std::unexpected(param.error());
  param_list.emplace_back(*param);

  if (!expect(TokenType::COMMA))
    return param_list;

  do {
    consume();
    auto next_param = parse_param();
    if (!next_param)
      return std::unexpected(next_param.error());
    param_list.emplace_back(*next_param);
  } while (expect(TokenType::COMMA));

  return param_list;
}

std::expected<AST *, ParseError> Parser::parse_param() {
  Param *param = new Param(peek().loc);

  OwnershipMod owner = get_ownership();
  param->ownership = owner;

  if (is_at_end())
    return std::unexpected(ParseError::UnexpectedEOF);
  if (!expect(TokenType::IDENTIFIER))
    return std::unexpected(ParseError::UnexpectedToken);
  Token name = consume();
  param->name = name.lexeme;

  if (expect(TokenType::LBRACKET)) {
    param->is_array = true;
    consume();
    if (!expect(TokenType::RBRACKET))
      return std::unexpected(ParseError::UnexpectedToken);
    consume();
  }

  if (!expect(TokenType::COLON))
    return std::unexpected(ParseError::UnexpectedToken);
  consume();

  Token param_type = consume();
  Type type = convert_type(param_type.type);
  if (type == Type::ERROR)
    return std::unexpected(ParseError::UnexpectedToken);
  param->type = type;

  return param;
}

std::expected<AST *, ParseError> Parser::parse_struct_definition() {
  StructDef *strct = new StructDef(peek().loc);

  strct->vis_mod = get_visibility();

  if (expect(TokenType::END_OF_FILE))
    return std::unexpected(ParseError::UnexpectedEOF);
  if (!expect(TokenType::STRUCT))
    return std::unexpected(ParseError::UnexpectedToken);
  consume(); // skip STRUCT keyword

  if (!expect(TokenType::IDENTIFIER))
    return std::unexpected(ParseError::UnexpectedToken);

  Token name = consume();

  strct->name = name.lexeme;

  auto gen_dec = parse_generic_declaration();
  if (!gen_dec)
    return std::unexpected(gen_dec.error());
  strct->generics = std::move(*gen_dec);

  auto fields = parse_struct_block();
  if (!fields)
    return std::unexpected(fields.error());
  strct->fields = std::move(*fields);

  return strct;
}

std::expected<std::vector<std::unique_ptr<AST>>, ParseError>
Parser::parse_struct_block() {
  if (is_at_end())
    return std::unexpected(ParseError::UnexpectedEOF);
  if (!expect(TokenType::LBRACE))
    return std::unexpected(ParseError::UnexpectedToken);
  consume();

  auto fields = parse_struct_fields();
  if (!fields)
    return std::unexpected(fields.error());

  if (is_at_end())
    return std::unexpected(ParseError::UnexpectedEOF);
  if (!expect(TokenType::RBRACE))
    return std::unexpected(ParseError::UnexpectedToken);
  consume();

  return fields;
}

std::expected<AST *, ParseError> Parser::parse_enum_definition() {
  EnumDef *enm = new EnumDef(peek().loc);

  enm->vis_mod = get_visibility();

  if (expect(TokenType::END_OF_FILE))
    return std::unexpected(ParseError::UnexpectedEOF);
  if (!expect(TokenType::ENUM))
    return std::unexpected(ParseError::UnexpectedToken);
  consume(); // skip ENUM keyword

  if (!expect(TokenType::IDENTIFIER))
    return std::unexpected(ParseError::UnexpectedToken);

  Token name = consume();
  enm->name = name.lexeme;

  auto gen_dec = parse_generic_declaration();
  if (!gen_dec)
    return std::unexpected(gen_dec.error());
  enm->generics = std::move(*gen_dec);

  auto enum_values = parse_enum_block();
  if (!enum_values)
    return std::unexpected(enum_values.error());
  enm->enum_vals = std::move(*enum_values);

  return enm;
}

std::expected<AST *, ParseError> Parser::parse_trait_definition() {
  TraitDef *trait = new TraitDef(peek().loc);

  trait->vis_mod = get_visibility();

  if (expect(TokenType::END_OF_FILE))
    return std::unexpected(ParseError::UnexpectedEOF);
  if (!expect(TokenType::TRAIT))
    return std::unexpected(ParseError::UnexpectedToken);
  consume(); // skip TRAIT keyword

  if (!expect(TokenType::IDENTIFIER))
    return std::unexpected(ParseError::UnexpectedToken);

  Token name = consume();
  trait->name = name.lexeme;

  auto inherits = parse_inherits();
  if (!inherits && *inherits != nullptr)
    return std::unexpected(inherits.error());
  else
    trait->inherits = std::unique_ptr<AST>(*inherits);

  auto body = parse_trait_block();
  if (!body)
    return std::unexpected(body.error());
  trait->contents = std::move(*body);

  return trait;
}

std::expected<AST *, ParseError> Parser::parse_impl_definition() {
  ImplDef *impl = new ImplDef(peek().loc);

  impl->vis_mod = get_visibility();

  if (expect(TokenType::END_OF_FILE))
    return std::unexpected(ParseError::UnexpectedEOF);
  if (expect(TokenType::IMPL))
    return std::unexpected(ParseError::UnexpectedToken);
  consume(); // skip IMPL keyword

  if (!expect(TokenType::IDENTIFIER))
    return std::unexpected(ParseError::UnexpectedToken);

  Token name = consume();

  impl->name = name.lexeme;

  auto generics = parse_generic_declaration();
  if (!generics)
    return std::unexpected(generics.error());
  impl->generics = std::move(*generics);

  auto block = parse_impl_block();
  if (!block)
    return std::unexpected(block.error());
  impl->impl_block = std::move(*block);

  return impl;
}

std::expected<AST *, ParseError> Parser::parse_variable_definition() {
  VariableDef *def = new VariableDef(peek().loc);

  auto decl = parse_variable_declaration();
  if (!decl)
    return std::unexpected(decl.error());
  def->decl = std::unique_ptr<AST>(*decl);

  auto expression = parse_expression();
  if (!expression)
    return std::unexpected(expression.error());
  def->expression = std::unique_ptr<AST>(*expression);

  return def;
}

std::expected<AST *, ParseError> Parser::parse_variable_declaration() {
  VariableDecl *decl = new VariableDecl(peek().loc);

  decl->vis_mod = get_visibility();

  decl->ownership = get_ownership();

  if (is_at_end())
    return std::unexpected(ParseError::UnexpectedEOF);
  if (!expect(TokenType::IDENTIFIER))
    return std::unexpected(ParseError::UnexpectedToken);
  Token name = consume();
  decl->name = name.lexeme;

  auto array_decl = parse_array_def();
  if (!array_decl)
    return std::unexpected(array_decl.error());
  decl->array_size = std::unique_ptr<AST>(*array_decl);

  Token var_type = consume();
  if (is_at_end())
    return std::unexpected(ParseError::UnexpectedEOF);
  Type type = convert_type(var_type.type);

  if (type == Type::ERROR) {
    return std::unexpected(ParseError::UnexpectedToken);
  }

  decl->type = type;

  return decl;
}

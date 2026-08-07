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

/**
 * @brief checks next token to see if it's what we expect
 *
 * @return true if next token matches expectations, false otherwise
 */
bool Parser::expect(TokenType type) {
  if (is_at_end())
    return false;
  return peek().type == type;
}

/**
 * @brief consume a token
 *
 * A token is only consumed if it is of the expected type. When consumption
 * is successful, current is increased.
 *
 * @return consumed token if matching, unexpected otherwise
 */
std::expected<Token, ParseError> Parser::consume(TokenType type) {
  if (is_at_end())
    return std::unexpected(ParseError::UnexpectedEOF);
  if (!expect(type))
    return std::unexpected(ParseError::UnexpectedToken);
  return m_token_stream[current++];
}

/**
 * @brief checks for end of token stream.
 *
 * @return true if end is reached, false otherwise.
 */
bool Parser::is_at_end() { return peek().type == TokenType::END_OF_FILE; }

/**
 * @brief peak at next token in token stream
 *
 * @return the next token without incrementing current token
 */
Token Parser::peek() { return m_token_stream[current]; }

/**
 * @brief peak at token after the next token
 *
 * @return the token after the next in the token stream without increasing
 * current
 */
Token Parser::look_ahead() { return m_token_stream[current + 1]; }

/**
 * @brief Determines visibility
 *
 * Visibility modifiers are optional. If excluded, visibility
 * defaults to private.
 *
 * @return visibility modifier
 */
VisMod Parser::get_visibility() {
  auto pub = consume(TokenType::PUB);
  if (pub)
    return VisMod::PUB;

  // PRIV keyword is optional, so must be checked for. Does not cause failure if
  // not there.
  auto priv = consume(TokenType::PRIV); // Must appease clang-tidy
  return VisMod::PRIV;
}

/**
 * @brief Determines ownership
 *
 * Ownership modifiers are optional keywords. If excluded, ownership
 * will default to OWNED
 *
 * @return ownership modifier
 */
OwnershipMod Parser::get_ownership() {
  static constexpr TokenType tokens[] = {TokenType::REF, TokenType::SHARED,
                                         TokenType::CONST, TokenType::OWNED};

  for (TokenType token : tokens) {
    if (consume(token))
      return convert_ownership(token);
  }
  return OwnershipMod::OWNED;
}

/**
 * @brief Determines type
 *
 * @return Type
 */
Type Parser::get_type() {
  static constexpr TokenType types[] = {
      TokenType::I8,   TokenType::I16,    TokenType::I32,  TokenType::I64,
      TokenType::U8,   TokenType::U16,    TokenType::U32,  TokenType::U64,
      TokenType::BOOL, TokenType::STRING, TokenType::CHAR, TokenType::FLOAT,
      TokenType::VOID};

  for (TokenType token : types) {
    if (consume(token))
      return convert_type(token);
  }

  return Type::ERROR;
}

/**
 * @brief forward facing method to begin parsing process
 *
 * @return the head of the newly created abstract syntax tree
 */
std::unique_ptr<AST> Parser::parse() { return parse_program().value(); }

std::expected<std::unique_ptr<AST>, ParseError> Parser::parse_program() {
  auto program = std::make_unique<Program>(peek().loc);

  while (!is_at_end()) {
    auto top = parse_top_level_decl();

    if (!top)
      handle_parser_error(top.error(), curr_token);

    program->top_level_decls.push_back(std::move(*top));
  }

  return program;
}

std::expected<std::unique_ptr<AST>, ParseError> Parser::parse_top_level_decl() {
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

std::expected<std::unique_ptr<AST>, ParseError> Parser::parse_vismod() {
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

std::expected<std::unique_ptr<AST>, ParseError> Parser::parse_import() {
  // Guaranteed to have the keyword if made to this point, but clang-tidy gets
  // mad
  if (auto keyword = consume(TokenType::IMPORT); !keyword)
    return std::unexpected(keyword.error());

  auto import_name = consume(TokenType::IDENTIFIER);
  if (!import_name)
    return std::unexpected(import_name.error());

  auto import = std::make_unique<ImportStatement>(import_name->loc);
  import->module = import_name->lexeme;
  return import;
}

std::expected<std::unique_ptr<AST>, ParseError>
Parser::parse_function_definition() {
  auto func = std::make_unique<FunctionDef>(peek().loc);

  auto att = consume(TokenType::ATTRIBUTE);
  while (att) {
    func->attributes.emplace_back(new Attribute(att->loc, att->lexeme));
    att = consume(TokenType::ATTRIBUTE);
  }

  func->vis_mod = get_visibility();

  auto keyword = consume(TokenType::FUNC); // skip FUNC keyword
  if (!keyword)
    return std::unexpected(keyword.error());

  auto name = consume(TokenType::IDENTIFIER);
  if (!name)
    return std::unexpected(name.error());

  func->name = name->lexeme;

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

  func->function_return = std::move(*ret);

  auto block = parse_block();
  if (!block)
    return std::unexpected(block.error());
  func->block = std::move(*block);

  return func;
}

std::expected<std::unique_ptr<AST>, ParseError>
Parser::parse_function_declaration() {
  auto func = std::make_unique<FunctionDecl>(peek().loc);

  func->vis_mod = get_visibility();

  auto name = consume(TokenType::IDENTIFIER);
  if (!name)
    return std::unexpected(name.error());
  func->name = name->lexeme;

  auto gen_dec = parse_generic_declaration();
  if (!gen_dec)
    return std::unexpected(gen_dec.error());
  func->generics = std::move(*gen_dec);

  auto param_list = parse_param_list();
  if (!param_list)
    return std::unexpected(param_list.error());
  func->param_list = std::move(*param_list);

  auto function_return = parse_function_return();
  if (!function_return)
    return std::unexpected(function_return.error());
  func->function_return = std::move(*function_return);

  return func;
}

std::expected<std::unique_ptr<AST>, ParseError>
Parser::parse_function_return() {
  auto ret = std::make_unique<FunctionReturn>(peek().loc);

  if (auto point = consume(TokenType::RETURN_POINT); !point)
    return std::unexpected(point.error());

  OwnershipMod owner = get_ownership();
  ret->ownership = owner;

  Type ret_type = get_type();

  if (ret_type == Type::ERROR)
    return std::unexpected(ParseError::UnexpectedToken);
  ret->type = ret_type;

  return ret;
}

std::expected<std::vector<std::unique_ptr<AST>>, ParseError>
Parser::parse_param_list() {
  std::vector<std::unique_ptr<AST>> param_list;

  if (auto paren = consume(TokenType::LPAREN); !paren)
    return std::unexpected(paren.error());

  if (consume(TokenType::RPAREN))
    return param_list;

  do {
    auto param = parse_param();
    if (!param)
      return param_list;
    param_list.push_back(std::move(*param));
  } while (consume(TokenType::COMMA));

  if (auto paren = consume(TokenType::RPAREN); !paren)
    return std::unexpected(paren.error());

  return param_list;
}

std::expected<std::unique_ptr<AST>, ParseError> Parser::parse_param() {
  auto param = std::make_unique<Param>(peek().loc);

  OwnershipMod owner = get_ownership();
  if (owner == OwnershipMod::ERROR)
    return std::unexpected(ParseError::UnexpectedToken);
  param->ownership = owner;

  auto name = consume(TokenType::IDENTIFIER);
  param->name = name->lexeme;

  if (consume(TokenType::LBRACKET)) {
    param->is_array = true;
    if (!consume(TokenType::RBRACKET))
      return std::unexpected(ParseError::UnexpectedToken);
  }

  if (auto colon = consume(TokenType::COLON); !colon)
    return std::unexpected(colon.error());

  Type param_type = get_type();
  if (param_type == Type::ERROR)
    return std::unexpected(ParseError::UnexpectedToken);
  param->type = param_type;

  return param;
}

std::expected<std::unique_ptr<AST>, ParseError>
Parser::parse_struct_definition() {
  auto strct = std::make_unique<StructDef>(peek().loc);

  strct->vis_mod = get_visibility();

  if (auto keyword = consume(TokenType::STRUCT); !keyword)
    return std::unexpected(keyword.error());

  auto name = consume(TokenType::IDENTIFIER);
  if (!name)
    return std::unexpected(name.error());

  strct->name = name->lexeme;

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
  if (auto brace = consume(TokenType::LBRACE); !brace)
    return std::unexpected(brace.error());

  auto fields = parse_struct_fields();
  if (!fields)
    return std::unexpected(fields.error());

  if (auto brace = consume(TokenType::RBRACE); !brace)
    return std::unexpected(brace.error());

  return fields;
}

std::expected<std::vector<std::unique_ptr<AST>>, ParseError>
Parser::parse_struct_fields() {
  std::vector<std::unique_ptr<AST>> fields;

  do {
    auto field = parse_struct_field();
    if (!field)
      return std::unexpected(field.error());
    fields.push_back(std::move(*field));
  } while (!expect(TokenType::RBRACE));

  return fields;
}

std::expected<std::unique_ptr<AST>, ParseError> Parser::parse_struct_field() {
  auto field = std::make_unique<StructField>(peek().loc);

  field->vis_mod = get_visibility();
  field->ownership = get_ownership();

  auto name = consume(TokenType::IDENTIFIER);
  if (!name)
    return std::unexpected(name.error());
  field->name = name->lexeme;

  field->is_array = false;
  if (auto lbracket = consume(TokenType::LBRACKET); !lbracket) {
    field->is_array = true;
    if (auto rbracket = consume(TokenType::RBRACKET); !rbracket)
      return std::unexpected(rbracket.error());
  }

  if (auto colon = consume(TokenType::COLON); !colon)
    return std::unexpected(colon.error());

  Type type = get_type();
  if (type == Type::ERROR)
    return std::unexpected(ParseError::UnexpectedEOF);
  field->type = type;

  return field;
}

std::expected<std::unique_ptr<AST>, ParseError>
Parser::parse_enum_definition() {
  auto enm = std::make_unique<EnumDef>(peek().loc);

  enm->vis_mod = get_visibility();

  if (auto keyword = consume(TokenType::ENUM); !keyword)
    return std::unexpected(keyword.error());

  auto name = consume(TokenType::IDENTIFIER);
  if (!name)
    return std::unexpected(name.error());
  enm->name = name->lexeme;

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

std::expected<std::vector<std::unique_ptr<AST>>, ParseError>
Parser::parse_enum_block() {
  if (auto brace = consume(TokenType::LBRACE); !brace)
    return std::unexpected(brace.error());

  std::vector<std::unique_ptr<AST>> values;

  do {
    auto value = parse_enum_value();
    if (!value)
      return std::unexpected(value.error());
    values.push_back(std::move(*value));
  } while (consume(TokenType::COMMA));

  if (auto brace = consume(TokenType::RBRACE); !brace)
    return std::unexpected(brace.error());
  return values;
}

std::expected<std::unique_ptr<AST>, ParseError> Parser::parse_enum_value() {
  auto value = std::make_unique<EnumValue>(peek().loc);

  auto name = consume(TokenType::IDENTIFIER);
  if (!name)
    return std::unexpected(name.error());
  value->name = name->lexeme;

  if (consume(TokenType::LBRACE)) {
    do {
      auto field = parse_enum_field();
      if (!field)
        return std::unexpected(field.error());
      value->fields.push_back(std::move(*field));
    } while (consume(TokenType::COMMA));

    if (auto brace = consume(TokenType::RBRACE); !brace)
      return std::unexpected(brace.error());
  }

  return value;
}

std::expected<std::unique_ptr<AST>, ParseError> Parser::parse_enum_field() {
  auto field = std::make_unique<EnumField>(peek().loc);

  auto name = consume(TokenType::IDENTIFIER);
  if (!name)
    return std::unexpected(name.error());
  field->name = name->lexeme;

  if (auto colon = consume(TokenType::COLON); !colon)
    return std::unexpected(colon.error());

  Type type = get_type();
  if (type == Type::ERROR)
    return std::unexpected(ParseError::UnexpectedToken);
  field->type = type;

  return field;
}

std::expected<std::unique_ptr<AST>, ParseError>
Parser::parse_trait_definition() {
  auto trait = std::make_unique<TraitDef>(peek().loc);
  trait->vis_mod = get_visibility();

  if (auto keyword = consume(TokenType::TRAIT); !keyword)
    return std::unexpected(keyword.error());

  auto name = consume(TokenType::IDENTIFIER);
  if (!name)
    return std::unexpected(name.error());
  trait->name = name->lexeme;

  auto inherits = parse_inherits();
  if (!inherits && *inherits != nullptr)
    return std::unexpected(inherits.error());
  else
    trait->inherits = std::move(*inherits);

  auto body = parse_trait_block();
  if (!body)
    return std::unexpected(body.error());
  trait->block = std::move(*body);

  return trait;
}

std::expected<std::vector<std::unique_ptr<AST>>, ParseError>
Parser::parse_trait_block() {
  std::vector<std::unique_ptr<AST>> block;

  if (auto brace = consume(TokenType::LBRACE); !brace)
    return std::unexpected(brace.error());

  do {
    auto func = parse_function_declaration();
    if (!func)
      return std::unexpected(func.error());
    block.push_back(std::move(*func));
    if (auto semicolon = consume(TokenType::SEMICOLON); !semicolon)
      return std::unexpected(semicolon.error());
  } while (consume(TokenType::FUNC) || peek().type == TokenType::PRIV ||
           peek().type == TokenType::PUB);

  if (auto brace = consume(TokenType::RBRACE); !brace)
    return std::unexpected(brace.error());

  return block;
}

std::expected<std::unique_ptr<AST>, ParseError>
Parser::parse_impl_definition() {
  auto impl = std::make_unique<ImplDef>(peek().loc);

  impl->vis_mod = get_visibility();

  if (auto keyword = consume(TokenType::IMPL); !keyword)
    return std::unexpected(keyword.error());

  auto name = consume(TokenType::IDENTIFIER);
  if (!name)
    return std::unexpected(name.error());

  if (consume(TokenType::FOR)) {
    impl->trait = name->lexeme;
    auto name2 = consume(TokenType::IDENTIFIER);
    if (!name2)
      return std::unexpected(name2.error());
    impl->name = name2->lexeme;
  } else
    impl->name = name->lexeme;

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

std::expected<std::vector<std::unique_ptr<AST>>, ParseError>
Parser::parse_impl_block() {
  if (auto brace = consume(TokenType::LBRACE); !brace)
    return std::unexpected(brace.error());

  std::vector<std::unique_ptr<AST>> implementations;

  do {
    auto func = parse_function_definition();
    if (!func)
      return std::unexpected(func.error());
    implementations.push_back(std::move(*func));
  } while (consume(TokenType::FUNC) || peek().type == TokenType::PRIV ||
           peek().type == TokenType::PUB);

  if (auto brace = consume(TokenType::RBRACE); !brace)
    return std::unexpected(brace.error());

  return implementations;
}

std::expected<std::unique_ptr<AST>, ParseError>
Parser::parse_variable_definition() {
  auto def = std::make_unique<VariableDef>(peek().loc);

  auto decl = parse_variable_declaration();
  if (!decl)
    return std::unexpected(decl.error());
  def->decl = std::move(*decl);

  auto expression = parse_expression();
  if (!expression)
    return std::unexpected(expression.error());
  def->expression = std::move(*expression);

  return def;
}

std::expected<std::unique_ptr<AST>, ParseError>
Parser::parse_variable_declaration() {
  auto decl = std::make_unique<VariableDecl>(peek().loc);

  decl->vis_mod = get_visibility();

  decl->ownership = get_ownership();

  auto name = consume(TokenType::IDENTIFIER);
  if (!name)
    return std::unexpected(name.error());
  decl->name = name->lexeme;

  auto array_decl = parse_array_def();
  if (!array_decl)
    return std::unexpected(array_decl.error());
  decl->array_size = std::move(*array_decl);

  Type var_type = get_type();
  if (var_type == Type::ERROR)
    return std::unexpected(ParseError::UnexpectedToken);

  decl->type = var_type;

  return decl;
}

std::expected<std::vector<std::unique_ptr<AST>>, ParseError>
Parser::parse_block() {
  if (auto brace = consume(TokenType::LBRACE); !brace)
    return std::unexpected(brace.error());

  std::vector<std::unique_ptr<AST>> block;

  while (!consume(TokenType::RBRACE)) {
    auto statement = parse_statement();
    if (!statement)
      return std::unexpected(statement.error());
    block.push_back(std::move(*statement));
  }

  return block;
}

std::expected<std::unique_ptr<AST>, ParseError> Parser::parse_statement() {
  switch (peek().type) {
  case TokenType::IF:
    return parse_if_statement();
  case TokenType::WHILE:
    return parse_while_statement();
  case TokenType::DO:
    return parse_do_while_statement();
  case TokenType::FOR:
    return parse_for_statement();
  case TokenType::LOOP:
    return parse_loop_statement();
  case TokenType::ASM:
    return parse_assembly_statement();
  case TokenType::MATCH:
    return parse_match_statement();
  default:
    return parse_simple_statement();
  }
}

std::expected<std::unique_ptr<AST>, ParseError> Parser::parse_if_statement() {
  auto if_stmt = std::make_unique<IfStmt>(peek().loc);

  if (auto keyword = consume(TokenType::IF); !keyword)
    return std::unexpected(keyword.error());

  if (auto paren = consume(TokenType::LPAREN); !paren)
    return std::unexpected(paren.error());

  auto exp = parse_expression();
  if (!exp)
    return std::unexpected(exp.error());
  if_stmt->condition = std::move(*exp);

  auto block = parse_block();
  if (!block)
    return std::unexpected(block.error());
  if_stmt->block = std::move(*block);

  return if_stmt;
}

std::expected<std::unique_ptr<AST>, ParseError>
Parser::parse_while_statement() {
  auto while_stmt = std::make_unique<WhileStmt>(peek().loc);

  while_stmt->do_while = false;

  if (auto keyword = consume(TokenType::WHILE); !keyword)
    return std::unexpected(keyword.error());

  if (auto paren = consume(TokenType::LPAREN); !paren)
    return std::unexpected(paren.error());

  auto expr = parse_expression();
  if (!expr)
    return std::unexpected(expr.error());
  while_stmt->loop_condition = std::move(*expr);

  if (auto paren = consume(TokenType::RPAREN); !paren)
    return std::unexpected(paren.error());

  auto block = parse_block();
  if (!block)
    return std::unexpected(block.error());
  while_stmt->block = std::move(*block);

  return while_stmt;
}

std::expected<std::unique_ptr<AST>, ParseError>
Parser::parse_do_while_statement() {
  auto while_stmt = std::make_unique<WhileStmt>(peek().loc);

  while_stmt->do_while = true;

  if (auto keyword = consume(TokenType::DO); !keyword)
    return std::unexpected(keyword.error());

  auto block = parse_block();
  if (!block)
    return std::unexpected(block.error());
  while_stmt->block = std::move(*block);

  if (auto keyword = consume(TokenType::WHILE); !keyword)
    return std::unexpected(keyword.error());

  if (auto paren = consume(TokenType::LPAREN); !paren)
    return std::unexpected(paren.error());

  auto expr = parse_expression();
  if (!expr)
    return std::unexpected(expr.error());
  while_stmt->loop_condition = std::move(*expr);

  if (auto paren = consume(TokenType::RPAREN); !paren)
    return std::unexpected(paren.error());

  return while_stmt;
}

std::expected<std::unique_ptr<AST>, ParseError> Parser::parse_for_statement() {
  auto for_loop = std::make_unique<ForStmt>(peek().loc);

  if (auto paren = consume(TokenType::LPAREN); !paren)
    return std::unexpected(paren.error());

  auto condition = parse_ranged();
  if (!condition) {
    condition = parse_foreach();
    if (!condition)
      return std::unexpected(condition.error());
  }
  for_loop->loop_condition = std::move(*condition);

  if (auto paren = consume(TokenType::RPAREN); !paren)
    return std::unexpected(paren.error());

  auto block = parse_block();
  if (!block)
    return std::unexpected(block.error());
  for_loop->block = std::move(*block);

  return for_loop;
}

std::expected<std::unique_ptr<AST>, ParseError> Parser::parse_ranged() {
  auto range = std::make_unique<Ranged>(peek().loc);

  auto var = parse_variable_declaration();
  if (!var)
    return std::unexpected(var.error());

  auto min_expr = parse_expression();
  if (!min_expr)
    return std::unexpected(min_expr.error());

  auto range_type = consume(TokenType::RANGE);

  if (!range_type) {
    range_type = consume(TokenType::RANGE_INCLUSIVE);
    if (!range_type)
      return std::unexpected(range_type.error());
    range->inclusive = true;
  } else {
    range->inclusive = false;
  }

  auto max_expr = parse_expression();
  if (!max_expr)
    return std::unexpected(max_expr.error());
  range->max_exp = std::move(*max_expr);

  return range;
}

std::expected<std::unique_ptr<AST>, ParseError> Parser::parse_foreach() {
  auto for_each = std::make_unique<ForEach>(peek().loc);

  auto var = parse_variable_declaration();
  if (!var)
    return std::unexpected(var.error());
  for_each->var_decl = std::move(*var);

  if (auto colon = consume(TokenType::COLON); !colon)
    return std::unexpected(colon.error());

  auto mut = parse_mutable();
  if (!mut)
    return std::unexpected(mut.error());
  for_each->mut = std::move(*mut);

  return for_each;
}

std::expected<std::unique_ptr<AST>, ParseError> Parser::parse_loop_statement() {
  if (auto keyword = consume(TokenType::LOOP); !keyword)
    return std::unexpected(keyword.error());

  auto loop = std::make_unique<LoopStmt>(peek().loc);

  auto block = parse_block();
  if (!block)
    return std::unexpected(block.error());
  loop->block = std::move(*block);

  return loop;
}

std::expected<std::unique_ptr<AST>, ParseError>
Parser::parse_assembly_statement() {
  if (auto keyword = consume(TokenType::ASM); !keyword)
    return std::unexpected(keyword.error());

  auto asm_stmt = std::make_unique<AsmStmt>(peek().loc);

  auto block = parse_block();
  if (!block)
    return std::unexpected(block.error());
  asm_stmt->block = std::move(*block);

  return asm_stmt;
}

std::expected<std::unique_ptr<AST>, ParseError>
Parser::parse_match_statement() {
  if (auto keyword = consume(TokenType::MATCH); !keyword)
    return std::unexpected(keyword.error());

  auto match = std::make_unique<MatchStmt>(peek().loc);

  auto mut = parse_mutable();
  if (!mut)
    return std::unexpected(mut.error());
  match->mut = std::move(*mut);

  auto block = parse_match_block();
  if (!block)
    return std::unexpected(block.error());
  match->block = std::move(*block);

  return match;
}

std::expected<std::vector<std::unique_ptr<AST>>, ParseError>
Parser::parse_match_block() {
  if (auto brace = consume(TokenType::LBRACE); !brace)
    return std::unexpected(brace.error());

  auto options = parse_match_options();
  if (!options)
    return std::unexpected(options.error());

  return options;
}

std::expected<std::vector<std::unique_ptr<AST>>, ParseError>
Parser::parse_match_options() {
  std::vector<std::unique_ptr<AST>> options;

  do {
    auto option = parse_match_option();
    if (!option)
      return std::unexpected(option.error());
    options.push_back(std::move(*option));
  } while (!consume(TokenType::RBRACE));

  return options;
}

std::expected<std::unique_ptr<AST>, ParseError> Parser::parse_match_option() {
  auto option = std::make_unique<MatchOption>(peek().loc);
}

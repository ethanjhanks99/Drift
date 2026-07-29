#pragma once

#include "lexer/Token.hpp"
#include "tools/AST.hpp"
#include "tools/OwnershipMod.hpp"
#include "tools/ParseError.hpp"
#include "tools/VisMod.hpp"
#include <expected>
#include <memory>
#include <vector>

class Parser {
public:
  Parser(std::vector<Token> token_stream)
      : m_token_stream(token_stream), curr_token(Token()) {}
  AST *parse();

private:
  std::vector<Token> m_token_stream;
  Token curr_token;
  int current = 0;

  Token consume();
  bool expect(TokenType type);
  bool is_at_end();
  Token peek();
  Token look_ahead();
  VisMod get_visibility();
  OwnershipMod get_ownership();
  std::expected<AST *, ParseError> handle_error();

  std::expected<AST *, ParseError> parse_program();
  std::expected<AST *, ParseError> parse_top_level_decl();
  std::expected<AST *, ParseError> parse_vismod();
  std::expected<AST *, ParseError> parse_import();
  std::expected<AST *, ParseError> parse_function_definition();
  std::expected<AST *, ParseError> parse_function_return();
  std::expected<AST *, ParseError> parse_function_declaration();
  std::expected<AST *, ParseError> parse_struct_definition();
  std::expected<AST *, ParseError> parse_enum_definition();
  std::expected<AST *, ParseError> parse_trait_definition();
  std::expected<AST *, ParseError> parse_impl_definition();
  std::expected<AST *, ParseError> parse_variable_definition();
  std::expected<AST *, ParseError> parse_variable_declaration();
  std::expected<AST *, ParseError> parse_trait_bound();
  std::expected<std::vector<std::unique_ptr<AST>>, ParseError>
  parse_param_list();
  std::expected<AST *, ParseError> parse_param();
  std::expected<AST *, ParseError> parse_type();
  std::expected<AST *, ParseError> parse_inherits();
  std::expected<std::vector<std::unique_ptr<AST>>, ParseError> parse_block();
  std::expected<std::vector<std::unique_ptr<AST>>, ParseError>
  parse_generic_declaration();
  std::expected<AST *, ParseError> parse_struct_block();
  std::expected<std::vector<std::unique_ptr<AST>>, ParseError>
  parse_struct_fields();
  std::expected<AST *, ParseError> parse_struct_field();
  std::expected<AST *, ParseError> parse_array_def();
  std::expected<std::vector<std::unique_ptr<AST>>, ParseError>
  parse_enum_block();
  std::expected<AST *, ParseError> parse_enum_value();
  std::expected<AST *, ParseError> parse_field();
  std::expected<AST *, ParseError> parse_module_access();
  std::expected<std::vector<std::unique_ptr<AST>>, ParseError>
  parse_trait_block();
  std::expected<std::vector<std::unique_ptr<AST>>, ParseError>
  parse_impl_block();
  std::expected<AST *, ParseError> parse_statement();
  std::expected<AST *, ParseError> parse_if_statement();
  std::expected<AST *, ParseError> parse_while_statement();
  std::expected<AST *, ParseError> parse_do_while_statement();
  std::expected<AST *, ParseError> parse_for_statement();
  std::expected<AST *, ParseError> parse_ranged();
  std::expected<AST *, ParseError> parse_foreach();
  std::expected<AST *, ParseError> parse_loop_statement();
  std::expected<AST *, ParseError> parse_assembly_statement();
  std::expected<AST *, ParseError> parse_match_statement();
  std::expected<AST *, ParseError> parse_match_block();
  std::expected<AST *, ParseError> parse_match_options();
  std::expected<AST *, ParseError> parse_match_option();
  std::expected<AST *, ParseError> parse_simple_statement();
  std::expected<AST *, ParseError> parse_assignment();
  std::expected<AST *, ParseError> parse_return_statement();
  std::expected<AST *, ParseError> parse_impl_access();
  std::expected<AST *, ParseError> parse_break_statement();
  std::expected<AST *, ParseError> parse_expression();
  std::expected<AST *, ParseError> parse_or_expression();
  std::expected<AST *, ParseError> parse_and_expression();
  std::expected<AST *, ParseError> parse_bitor_expression();
  std::expected<AST *, ParseError> parse_bitx_expression();
  std::expected<AST *, ParseError> parse_bitand_expression();
  std::expected<AST *, ParseError> parse_bitshift_expression();
  std::expected<AST *, ParseError> parse_sum_expression();
  std::expected<AST *, ParseError> parse_mult_expression();
  std::expected<AST *, ParseError> parse_unary_expression();
  std::expected<AST *, ParseError> parse_post_unary_expression();
  std::expected<AST *, ParseError> parse_power_expression();
  std::expected<AST *, ParseError> parse_paren_expression();
  std::expected<AST *, ParseError> parse_function_call();
  std::expected<AST *, ParseError> parse_calls();
  std::expected<AST *, ParseError> parse_call();
  std::expected<AST *, ParseError> parse_argument_list();
  std::expected<AST *, ParseError> parse_enum_construction();
  std::expected<AST *, ParseError> parse_field_assignment();
  std::expected<AST *, ParseError> parse_struct_type();
  std::expected<AST *, ParseError> parse_generic_definition();
  std::expected<AST *, ParseError> parse_generic_type();
  std::expected<AST *, ParseError> parse_mutable();
  std::expected<AST *, ParseError> parse_array_access();
  std::expected<AST *, ParseError> parse_point_access();
};

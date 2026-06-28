#pragma once

#include "lexer/Token.hpp"
#include "tools/AST.hpp"
#include <memory>
#include <vector>

class Parser {
public:
  Parser(std::vector<Token> token_stream) : m_token_stream(token_stream) {}
  std::unique_ptr<AST> parse();

private:
  std::vector<Token> m_token_stream;
  int current = 0;

  bool consume(Token &token);
  bool expect(Token &token);
  Token advance();
  bool is_at_end();
  Token peek();

  AST *parse_program();
  AST *parse_top_level_decl();
  AST *parse_attribute();
  AST *parse_import();
  AST *parse_function_definition();
  AST *parse_function_return();
  AST *parse_function_declaration();
  AST *parse_struct_definition();
  AST *parse_enum_definition();
  AST *parse_trait_definition();
  AST *parse_impl_definition();
  AST *parse_variable_definition();
  AST *parse_variable_declaration();
  AST *parse_block();
  AST *parse_trait_bound();
  AST *parse_param_list();
  AST *parse_param();
  AST *parse_type();
  AST *parse_generic_declaration();
  AST *parse_struct_block();
  AST *parse_struct_fields();
  AST *parse_struct_field();
  AST *parse_array_def();
  AST *parse_enum_block();
  AST *parse_enum_value();
  AST *parse_field();
  AST *parse_module_access();
  AST *parse_trait_block();
  AST *parse_impl_block();
  AST *parse_statement();
  AST *parse_if_statement();
  AST *parse_while_statement();
  AST *parse_do_while_statement();
  AST *parse_for_statement();
  AST *parse_ranged();
  AST *parse_foreach();
  AST *parse_loop_statement();
  AST *parse_assembly_statement();
  AST *parse_match_statement();
  AST *parse_match_block();
  AST *parse_match_options();
  AST *parse_match_option();
  AST *parse_simple_statement();
  AST *parse_assignment();
  AST *parse_return_statement();
  AST *parse_impl_access();
  AST *parse_break_statement();
  AST *parse_expression();
  AST *parse_or_expression();
  AST *parse_and_expression();
  AST *parse_bitor_expression();
  AST *parse_bitx_expression();
  AST *parse_bitand_expression();
  AST *parse_bitshift_expression();
  AST *parse_sum_expression();
  AST *parse_mult_expression();
  AST *parse_unary_expression();
  AST *parse_post_unary_expression();
  AST *parse_power_expression();
  AST *parse_paren_expression();
  AST *parse_function_call();
  AST *parse_calls();
  AST *parse_call();
  AST *parse_argument_list();
  AST *parse_enum_construction();
  AST *parse_field_assignment();
  AST *parse_struct_type();
  AST *parse_generic_definition();
  AST *parse_generic_type();
  AST *parse_mutable();
  AST *parse_array_access();
  AST *parse_point_access();
};

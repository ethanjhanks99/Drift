#pragma once

#include "ASTNode.hpp"
#include "lexer/Token.hpp"
#include <vector>

class Parser {
public:
  Parser(std::vector<Token> token_stream);
  ASTNode *parse();

private:
  std::vector<Token> m_token_stream;

  void expect();

  ASTNode *parse_program();
  ASTNode *parse_top_level_decl();
  ASTNode *parse_attribute();
  ASTNode *parse_import();
  ASTNode *parse_function_definition();
  ASTNode *parse_function_return();
  ASTNode *parse_function_declaration();
  ASTNode *parse_struct_definition();
  ASTNode *parse_enum_definition();
  ASTNode *parse_trait_definition();
  ASTNode *parse_impl_definition();
  ASTNode *parse_variable_definition();
  ASTNode *parse_variable_declaration();
  ASTNode *parse_block();
  ASTNode *parse_trait_bound();
  ASTNode *parse_param_list();
  ASTNode *parse_param();
  ASTNode *parse_type();
  ASTNode *parse_generic_declaration();
  ASTNode *parse_struct_block();
  ASTNode *parse_struct_fields();
  ASTNode *parse_struct_field();
  ASTNode *parse_array_def();
  ASTNode *parse_enum_block();
  ASTNode *parse_enum_value();
  ASTNode *parse_field();
  ASTNode *parse_module_access();
  ASTNode *parse_trait_block();
  ASTNode *parse_impl_block();
  ASTNode *parse_statement();
  ASTNode *parse_if_statement();
  ASTNode *parse_while_statement();
  ASTNode *parse_do_while_statement();
  ASTNode *parse_for_statement();
  ASTNode *parse_ranged();
  ASTNode *parse_foreach();
  ASTNode *parse_loop_statement();
  ASTNode *parse_assembly_statement();
  ASTNode *parse_match_statement();
  ASTNode *parse_match_block();
  ASTNode *parse_match_options();
  ASTNode *parse_match_option();
  ASTNode *parse_simple_statement();
  ASTNode *parse_assignment();
  ASTNode *parse_return_statement();
  ASTNode *parse_impl_access();
  ASTNode *parse_break_statement();
  ASTNode *parse_expression();
  ASTNode *parse_or_expression();
  ASTNode *parse_and_expression();
  ASTNode *parse_bitor_expression();
  ASTNode *parse_bitx_expression();
  ASTNode *parse_bitand_expression();
  ASTNode *parse_bitshift_expression();
  ASTNode *parse_sum_expression();
  ASTNode *parse_mult_expression();
  ASTNode *parse_unary_expression();
  ASTNode *parse_post_unary_expression();
  ASTNode *parse_power_expression();
  ASTNode *parse_paren_expression();
  ASTNode *parse_function_call();
  ASTNode *parse_calls();
  ASTNode *parse_call();
  ASTNode *parse_argument_list();
  ASTNode *parse_enum_construction();
  ASTNode *parse_field_assignment();
  ASTNode *parse_struct_type();
  ASTNode *parse_generic_definition();
  ASTNode *parse_generic_type();
  ASTNode *parse_mutable();
  ASTNode *parse_array_access();
  ASTNode *parse_point_access();
};

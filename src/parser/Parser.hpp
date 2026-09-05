#pragma once

#include "lexer/Token.hpp"
#include "tools/AST.hpp"
#include "tools/AssignOp.hpp"
#include "tools/BinaryOp.hpp"
#include "tools/OwnershipMod.hpp"
#include "tools/ParseError.hpp"
#include "tools/UnaryOp.hpp"
#include "tools/VisMod.hpp"
#include <expected>
#include <memory>
#include <optional>
#include <vector>

class Parser {
public:
  Parser(std::vector<Token> token_stream)
      : m_token_stream(token_stream), curr_token(Token()) {}
  std::unique_ptr<AST> parse();

private:
  std::vector<Token> m_token_stream;
  Token curr_token;
  int current = 0;

  std::expected<Token, ParseError> consume(TokenType type);
  bool expect(TokenType type);
  bool is_at_end();
  Token peek();
  Token look_ahead();
  VisMod get_visibility();
  OwnershipMod get_ownership();
  std::expected<Type, ParseError> get_type();
  std::expected<AssignOp, ParseError> get_assign_op();
  std::optional<BinaryOp> get_compare_op();
  std::optional<BinaryOp> get_binary_op(TokenType token);
  std::optional<UnaryOp> get_pre_unary_op();
  std::optional<UnaryOp> get_post_unary_op();
  std::unique_ptr<AST> make_binary_node(std::unique_ptr<AST> left,
                                        std::unique_ptr<AST> right,
                                        BinaryOp op);
  std::unique_ptr<AST> make_unary_node(std::unique_ptr<AST> operand, UnaryOp op,
                                       bool prefix);

  std::expected<std::unique_ptr<AST>, ParseError> parse_program();
  std::expected<std::unique_ptr<AST>, ParseError> parse_top_level_decl();
  std::expected<std::unique_ptr<AST>, ParseError> parse_import();
  std::expected<std::unique_ptr<AST>, ParseError>
  parse_function_definition(VisMod vis_mod = VisMod::PRIV);
  std::expected<std::unique_ptr<AST>, ParseError> parse_function_return();
  std::expected<std::unique_ptr<AST>, ParseError>
  parse_function_declaration(VisMod vis_mod = VisMod::PRIV);
  std::expected<std::unique_ptr<AST>, ParseError>
  parse_struct_definition(VisMod vis_mod = VisMod::PRIV);
  std::expected<std::unique_ptr<AST>, ParseError>
  parse_enum_definition(VisMod vis_mod = VisMod::PRIV);
  std::expected<std::unique_ptr<AST>, ParseError>
  parse_trait_definition(VisMod vis_mod = VisMod::PRIV);
  std::expected<std::unique_ptr<AST>, ParseError>
  parse_impl_definition(VisMod vis_mod = VisMod::PRIV);
  std::expected<std::unique_ptr<AST>, ParseError>
  parse_variable_definition(VisMod vis_mod = VisMod::PRIV,
                            OwnershipMod ownership = OwnershipMod::OWNED);
  std::expected<std::unique_ptr<AST>, ParseError>
  parse_variable_declaration(VisMod vis_mod = VisMod::PRIV,
                             OwnershipMod ownership = OwnershipMod::OWNED);
  std::expected<std::unique_ptr<AST>, ParseError> parse_trait_bound();
  std::expected<std::vector<std::unique_ptr<AST>>, ParseError>
  parse_param_list();
  std::expected<std::unique_ptr<AST>, ParseError> parse_param();
  std::expected<std::unique_ptr<AST>, ParseError> parse_inherits();
  std::expected<std::vector<std::unique_ptr<AST>>, ParseError> parse_block();
  std::expected<std::vector<std::unique_ptr<AST>>, ParseError>
  parse_generic_declaration();
  std::expected<std::vector<std::unique_ptr<AST>>, ParseError>
  parse_struct_block();
  std::expected<std::vector<std::unique_ptr<AST>>, ParseError>
  parse_struct_fields();
  std::expected<std::unique_ptr<AST>, ParseError> parse_struct_field();
  std::expected<std::unique_ptr<AST>, ParseError> parse_array_def();
  std::expected<std::vector<std::unique_ptr<AST>>, ParseError>
  parse_enum_block();
  std::expected<std::unique_ptr<AST>, ParseError> parse_enum_value();
  std::expected<std::unique_ptr<AST>, ParseError> parse_enum_field();
  std::expected<std::unique_ptr<AST>, ParseError> parse_module_access();
  std::expected<std::vector<std::unique_ptr<AST>>, ParseError>
  parse_trait_block();
  std::expected<std::vector<std::unique_ptr<AST>>, ParseError>
  parse_impl_block();
  std::expected<std::unique_ptr<AST>, ParseError> parse_statement();
  std::expected<std::unique_ptr<AST>, ParseError> parse_if_statement();
  std::expected<std::unique_ptr<AST>, ParseError> parse_while_statement();
  std::expected<std::unique_ptr<AST>, ParseError> parse_do_while_statement();
  std::expected<std::unique_ptr<AST>, ParseError> parse_for_statement();
  std::expected<std::unique_ptr<AST>, ParseError> parse_ranged();
  std::expected<std::unique_ptr<AST>, ParseError> parse_foreach();
  std::expected<std::unique_ptr<AST>, ParseError> parse_loop_statement();
  std::expected<std::unique_ptr<AST>, ParseError> parse_assembly_statement();
  std::expected<std::unique_ptr<AST>, ParseError> parse_match_statement();
  std::expected<std::vector<std::unique_ptr<AST>>, ParseError>
  parse_match_block();
  std::expected<std::vector<std::unique_ptr<AST>>, ParseError>
  parse_match_options();
  std::expected<std::unique_ptr<AST>, ParseError> parse_match_option();
  std::expected<std::unique_ptr<AST>, ParseError> parse_simple_statement();
  std::expected<std::unique_ptr<AST>, ParseError> parse_assignment();
  std::expected<std::unique_ptr<AST>, ParseError> parse_return_statement();
  std::expected<std::unique_ptr<AST>, ParseError> parse_impl_access();
  std::expected<std::unique_ptr<AST>, ParseError> parse_break_statement();
  std::expected<std::unique_ptr<AST>, ParseError> parse_expression();
  std::expected<std::unique_ptr<AST>, ParseError> parse_or_expression();
  std::expected<std::unique_ptr<AST>, ParseError> parse_and_expression();
  std::expected<std::unique_ptr<AST>, ParseError> parse_bitor_expression();
  std::expected<std::unique_ptr<AST>, ParseError> parse_bitx_expression();
  std::expected<std::unique_ptr<AST>, ParseError> parse_bitand_expression();
  std::expected<std::unique_ptr<AST>, ParseError> parse_bitshift_expression();
  std::expected<std::unique_ptr<AST>, ParseError> parse_sum_expression();
  std::expected<std::unique_ptr<AST>, ParseError> parse_mult_expression();
  std::expected<std::unique_ptr<AST>, ParseError> parse_unary_expression();
  std::expected<std::unique_ptr<AST>, ParseError> parse_post_unary_expression();
  std::expected<std::unique_ptr<AST>, ParseError> parse_power_expression();
  std::expected<std::unique_ptr<AST>, ParseError> parse_paren_expression();
  std::expected<std::unique_ptr<AST>, ParseError> parse_function_call();
  std::expected<std::vector<std::unique_ptr<AST>>, ParseError> parse_calls();
  std::expected<std::unique_ptr<AST>, ParseError> parse_call();
  std::expected<std::vector<std::unique_ptr<AST>>, ParseError>
  parse_argument_list();
  std::expected<std::unique_ptr<AST>, ParseError> parse_enum_construction();
  std::expected<std::unique_ptr<AST>, ParseError> parse_field_assignment();
  std::expected<std::unique_ptr<AST>, ParseError> parse_struct_type();
  std::expected<std::unique_ptr<AST>, ParseError> parse_generic_definition();
  std::expected<std::unique_ptr<AST>, ParseError> parse_generic_type();
  std::expected<std::unique_ptr<AST>, ParseError> parse_mutable();
  std::expected<std::unique_ptr<AST>, ParseError> parse_immutable();
  std::expected<std::unique_ptr<AST>, ParseError> parse_array_access();
  std::expected<std::unique_ptr<AST>, ParseError> parse_point_access();
};

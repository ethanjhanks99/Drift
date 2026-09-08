#pragma once

#include "tools/AST.hpp"
#include "tools/AssignOp.hpp"
#include "tools/OwnershipMod.hpp"
#include "tools/SourceLocation.hpp"
#include "tools/Type.hpp"
#include "tools/VisMod.hpp"
#include <memory>
#include <string>
#include <vector>

std::unique_ptr<AST> make_program_node(SourceLocation loc,
                                       std::vector<std::unique_ptr<AST>> decls);
std::unique_ptr<AST> make_import_node(SourceLocation loc,
                                      std::string module_name);
std::unique_ptr<AST> make_attribute_node(SourceLocation loc, std::string name);
std::unique_ptr<AST>
make_func_def_node(SourceLocation loc, VisMod vis_mod, std::string name,
                   std::vector<std::unique_ptr<AST>> generics,
                   std::vector<std::unique_ptr<AST>> params,
                   std::unique_ptr<AST> function_return,
                   std::vector<std::unique_ptr<AST>> block);
std::unique_ptr<AST>
make_func_decl_node(SourceLocation loc, VisMod vis_mod, std::string name,
                    std::vector<std::unique_ptr<AST>> generics,
                    std::vector<std::unique_ptr<AST>> params,
                    std::unique_ptr<AST> function_return);
std::unique_ptr<AST> make_function_return_node(SourceLocation loc,
                                               OwnershipMod ownership,
                                               Type type);
std::unique_ptr<AST> make_param_node(SourceLocation loc, OwnershipMod ownership,
                                     std::string name, bool is_array,
                                     Type type);
std::unique_ptr<AST>
make_struct_node(SourceLocation loc, VisMod vis_mod, std::string name,
                 std::vector<std::unique_ptr<AST>> generics,
                 std::vector<std::unique_ptr<AST>> fields);
std::unique_ptr<AST> make_struct_field_node(SourceLocation loc, VisMod vis_mod,
                                            OwnershipMod ownership,
                                            std::string name, bool is_array,
                                            Type type);
std::unique_ptr<AST>
make_enum_node(SourceLocation loc, VisMod vis_mod, std::string name,
               std::vector<std::unique_ptr<AST>> generics,
               std::vector<std::unique_ptr<AST>> enum_vals);
std::unique_ptr<AST>
make_enum_value_node(SourceLocation loc, std::string name,
                     std::vector<std::unique_ptr<AST>> fields);
std::unique_ptr<AST> make_value_field_node(SourceLocation loc, std::string name,
                                           Type type);
std::unique_ptr<AST> make_trait_node(SourceLocation loc, VisMod vis_mod,
                                     std::string name,
                                     std::unique_ptr<AST> inherits,
                                     std::vector<std::unique_ptr<AST>> body);
std::unique_ptr<AST> make_impl_node(SourceLocation loc, VisMod vis_mod,
                                    std::string struct_name,
                                    std::string trait_name,
                                    std::vector<std::unique_ptr<AST>> generics,
                                    std::vector<std::unique_ptr<AST>> block,
                                    bool with_trait);
std::unique_ptr<AST> make_var_def_node(SourceLocation loc,
                                       std::unique_ptr<AST> var_decl,
                                       std::unique_ptr<AST> expression);
std::unique_ptr<AST> make_var_decl_node(SourceLocation loc, VisMod vis_mod,
                                        OwnershipMod ownership,
                                        std::string name,
                                        std::unique_ptr<AST> array_size,
                                        Type type);
std::unique_ptr<AST> make_if_node(SourceLocation loc,
                                  std::unique_ptr<AST> condition,
                                  std::vector<std::unique_ptr<AST>> block);
std::unique_ptr<AST> make_while_node(SourceLocation loc, bool do_while,
                                     std::unique_ptr<AST> condition,
                                     std::vector<std::unique_ptr<AST>> block);
std::unique_ptr<AST> make_for_node(SourceLocation loc,
                                   std::unique_ptr<AST> condition,
                                   std::vector<std::unique_ptr<AST>> block);
std::unique_ptr<AST> make_ranged_node(SourceLocation loc,
                                      std::unique_ptr<AST> var,
                                      std::unique_ptr<AST> min_expr,
                                      bool inclusive,
                                      std::unique_ptr<AST> max_expr);
std::unique_ptr<AST> make_foreach_node(SourceLocation loc,
                                       std::unique_ptr<AST> var,
                                       std::unique_ptr<AST> mut);
std::unique_ptr<AST> make_loop_node(SourceLocation loc,
                                    std::vector<std::unique_ptr<AST>> block);
std::unique_ptr<AST> make_asm_node(SourceLocation loc,
                                   std::vector<std::unique_ptr<AST>> block);
std::unique_ptr<AST> make_match_node(SourceLocation loc,
                                     std::unique_ptr<AST> mut,
                                     std::vector<std::unique_ptr<AST>> block);
std::unique_ptr<AST>
make_match_option_node(SourceLocation loc, std::unique_ptr<AST> expr,
                       std::vector<std::unique_ptr<AST>> block);
std::unique_ptr<AST> make_assignment_node(SourceLocation loc,
                                          std::unique_ptr<AST> mut, AssignOp op,
                                          std::unique_ptr<AST> expr);
std::unique_ptr<AST> make_return_node(SourceLocation loc,
                                      std::unique_ptr<AST> ret_value);
std::unique_ptr<AST> make_break_node(SourceLocation loc,
                                     std::unique_ptr<AST> break_value);
std::unique_ptr<AST> make_binary_node(SourceLocation loc,
                                      std::unique_ptr<AST> left,
                                      std::unique_ptr<AST> right, BinaryOp op);
std::unique_ptr<AST> make_unary_node(SourceLocation loc,
                                     std::unique_ptr<AST> operand, UnaryOp op,
                                     bool prefix);

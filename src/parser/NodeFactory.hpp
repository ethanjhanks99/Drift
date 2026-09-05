#pragma once

#include "tools/AST.hpp"
#include "tools/OwnershipMod.hpp"
#include "tools/SourceLocation.hpp"
#include "tools/Type.hpp"
#include "tools/VisMod.hpp"
#include <memory>
#include <vector>

std::unique_ptr<AST> make_program_node(SourceLocation loc,
                                       std::vector<std::unique_ptr<AST>> decls);
std::unique_ptr<AST> make_import_node(SourceLocation loc,
                                      std::string module_name);
std::unique_ptr<AST>
make_func_def_node(SourceLocation loc,
                   std::vector<std::unique_ptr<AST>> attributes, VisMod vis_mod,
                   std::string name, std::vector<std::unique_ptr<AST>> generics,
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

#pragma once

#include "tools/NodeType.hpp"
#include "tools/SourceLocation.hpp"
#include <memory>
#include <string>
#include <vector>

struct AST {
  NodeType type;
  SourceLocation loc;
};

struct Program : AST {
  std::vector<std::unique_ptr<AST>> top_level_decls;
};

struct Attribute : AST {
  std::string name;
};

struct ImportStatement : AST {
  std::string module;
};

struct FunctionDef {
  std::vector<std::unique_ptr<AST>> attributes;
  // VisMod vis_mod;
  std::string name;
  bool generic;
  std::vector<std::unique_ptr<AST>> param_list;
  std::unique_ptr<AST> function_return;
  std::unique_ptr<AST> block;
};

struct FunctionDecl {
  // VisMod vis_mod;
  std::string name;
  bool generic;
  std::vector<std::unique_ptr<AST>> param_list;
  std::unique_ptr<AST> function_return;
};

struct FunctionReturn {
  // OwnershipMod ownership;
  // Type type;
};

struct Param {
  // OwnershipMod ownership;
  std::string name;
  bool is_array;
  // Type type;
};

struct StructDef {
  // VisMod vis_mdod;
  std::string name;
  bool generic;
  std::vector<std::unique_ptr<AST>> fields;
};

struct StructField {
  // VisMod vis_mod;
  // OwnershipMod ownership;
  std::string name;
  bool is_array;
  // Type type;
};

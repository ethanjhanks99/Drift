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

struct FunctionDef : AST {
  std::vector<std::unique_ptr<AST>> attributes;
  // VisMod vis_mod;
  std::string name;
  bool generic;
  int gen_count;
  std::vector<std::unique_ptr<AST>> param_list;
  std::unique_ptr<AST> function_return;
  std::unique_ptr<AST> block;
};

struct FunctionDecl : AST {
  // VisMod vis_mod;
  std::string name;
  bool generic;
  int gen_count;
  std::vector<std::unique_ptr<AST>> param_list;
  std::unique_ptr<AST> function_return;
};

struct FunctionReturn : AST {
  // OwnershipMod ownership;
  // Type type;
};

struct Param : AST {
  // OwnershipMod ownership;
  std::string name;
  bool is_array;
  // Type type;
};

struct StructDef : AST {
  // VisMod vis_mdod;
  std::string name;
  bool generic;
  int gen_count;
  std::vector<std::unique_ptr<AST>> fields;
};

struct StructField : AST {
  // VisMod vis_mod;
  // OwnershipMod ownership;
  std::string name;
  bool is_array;
  // Type type;
};

struct EnumDef : AST {
  std::string name;
  bool generic;
  int gen_count;
  std::vector<std::unique_ptr<AST>> enum_vals;
};

struct EnumValue : AST {
  std::string name;
  std::vector<std::unique_ptr<AST>> fields;
};

struct EnumField : AST {
  std::string name;
  // Type type;
};

struct TraitDef : AST {
  std::string name;
  std::unique_ptr<AST> inherits;
  std::vector<std::unique_ptr<AST>> contents;
};

struct ImplDef : AST {
  std::string name;
};

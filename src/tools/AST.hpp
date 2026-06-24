#pragma once

#include "tools/BinaryOp.hpp"
#include "tools/NodeType.hpp"
#include "tools/OwnershipMod.hpp"
#include "tools/SourceLocation.hpp"
#include "tools/Type.hpp"
#include "tools/UnaryOp.hpp"
#include "tools/VisMod.hpp"
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
  VisMod vis_mod;
  std::string name;
  int gen_count;
  std::vector<std::unique_ptr<AST>> param_list;
  std::unique_ptr<AST> function_return;
  std::vector<std::unique_ptr<AST>> block;
};

struct FunctionDecl : AST {
  VisMod vis_mod;
  std::string name;
  int gen_count;
  std::vector<std::unique_ptr<AST>> param_list;
  std::unique_ptr<AST> function_return;
};

struct FunctionReturn : AST {
  OwnershipMod ownership;
  Type type;
};

struct Param : AST {
  OwnershipMod ownership;
  std::string name;
  bool is_array;
  Type type;
};

struct StructDef : AST {
  VisMod vis_mdod;
  std::string name;
  int gen_count;
  std::vector<std::unique_ptr<AST>> fields;
};

struct StructField : AST {
  VisMod vis_mod;
  OwnershipMod ownership;
  std::string name;
  bool is_array;
  Type type;
};

struct EnumDef : AST {
  std::string name;
  int gen_count;
  std::vector<std::unique_ptr<AST>> enum_vals;
};

struct EnumValue : AST {
  std::string name;
  std::vector<std::unique_ptr<AST>> fields;
};

struct EnumField : AST {
  std::string name;
  Type type;
};

struct TraitDef : AST {
  std::string name;
  std::unique_ptr<AST> inherits;
  std::vector<std::unique_ptr<AST>> contents;
};

struct ImplDef : AST {
  std::string name;
  std::string trait;
  int gen_count;
  std::vector<std::string> gen_types;
  std::vector<std::unique_ptr<AST>> impl_block;
};

struct IfStmt : AST {
  std::unique_ptr<AST> condition;
  std::vector<std::unique_ptr<AST>> block;
};

struct WhileStmt : AST {
  bool do_while;
  std::unique_ptr<AST> loop_condition;
  std::vector<std::unique_ptr<AST>> block;
};

struct ForStmt : AST {
  std::unique_ptr<AST> loop_condition;
  std::vector<std::unique_ptr<AST>> block;
};

struct LoopStmt : AST {
  std::vector<std::unique_ptr<AST>> block;
};

struct MatchStmt : AST {
  std::unique_ptr<AST> mut;
  std::vector<std::unique_ptr<AST>> block;
};

struct MatchOption : AST {
  std::unique_ptr<AST> comp;
  std::vector<std::unique_ptr<AST>> block;
};

struct VariableDecl : AST {
  OwnershipMod ownership;
  std::string name;
  std::unique_ptr<AST> array_size;
  Type type;
};

struct VariableDef : AST {
  std::unique_ptr<AST> expression;
};

struct Return : AST {
  std::unique_ptr<AST> return_value;
};

struct Break : AST {
  std::unique_ptr<AST> break_value;
};

struct BinaryExpr : AST {
  BinaryOp op;
  std::unique_ptr<AST> left;
  std::unique_ptr<AST> right;
};

struct UnaryExpr : AST {
  UnaryOp op;
  std::unique_ptr<AST> operand;
};

struct Mutable : AST {
  std::string name;
  std::unique_ptr<AST> array_access;
  std::unique_ptr<AST> point_access;
  Type type;
};

struct Immutable : AST {
  std::unique_ptr<AST> value;
};

struct Call : AST {
  std::unique_ptr<AST> module_access;
  std::unique_ptr<AST> impl_access;
  std::vector<std::unique_ptr<AST>> args;
  std::string name;
};

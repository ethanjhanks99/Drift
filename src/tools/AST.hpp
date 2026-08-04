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

  AST(NodeType node_type, SourceLocation location)
      : type(node_type), loc(location) {}
};

struct Program : AST {
  std::vector<std::unique_ptr<AST>> top_level_decls;

  Program(SourceLocation loc) : AST(NodeType::PROGRAM, loc) {}
};

struct ImportStatement : AST {
  std::string module;

  ImportStatement(SourceLocation loc) : AST(NodeType::IMPORT_STATE, loc) {}
};

struct FunctionDef : AST {
  std::vector<std::unique_ptr<AST>> attributes;
  VisMod vis_mod;
  std::string name;
  std::vector<std::unique_ptr<AST>> generics;
  std::vector<std::unique_ptr<AST>> param_list;
  std::unique_ptr<AST> function_return;
  std::vector<std::unique_ptr<AST>> block;

  FunctionDef(SourceLocation loc) : AST(NodeType::FUNCTION_DEF, loc) {}
};

struct Attribute : AST {
  std::string name;

  Attribute(SourceLocation loc, std::string at_name)
      : AST(NodeType::ATTRIBUTE, loc), name(at_name) {}
};

struct FunctionDecl : AST {
  VisMod vis_mod;
  std::string name;
  std::vector<std::unique_ptr<AST>> generics;
  std::vector<std::unique_ptr<AST>> param_list;
  std::unique_ptr<AST> function_return;

  FunctionDecl(SourceLocation loc) : AST(NodeType::FUNCTION_DECL, loc) {}
};

struct FunctionReturn : AST {
  OwnershipMod ownership;
  Type type;

  FunctionReturn(SourceLocation loc) : AST(NodeType::FUNCTION_TYPE, loc) {}
};

struct Param : AST {
  OwnershipMod ownership;
  std::string name;
  bool is_array;
  Type type;

  Param(SourceLocation loc) : AST(NodeType::PARAM, loc) {}
};

struct StructDef : AST {
  VisMod vis_mod;
  std::string name;
  std::vector<std::unique_ptr<AST>> generics;
  std::vector<std::unique_ptr<AST>> fields;

  StructDef(SourceLocation loc) : AST(NodeType::STRUCT_DEF, loc) {}
};

struct StructField : AST {
  VisMod vis_mod;
  OwnershipMod ownership;
  std::string name;
  bool is_array;
  Type type;

  StructField(SourceLocation loc) : AST(NodeType::FIELD, loc) {}
};

struct EnumDef : AST {
  VisMod vis_mod;
  std::string name;
  std::vector<std::unique_ptr<AST>> generics;
  std::vector<std::unique_ptr<AST>> enum_vals;

  EnumDef(SourceLocation loc) : AST(NodeType::ENUM_DEF, loc) {}
};

struct EnumValue : AST {
  std::string name;
  std::vector<std::unique_ptr<AST>> fields;

  EnumValue(SourceLocation loc) : AST(NodeType::ENUM_VALUE, loc) {}
};

struct EnumField : AST {
  std::string name;
  Type type;

  EnumField(SourceLocation loc) : AST(NodeType::FIELD, loc) {}
};

struct TraitDef : AST {
  VisMod vis_mod;
  std::string name;
  std::unique_ptr<AST> inherits;
  std::vector<std::unique_ptr<AST>> block;

  TraitDef(SourceLocation loc) : AST(NodeType::TRAIT_DEF, loc) {}
};

struct ImplDef : AST {
  VisMod vis_mod;
  std::string name;
  std::string trait;
  std::vector<std::unique_ptr<AST>> generics;
  std::vector<std::string> gen_types;
  std::vector<std::unique_ptr<AST>> impl_block;

  ImplDef(SourceLocation loc) : AST(NodeType::IMPL_DEF, loc) {}
};

struct IfStmt : AST {
  std::unique_ptr<AST> condition;
  std::vector<std::unique_ptr<AST>> block;

  IfStmt(SourceLocation loc) : AST(NodeType::IF_STATE, loc) {}
};

struct WhileStmt : AST {
  bool do_while;
  std::unique_ptr<AST> loop_condition;
  std::vector<std::unique_ptr<AST>> block;

  WhileStmt(SourceLocation loc) : AST(NodeType::WHILE_STATE, loc) {}
};

struct ForStmt : AST {
  std::unique_ptr<AST> loop_condition;
  std::vector<std::unique_ptr<AST>> block;

  ForStmt(SourceLocation loc) : AST(NodeType::FOR_STATE, loc) {}
};

struct LoopStmt : AST {
  std::vector<std::unique_ptr<AST>> block;

  LoopStmt(SourceLocation loc) : AST(NodeType::LOOP_STATE, loc) {}
};

struct MatchStmt : AST {
  std::unique_ptr<AST> mut;
  std::vector<std::unique_ptr<AST>> block;

  MatchStmt(SourceLocation loc) : AST(NodeType::MATCH_STATE, loc) {}
};

struct MatchOption : AST {
  std::unique_ptr<AST> comp;
  std::vector<std::unique_ptr<AST>> block;

  MatchOption(SourceLocation loc) : AST(NodeType::MATCH_OPTIONS, loc) {}
};

struct VariableDef : AST {
  std::unique_ptr<AST> decl;
  std::unique_ptr<AST> expression;

  VariableDef(SourceLocation loc) : AST(NodeType::VARIABLE_DEF, loc) {}
};

struct VariableDecl : AST {
  VisMod vis_mod;
  OwnershipMod ownership;
  std::string name;
  std::unique_ptr<AST> array_size;
  Type type;

  VariableDecl(SourceLocation loc) : AST(NodeType::VARIABLE_DECL, loc) {}
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

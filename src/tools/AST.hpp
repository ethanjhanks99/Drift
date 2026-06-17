#pragma once

#include "tools/NodeType.hpp"
#include "tools/SourceLocation.hpp"
#include <memory>
#include <vector>

struct AST {
  NodeType type;
  SourceLocation loc;
};

struct Program : AST {
  std::vector<std::unique_ptr<AST>> top_level_decls;
};

#pragma once

#include "tools/NodeType.hpp"
#include "tools/SourceLocation.hpp"

struct AST {
  NodeType type;
  SourceLocation loc;
};

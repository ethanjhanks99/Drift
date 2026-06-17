#pragma once

#include <ostream>
#include <string>

struct SourceLocation {
  uint line;
  uint column;
  std::string file;

  SourceLocation(uint l, uint c, std::string f) : line(l), column(c), file(f) {}

  friend std::ostream &operator<<(std::ostream &os, const SourceLocation loc);
};

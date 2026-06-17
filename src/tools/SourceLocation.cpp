#include "SourceLocation.hpp"
#include <iostream>

std::ostream &operator<<(std::ostream &os, const SourceLocation loc) {
  std::cout << "Line: " << loc.line << " Column: " << loc.column
            << " in File: " << loc.file << std::endl;

  return os;
}

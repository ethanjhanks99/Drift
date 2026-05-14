#include "lexer/Lexer.hpp"
#include "lexer/Token.hpp"
#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators.hpp>

const std::string source = "func main() {\n\
\tint num1 := 4;\n\
\tstring msg = \"hello world\"\n\
\tint ";

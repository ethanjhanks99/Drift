#pragma once

#include <string>

enum class UnaryOp { NOT, BNOT, PPLUS, SSUB, NEG, QMARK, ERROR };

UnaryOp convert_unary(std::string lexeme);

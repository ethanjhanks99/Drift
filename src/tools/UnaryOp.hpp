#pragma once

#include "lexer/Token.hpp"

enum class UnaryOp { NOT, BNOT, PPLUS, SSUB, NEG, QMARK, ERROR };

UnaryOp convert_unary(TokenType token);

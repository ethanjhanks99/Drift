#pragma once

#include "lexer/Token.hpp"
#include "tools/ParseError.hpp"

void handle_parser_error(ParseError error, Token token);

void handle_unexpected_token(Token token);
void handle_unexpected_eof(Token token);

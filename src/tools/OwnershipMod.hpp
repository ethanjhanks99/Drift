#pragma once

#include "lexer/Token.hpp"
enum class OwnershipMod { OWNED, REF, SHARED, CONST, ERROR };

OwnershipMod convert_ownership(TokenType type);

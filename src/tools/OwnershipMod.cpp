#include "OwnershipMod.hpp"
#include "lexer/Token.hpp"

OwnershipMod convert_ownership(TokenType type) {
  switch (type) {
  case TokenType::SHARED:
    return OwnershipMod::SHARED;
  case TokenType::REF:
    return OwnershipMod::REF;
  case TokenType::CONST:
    return OwnershipMod::CONST;
  case TokenType::END_OF_FILE:
    return OwnershipMod::ERROR;
  default:
    return OwnershipMod::OWNED;
  }
}

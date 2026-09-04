#include "Parser.hpp"
#include "lexer/Token.hpp"
#include "tools/AST.hpp"
#include "tools/AssignOp.hpp"
#include "tools/BinaryOp.hpp"
#include "tools/OwnershipMod.hpp"
#include "tools/ParseError.hpp"
#include "tools/VisMod.hpp"
#include <expected>
#include <memory>
#include <optional>
#include <vector>

/**
 * @brief checks next token to see if it's what we expect
 *
 * @params
 * type (TokenType): expected type of the token being checked
 *
 * @return true if next token matches expectations, false otherwise
 */
bool Parser::expect(TokenType type) {
  if (is_at_end())
    return false;
  return peek().type == type;
}

/**
 * @brief consume a token
 *
 * A token is only consumed if it is of the expected type. When consumption
 * is successful, current is increased.
 *
 * @params
 * type (TokenType): expected type of the token we are consuming
 *
 * @return consumed token if matching, unexpected otherwise
 */
std::expected<Token, ParseError> Parser::consume(TokenType type) {
  if (is_at_end())
    return std::unexpected(ParseError::UnexpectedEOF);
  if (!expect(type))
    return std::unexpected(ParseError::UnexpectedToken);
  return m_token_stream[current++];
}

/**
 * @brief checks for end of token stream.
 *
 * @return true if end is reached, false otherwise.
 */
bool Parser::is_at_end() { return peek().type == TokenType::END_OF_FILE; }

/**
 * @brief peak at next token in token stream
 *
 * @return the next token without incrementing current token
 */
Token Parser::peek() { return m_token_stream[current]; }

/**
 * @brief peak at token after the next token
 *
 * @return the token after the next in the token stream without increasing
 * current
 */
Token Parser::look_ahead() { return m_token_stream[current + 1]; }

/**
 * @brief Determines visibility
 *
 * Visibility modifiers are optional. If excluded, visibility
 * defaults to private.
 *
 * @return visibility modifier
 */
VisMod Parser::get_visibility() {
  auto pub = consume(TokenType::PUB);
  if (pub)
    return VisMod::PUB;

  // PRIV keyword is optional, so must be checked for. Does not cause failure if
  // not there.
  auto priv = consume(TokenType::PRIV); // Must appease clang-tidy
  return VisMod::PRIV;
}

/**
 * @brief Determines ownership
 *
 * Ownership modifiers are optional keywords. If excluded, ownership
 * will default to OWNED
 *
 * @return ownership modifier
 */
OwnershipMod Parser::get_ownership() {
  static constexpr TokenType tokens[] = {TokenType::REF, TokenType::SHARED,
                                         TokenType::CONST, TokenType::OWNED};

  for (TokenType token : tokens) {
    if (consume(token))
      return convert_ownership(token);
  }
  return OwnershipMod::OWNED;
}

/**
 * @brief Determines type
 *
 * @return Type
 */
std::expected<Type, ParseError> Parser::get_type() {
  static constexpr TokenType types[] = {
      TokenType::I8,   TokenType::I16,    TokenType::I32,  TokenType::I64,
      TokenType::U8,   TokenType::U16,    TokenType::U32,  TokenType::U64,
      TokenType::BOOL, TokenType::STRING, TokenType::CHAR, TokenType::FLOAT,
      TokenType::VOID};

  for (TokenType token : types) {
    if (consume(token))
      return convert_type(token);
  }

  if (consume(TokenType::END_OF_FILE))
    return std::unexpected(ParseError::UnexpectedEOF);
  return std::unexpected(ParseError::UnexpectedToken);
}

/**
 * @brief determines AssignOp from TokenType
 *
 * @return AssignOp, unexpected if failure
 */
std::expected<AssignOp, ParseError> Parser::get_assign_op() {
  static constexpr TokenType assign_types[] = {
      TokenType::ASSIGN, TokenType::PLUS_EQUALS, TokenType::MINUS_EQUALS,
      TokenType::MULT_EQUALS, TokenType::DIVIDE_EQUALS};

  for (TokenType token : assign_types) {
    if (consume(token))
      return convert_assign(token);
  }

  if (peek().type == TokenType::END_OF_FILE)
    return std::unexpected(ParseError::UnexpectedEOF);
  return std::unexpected(ParseError::UnexpectedToken);
}

/**
 * @brief get comparative operator
 *
 * @return BinaryOp for comparative operation
 */
std::optional<BinaryOp> Parser::get_compare_op() {
  static constexpr TokenType types[] = {
      TokenType::EQUAL,       TokenType::NOT_EQUAL, TokenType::GREAT,
      TokenType::GREAT_EQUAL, TokenType::LESS,      TokenType::LESS_EQUAL};

  for (TokenType type : types) {
    if (consume(type))
      return convert_binary(type);
  }

  return {};
}

/**
 * @brief get binary operations
 *
 * @params
 * token (TokenType): The type of token from which we will determine the binary
 *                    operator
 *
 * @return BinaryOp that matches the token param
 */
std::optional<BinaryOp> Parser::get_binary_op(TokenType token) {
  auto op = consume(token);
  if (!op)
    return {};

  return convert_binary(token);
}

/**
 * @brief create a binary comparison AST node
 *
 * @params
 * left (AST):        the left side of the operation
 * right (AST):       the right side of the operation
 * op (BinaryOp):     the comparative operation
 *
 * @return BinaryExpr AST node
 */
std::unique_ptr<AST> Parser::make_comp_node(std::unique_ptr<AST> left,
                                            std::unique_ptr<AST> right,
                                            BinaryOp op) {
  auto new_node = std::make_unique<BinaryExpr>(left->loc);
  new_node->left = std::move(left);
  new_node->right = std::move(right);
  new_node->op = op;

  return new_node;
}

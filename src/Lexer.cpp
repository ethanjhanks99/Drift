#include "Lexer.hpp"
#include "Token.hpp"

Lexer::Lexer(std::string m_source) {
  source = m_source;

  keywords["if"] = TokenType::IF;
  keywords["while"] = TokenType::WHILE;
  keywords["else"] = TokenType::ELSE;
  keywords["func"] = TokenType::FUNC;
  keywords["int"] = TokenType::I32;
  keywords["i8"] = TokenType::I8;
  keywords["i16"] = TokenType::I16;
  keywords["i32"] = TokenType::I32;
  keywords["i64"] = TokenType::I64;
  keywords["uint"] = TokenType::U32;
  keywords["u8"] = TokenType::U8;
  keywords["u16"] = TokenType::U16;
  keywords["u32"] = TokenType::U32;
  keywords["u64"] = TokenType::U64;
  keywords["float"] = TokenType::FLOAT;
  keywords["void"] = TokenType::VOID;
  keywords["string"] = TokenType::STRING;
  keywords["char"] = TokenType::CHAR;
  keywords["bool"] = TokenType::BOOL;
  keywords["struct"] = TokenType::STRUCT;
  keywords["true"] = TokenType::TRUE;
  keywords["false"] = TokenType::FALSE;
  keywords["then"] = TokenType::THEN;
  keywords["do"] = TokenType::DO;
  keywords["return"] = TokenType::RETURN;
  keywords["var"] = TokenType::VAR;
  keywords["for"] = TokenType::FOR;
  keywords["ref"] = TokenType::REF;
  keywords["owned"] = TokenType::OWNED;
  keywords["shared"] = TokenType::SHARED;
  keywords["const"] = TokenType::CONST;
  keywords["enum"] = TokenType::ENUM;
  keywords["trait"] = TokenType::TRAIT;
  keywords["impl"] = TokenType::IMPL;
  keywords["match"] = TokenType::MATCH;
  keywords["loop"] = TokenType::LOOP;
  keywords["break"] = TokenType::BREAK;
  keywords["continue"] = TokenType::CONTINUE;
  keywords["import"] = TokenType::IMPORT;
  keywords["pub"] = TokenType::PUB;
  keywords["priv"] = TokenType::PRIV;
  keywords["unsafe"] = TokenType::UNSAFE;
  keywords["static"] = TokenType::STATIC;
  keywords["panic"] = TokenType::PANIC;
  keywords["channel"] = TokenType::CHANNEL;
  keywords["atomic"] = TokenType::ATOMIC;
  keywords["in"] = TokenType::IN;
  keywords["asm"] = TokenType::ASM;
}

std::vector<Token> Lexer::scanTokens() {
  while (!isAtEnd()) {
    start = current;
    scanToken();
  }

  tokens.push_back(Token(TokenType::END_OF_FILE, "", line, start));

  return tokens;
}

bool Lexer::isAtEnd() { return current >= source.size(); }

char Lexer::advance() { return source[current++]; }

char Lexer::peek() {
  if (isAtEnd())
    return '\0';
  return source[current];
}

char Lexer::peekNext() {
  if (current + 1 >= source.size())
    return '\0';
  return source[current + 1];
}

bool Lexer::match(char expected) {
  if (isAtEnd())
    return false;
  if (source[current] != expected)
    return false;

  current++;
  return true;
}

void Lexer::addToken(TokenType type) {
  std::string lexeme = source.substr(start, current - start);
  tokens.push_back(Token(type, lexeme, line, start));
}

void Lexer::addToken(TokenType type, std::string literal) {
  tokens.push_back(Token(type, literal, line, start));
}

void Lexer::scanToken() {
  char c = advance();

  switch (c) {
  case '(':
    addToken(TokenType::LPAREN);
    break;
  case ')':
    addToken(TokenType::RPAREN);
    break;
  case '[':
    addToken(TokenType::LBRACKET);
    break;
  case ']':
    addToken(TokenType::RBRACKET);
    break;
  case '{':
    addToken(TokenType::LBRACE);
    break;
  case '}':
    addToken(TokenType::RBRACE);
    break;
  case ',':
    addToken(TokenType::COMMA);
    break;
  case ';':
    addToken(TokenType::SEMICOLON);
    break;
  case '?':
    addToken(TokenType::QMARK);
    break;
  case '.':
    if (match('.')) {
      handle_range();
    } else
      addToken(TokenType::DOT);
    break;
  case '^':
    addToken(TokenType::BIT_XOR);
    break;
  case '~':
    addToken(TokenType::BIT_NOT);
    break;
  case ':':
    addToken(match('=')
                 ? TokenType::ASSIGN
                 : (match(':') ? TokenType::COLON_COLON : TokenType::COLON));
    break;
  case '=':
    addToken(match('=') ? TokenType::EQUAL
                        : (match('>') ? TokenType::MATCH_ARROW
                                      : TokenType::ERROR_TOKEN));
    break;
  case '<':
    addToken(match('=')
                 ? TokenType::LESS_EQUAL
                 : (match('<') ? TokenType::LBIT_SHIFT : TokenType::LESS));
    break;
  case '>':
    addToken(match('=')
                 ? TokenType::GREAT_EQUAL
                 : (match('>') ? TokenType::RBIT_SHIFT : TokenType::GREAT));
    break;
  case '!':
    addToken(match('=') ? TokenType::NOT_EQUAL : TokenType::NOT);
    break;
  case '|':
    addToken(match('|') ? TokenType::OR : TokenType::BIT_OR);
    break;
  case '&':
    addToken(match('&') ? TokenType::AND : TokenType::BIT_AND);
    break;
  case '%':
    addToken(TokenType::MODULO);
    break;
  case '-':
    addToken(match('>') ? TokenType::RETURN_POINT
                        : (match('=') ? TokenType::MINUS_EQUALS
                                      : (match('-') ? TokenType::MINUS_MINUS
                                                    : TokenType::MINUS)));
    break;
  case '+':
    addToken(match('=')
                 ? TokenType::PLUS_EQUALS
                 : (match('+') ? TokenType::PLUS_PLUS : TokenType::PLUS));
    break;
  case '*':
    addToken(match('=') ? TokenType::MULT_EQUALS
                        : (match('*') ? TokenType::POWER : TokenType::STAR));
    break;
  case '/':
    if (match('/')) {
      while (peek() != '\n' && !isAtEnd())
        advance();
    } else {
      addToken(match('=') ? TokenType::DIVIDE_EQUALS : TokenType::SLASH);
    }
    break;

  case '@':
    handle_attribute();

  case ' ':
  case '\t':
  case '\r':
    break;
  case '\n':
    line++;
    break;
  default:
    if (isdigit(c)) {
      number();
    } else if (isalpha(c) || c == '_') {
      identifier();
    }
    break;
  }
}

void Lexer::handle_attribute() {}

void Lexer::string() {
  while (peek() != '"' && !isAtEnd()) {
    if (peek() == '\n')
      line++;
    advance();
  }

  if (isAtEnd()) {
    addToken(TokenType::ERROR_TOKEN);
  }

  advance();

  std::string literal = source.substr(start + 1, current - start - 2);
  addToken(TokenType::STR_LIT, literal);
}

void Lexer::number() {
  while (isdigit(peek()))
    advance();

  addToken(TokenType::INT_LITERAL);
}

void Lexer::identifier() {
  while (isalnum(peek()) || peek() == '_')
    advance();
  std::string word = source.substr(start, current - start);
  if (keywords.find(word) != keywords.end()) {
    addToken(keywords[word]);
  } else {
    addToken(TokenType::IDENTIFIER);
  }
}

void Lexer::handle_range() {
  if (peek() == '=') {
    advance();
    addToken(TokenType::RANGE_INCLUSIVE);
  } else {
    addToken(TokenType::RANGE);
  }
  advance();
}

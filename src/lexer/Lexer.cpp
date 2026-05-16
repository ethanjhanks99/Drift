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
  keywords["static"] = TokenType::STATIC;
  keywords["panic"] = TokenType::PANIC;
  keywords["channel"] = TokenType::CHANNEL;
  keywords["in"] = TokenType::IN;
  keywords["asm"] = TokenType::ASM;
  // keywords["Region"] = TokenType::REGION;
  // keywords["Option"] = TokenType::OPTION;
  // keywords["Result"] = TokenType::RESULT;
}

std::vector<Token> Lexer::scan_tokens() {
  while (!is_at_end()) {
    start = current;
    scan_token();
  }

  tokens.push_back(Token(TokenType::END_OF_FILE, "", line, start));

  return tokens;
}

bool Lexer::is_at_end() { return current >= source.size(); }

char Lexer::advance() { return source[current++]; }

char Lexer::peek() {
  if (is_at_end())
    return '\0';
  return source[current];
}

char Lexer::peek_next() {
  if (current + 1 >= source.size())
    return '\0';
  return source[current + 1];
}

bool Lexer::match(char expected) {
  if (is_at_end())
    return false;
  if (source[current] != expected)
    return false;

  current++;
  return true;
}

void Lexer::add_token(TokenType type) {
  std::string lexeme = source.substr(start, current - start);
  tokens.push_back(Token(type, lexeme, line, start));
}

void Lexer::add_token(TokenType type, std::string literal) {
  tokens.push_back(Token(type, literal, line, start));
}

void Lexer::scan_token() {
  char c = advance();

  switch (c) {
  case '(':
    add_token(TokenType::LPAREN);
    break;
  case ')':
    add_token(TokenType::RPAREN);
    break;
  case '[':
    add_token(TokenType::LBRACKET);
    break;
  case ']':
    add_token(TokenType::RBRACKET);
    break;
  case '{':
    add_token(TokenType::LBRACE);
    break;
  case '}':
    add_token(TokenType::RBRACE);
    break;
  case ',':
    add_token(TokenType::COMMA);
    break;
  case ';':
    add_token(TokenType::SEMICOLON);
    break;
  case '?':
    add_token(TokenType::QMARK);
    break;
  case '.':
    if (match('.')) {
      handle_range();
    } else
      add_token(TokenType::DOT);
    break;
  case '^':
    add_token(TokenType::BIT_XOR);
    break;
  case '~':
    add_token(TokenType::BIT_NOT);
    break;
  case ':':
    add_token(match('=')
                  ? TokenType::ASSIGN
                  : (match(':') ? TokenType::COLON_COLON : TokenType::COLON));
    break;
  case '=':
    add_token(match('=') ? TokenType::EQUAL
                         : (match('>') ? TokenType::MATCH_ARROW
                                       : TokenType::ERROR_TOKEN));
    break;
  case '<':
    add_token(match('=')
                  ? TokenType::LESS_EQUAL
                  : (match('<') ? TokenType::LBIT_SHIFT : TokenType::LESS));
    break;
  case '>':
    add_token(match('=')
                  ? TokenType::GREAT_EQUAL
                  : (match('>') ? TokenType::RBIT_SHIFT : TokenType::GREAT));
    break;
  case '!':
    add_token(match('=') ? TokenType::NOT_EQUAL : TokenType::NOT);
    break;
  case '|':
    add_token(match('|') ? TokenType::OR : TokenType::BIT_OR);
    break;
  case '&':
    add_token(match('&') ? TokenType::AND : TokenType::BIT_AND);
    break;
  case '%':
    add_token(TokenType::MODULO);
    break;
  case '-':
    add_token(match('>') ? TokenType::RETURN_POINT
                         : (match('=') ? TokenType::MINUS_EQUALS
                                       : (match('-') ? TokenType::MINUS_MINUS
                                                     : TokenType::MINUS)));
    break;
  case '+':
    add_token(match('=')
                  ? TokenType::PLUS_EQUALS
                  : (match('+') ? TokenType::PLUS_PLUS : TokenType::PLUS));
    break;
  case '*':
    add_token(match('=') ? TokenType::MULT_EQUALS
                         : (match('*') ? TokenType::POWER : TokenType::STAR));
    break;
  case '/':
    if (match('/')) {
      while (peek() != '\n' && !is_at_end())
        advance();
    } else {
      add_token(match('=') ? TokenType::DIVIDE_EQUALS : TokenType::SLASH);
    }
    break;

  case '@':
    advance();
    handle_attribute();
    break;
  case '"':
    string();
    break;
  case '\'':
    character();
    break;
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
    } else {
      add_token(TokenType::ERROR_TOKEN);
    }
    break;
  }
}

void Lexer::handle_attribute() {
  while ((isalpha(peek()) || peek() == '_')) {
    advance();
  }

  std::string att_name = source.substr(start + 1, current - start - 1);
  add_token(TokenType::ATTRIBUTE, att_name);
}

void Lexer::string() {
  while (peek() != '"' && !is_at_end()) {
    if (peek() == '\n')
      line++;
    advance();
  }

  if (is_at_end()) {
    add_token(TokenType::ERROR_TOKEN);
  } else {
    advance();
    std::string literal = source.substr(start + 1, current - start - 2);
    add_token(TokenType::STR_LIT, literal);
  }
}

void Lexer::character() {
  while (peek() != '\'' && !is_at_end()) {
    if (peek() == '\n') {
      add_token(TokenType::ERROR_TOKEN);
      return;
    }
    advance();
  }

  if (is_at_end())
    add_token(TokenType::ERROR_TOKEN);
  else {
    advance();
    std::string literal = source.substr(start + 1, current - start - 2);
    if (literal.length() > 1) {
      add_token(TokenType::ERROR_TOKEN);
    } else {
      add_token(TokenType::CHAR, literal);
    }
  }
}

void Lexer::number() {
  bool dec_seen = false;
  while (isdigit(peek()))
    advance();

  if (peek() == '.') {
    advance();
    if (!isdigit(peek())) {
      add_token(TokenType::ERROR_TOKEN);
      return;
    }
    while (isdigit(peek()))
      advance();
    add_token(TokenType::FLOAT_LITERAL);
    return;
  }

  add_token(TokenType::INT_LITERAL);
}

void Lexer::identifier() {
  while (isalnum(peek()) || peek() == '_')
    advance();
  std::string word = source.substr(start, current - start);
  if (keywords.find(word) != keywords.end()) {
    add_token(keywords[word]);
  } else {
    add_token(TokenType::IDENTIFIER);
  }
}

void Lexer::handle_range() {
  if (peek() == '=') {
    advance();
    add_token(TokenType::RANGE_INCLUSIVE);
  } else {
    add_token(TokenType::RANGE);
  }
  advance();
}

#include "Lexer.hpp"
#include "Token.hpp"

Lexer::Lexer(std::string m_source) {
    source = m_source;

    keywords["if"] = TokenType::IF;
    keywords["while"] = TokenType::WHILE;
    keywords["else"] = TokenType::ELSE;
    keywords["func"] = TokenType::FUNC;
    keywords["int"] = TokenType::INT;
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
    keywords["class"] = TokenType::CLASS;
    keywords["var"] = TokenType::VAR;
    keywords["for"] = TokenType::FOR;
}

std::vector<Token> Lexer::scanTokens() {
    while (!isAtEnd()) {
        start = current;
        scanToken();
    }

    tokens.push_back(Token(TokenType::END_OF_FILE, "", line, start));

    return tokens;
}

bool Lexer::isAtEnd() {
    return current >= source.size();
}

char Lexer::advance() {
    return source[current++];
}

char Lexer::peek() {
    if (isAtEnd()) return '\0';
    return source[current];
}

char Lexer::peekNext() {
    if (current + 1 >= source.size()) return '\0';
    return source[current + 1];
}

bool Lexer::match(char expected) {
    if (isAtEnd()) return false;
    if (source[current] != expected) return false;

    current++;
    return true;
}

void Lexer::addToken(TokenType type) {
    std::string lexeme = source.substr(start, current - start);
    tokens.push_back(Token(type, lexeme, line, start));
}

void Lexer::addToken(TokenType type, std::string literal) {
    Lexer::tokens.push_back(Token(type, literal, line, start));
}

void Lexer::scanToken() {
    char c = Lexer::advance();

    switch (c)
    {
    case '(':
        Lexer::addToken(TokenType::LPAREN);
        break;
    case ')':
        Lexer::addToken(TokenType::RPAREN);
        break;
    case '[':
        Lexer::addToken(TokenType::LBRACKET);
        break;
    case ']':
        Lexer::addToken(TokenType::RBRACKET);
        break;
    case '{':
        Lexer::addToken(TokenType::LBRACE);
        break;
    case '}':
        Lexer::addToken(TokenType::RBRACE);
        break;
    case ';':
        Lexer::addToken(TokenType::SEMICOLON);
        break;
    case '.':
        Lexer::addToken(TokenType::DOT);
        break;
    case '^':
        Lexer::addToken(TokenType::BIT_XOR);
        break;
    case '~':
        Lexer::addToken(TokenType::BIT_NOT);
        break;
    case ':':
        Lexer::addToken(Lexer::match('=') ? TokenType::ASSIGN : TokenType::ERROR_TOKEN);
        break;
    case '=':
        Lexer::addToken(Lexer::match('=') ? TokenType::EQUAL : TokenType::ERROR_TOKEN);
        break;
    case '<':
        Lexer::addToken(Lexer::match('=') ? TokenType::LESS_EQUAL : (Lexer::match('<') ? TokenType::LBIT_SHIFT : TokenType::LESS));
        break;
    case '>':
        Lexer::addToken(Lexer::match('=') ? TokenType::GREAT_EQUAL : (Lexer::match('>') ? TokenType::RBIT_SHIFT : TokenType::GREAT));
        break;
    case '!':
        Lexer::addToken(Lexer::match('=') ? TokenType::NOT_EQUAL : TokenType::NOT);
        break;
    case '|':
        Lexer::addToken(Lexer::match('|') ? TokenType::OR : TokenType::BIT_OR);
        break;
    case '&':
        Lexer::addToken(Lexer::match('&') ? TokenType::AND : TokenType::BIT_AND);
        break;
    case '-':
        Lexer::addToken(Lexer::match('>') ? TokenType::RETURN_POINT : TokenType::MINUS);
        break;
    case '+':
        Lexer::addToken(TokenType::PLUS);
        break;
    case '*':
        Lexer::addToken(TokenType::STAR);
        break;
    case '/':
        if (Lexer::match('/')) {
            while (Lexer::peek() != '\n' && !Lexer::isAtEnd()) Lexer::advance();
        } else {
            Lexer::addToken(TokenType::SLASH);
        }
        break;
    
    case ' ':
    case '\t':
    case '\r':
        break;
    case '\n':
        Lexer::line++;
        break;
    default:
        if (isdigit(c)) {
            Lexer::number();
        } else if (isalpha(c) || c == '_') {
            Lexer::identifier();
        }
        break;
    }
}

void Lexer::string() {
    while (peek() != '"' && !isAtEnd()) {
        if (peek() == '\n') line++;
        advance();
    }

    if (isAtEnd()) {
        addToken(TokenType::ERROR_TOKEN);
    }

    advance();

    std::string literal = source.substr(start + 1, current - start - 2);
    Lexer::addToken(TokenType::STR_LIT, literal);
}

void Lexer::number() {
    while (isdigit(peek())) advance();

    Lexer::addToken(TokenType::INT_LITERAL);
    
}

void Lexer::identifier() {
    while (isalnum(peek()) || peek() == '_') advance();
    std::string word = Lexer::source.substr(Lexer::start, Lexer::current - Lexer::start);
    if (Lexer::keywords.find(word) != Lexer::keywords.end()) {
        Lexer::addToken(Lexer::keywords[word]);
    } else {
        Lexer::addToken(TokenType::IDENTIFIER);
    }
}
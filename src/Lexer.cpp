#include "Lexer.hpp"
#include "Token.hpp"

Lexer::Lexer(std::string source) {
    Lexer::source = source;

    Lexer::keywords["if"] = TokenType::IF;
    Lexer::keywords["while"] = TokenType::WHILE;
    Lexer::keywords["else"] = TokenType::ELSE;
    Lexer::keywords["func"] = TokenType::FUNC;
    Lexer::keywords["int"] = TokenType::INT;
    Lexer::keywords["void"] = TokenType::VOID;
    Lexer::keywords["string"] = TokenType::STRING;
    Lexer::keywords["char"] = TokenType::CHAR;
    Lexer::keywords["bool"] = TokenType::BOOL;
    Lexer::keywords["struct"] = TokenType::STRUCT;
    Lexer::keywords["true"] = TokenType::TRUE;
    Lexer::keywords["false"] = TokenType::FALSE;
    Lexer::keywords["then"] = TokenType::THEN;
    Lexer::keywords["do"] = TokenType::DO;
    Lexer::keywords["return"] = TokenType::RETURN;
    Lexer::keywords["class"] = TokenType::CLASS;
    Lexer::keywords["var"] = TokenType::VAR;
    Lexer::keywords["for"] = TokenType::FOR;
}

std::vector<Token> Lexer::scanTokens() {
    while (!isAtEnd()) {
        Lexer::start = Lexer::current;
        Lexer::scanToken();
    }

    Lexer::tokens.push_back(Token(TokenType::END_OF_FILE, "", line, start));

    return Lexer::tokens;
}

bool Lexer::isAtEnd() {
    return current >= source.size();
}

char Lexer::advance() {
    return source[current++];
}

char Lexer::peek() {
    if (Lexer::isAtEnd()) return '\0';
    return Lexer::source[current];
}

char Lexer::peekNext() {

}

bool Lexer::match(char expected) {
    if (Lexer::isAtEnd()) return false;
    if (Lexer::source[current] != expected) return false;

    current++;
    return true;
}

void Lexer::addToken(TokenType type) {
    std::string lexeme = Lexer::source.substr(Lexer::start, Lexer::current - Lexer::start);
    Lexer::tokens.push_back(Token(type, lexeme, Lexer::line, Lexer::start));
}

void Lexer::addToken(TokenType type, std::string literal) {
    Lexer::tokens.push_back(Token(type, literal, Lexer::line, Lexer::start));
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
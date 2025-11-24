#include "Lexer.hpp"
#include "Token.hpp"

Lexer::Lexer(std::string source) {
    Lexer::source = source;
}

std::vector<Token> Lexer::scanTokens() {
    
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
    if (Lexer::isAtEnd) return false;
    if (Lexer::source[current] != expected) return false;

    current++;
    return true;
}

void Lexer::addToken(TokenType type) {

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

}

void Lexer::number() {
    
}

void Lexer::identifier() {
    
}
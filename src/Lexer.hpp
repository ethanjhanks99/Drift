#include "Token.hpp"
#include <string>
#include <vector>
#include <unordered_map>

class Lexer {
  public:
    Lexer(std::string source);
    std::vector<Token> scanTokens();

  private:
    std::string source;
    std::vector<Token> tokens;
    std::unordered_map<std::string, TokenType> keywords;
    
    int start = 0;
    int current = 0;
    int line = 1;

    bool isAtEnd();
    char advance();
    char peek();
    char peekNext();
    bool match(char expected);
    void addToken(TokenType type);
    void addToken(TokenType type, std::string literal);
    void scanToken();

    void string();
    void number();
    void identifier();
};
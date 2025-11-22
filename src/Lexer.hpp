#include "Token.hpp"
#include <vector>

class Lexer {
  public:
    Lexer(std::string source);
    std::vector<Token> scanTokens();

  private:
    std::string source;
    std::vector<Token> tokens;
    
    int start = 0;
    int current = 0;
    int line = 1;

    bool isAtEnd();
    char advance();
    char peek();
    char peekNext();
    bool match(char expected);
    void addToken(TokenType type);
    void scanToken();

    void string();
    void number();
    void identifier();
};
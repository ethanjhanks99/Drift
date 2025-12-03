#pragma once
#include <string>

enum class TokenType {
    /* Single Character Token Types */
    LPAREN, RPAREN, LBRACKET, RBRACKET,
    LBRACE, RBRACE, SEMICOLON, COMMA, DOT,
    RETURN_POINT,

    /* Logical Operators */
    ASSIGN, EQUAL, NOT_EQUAL, LESS, LESS_EQUAL,
    GREAT, GREAT_EQUAL, AND, OR, NOT,

    /* Mathematical and Bit Operators */
    PLUS, MINUS, STAR, SLASH, MODULO, BIT_AND, BIT_OR,
    BIT_XOR, BIT_NOT, LBIT_SHIFT, RBIT_SHIFT,

    /* Values and User Provided */
    IDENTIFIER, INT_LITERAL, FLOAT_LITERAL, STR_LIT, CHAR_LIT,

    /* Keywords */
    IF, WHILE, ELSE, FUNC, INT, VOID, STRING, 
    CHAR, BOOL, STRUCT, TRUE, FALSE, THEN, DO,
    RETURN,

    /* Reserved Words */
    CLASS, PLUS_EQUALS, MINUS_EQUALS, MULT_EQUALS, 
    DIVIDE_EQUALS, POWER, FOR, MODULO, VAR, COLON,
    PLUS_PLUS, MINUS_MINUS,

    /* Special */
    END_OF_FILE,
    ERROR_TOKEN
};

typedef struct Token
{
    TokenType type;
    std::string lexeme;
    uint line;
    uint column;

    Token(TokenType t, std::string le, uint l, uint c) : type(t), lexeme(le), line(l), column(c) {}

    std::string toString() const {
        return "Token(" + std::to_string((int)type) + ", '" + lexeme + "')";
    }
};

#pragma once
#include <string>

enum class TokenType {
    /* Single Character Token Types */
    LPAREN, RPAREN, LBRACKET, RBRACKET,
    LBRACE, RBRACE, SEMICOLON, COMMA, DOT,

    /* Double and Possible Double */
    ASSIGN, EQUAL, NOT_EQUAL, LESS, LESS_EQUAL,
    GREAT, GREAT_EQUAL, AND, OR, RETURN_POINT, // ->

    /* Mathematical and Bit Operators */
    PLUS, MINUS, STAR, SLASH, BIT_AND, BIT_OR,
    BIT_XOR, BIT_NOT, LBIT_SHIFT, RBIT_SHIFT,

    /* Values and User Provided */
    IDENTIFIER, NUMBER, STR_LIT, CHAR_LIT,

    /* Keywords */
    IF, WHILE, ELSE, FUNC, INT, VOID, STRING, 
    CHAR, BOOL, STRUCT, TRUE, FALSE, THEN, DO,
    RETURN,

    /* Reserved Words */
    CLASS, PLUS_EQUALS, MINUS_EQUALS, MULT_EQUALS, 
    DIVIDE_EQUALS, POWER, FOR, MODULO, VAR,

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

    std::string toString() const {
        return "Token(" + std::to_string((int)type) + ", '" + lexeme + "')";
    }
};

set(PROJECT drift_lexer)
set(HEADER_FILES
    Lexer.hpp
    Token.hpp)

set(SOURCE_FILES
    Lexer.cpp
    Token.cpp)

add_executable(${PROJECT} ${HEADER_FILES} ${SOURCE_FILES} main.cpp)

set(PROJECT drift_lexer)
set(HEADER_FILES
    Lexer.hpp
    Token.hpp)

set(SOURCE_FILES
    Lexer.cpp
    Token.cpp)

add_library(${PROJECT})

target_sources(drift_lexer
  PRIVATE ${SOURCE_FILES}
  PUBLIC
    FILE_SET HEADERS
    FILES ${HEADER_FILES}
)

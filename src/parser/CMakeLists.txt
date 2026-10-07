set(PROJECT drift_parser)
set(HEADER_FILES
    Parser.hpp
    NodeFactory.hpp)

set(SOURCE_FILES
    Parser.cpp
    ParserHelper.cpp
    NodeFactory.cpp)

add_library(${PROJECT})

target_sources(${PROJECT}
  PRIVATE ${SOURCE_FILES}
  PUBLIC
    FILE_SET HEADERS
    BASE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}
    FILES ${HEADER_FILES}
)

target_link_libraries(${PROJECT} PUBLIC drift_tools drift_lexer drift_error)


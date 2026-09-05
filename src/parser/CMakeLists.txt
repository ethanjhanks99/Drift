set(PROJECT drift_parser)
set(HEADER_FILES
    Parser.hpp
    NodeFactory.hpp)

set(SOURCE_FILES
    Parser.cpp
    NodeFactory.cpp)

add_library(${PROJECT})

target_sources(${PROJECT}
  PRIVATE ${SOURCE_FILES}
  PUBLIC
    FILE_SET HEADERS
    FILES ${HEADER_FILES}
)

target_include_directories(${PROJECT} PUBLIC ../)

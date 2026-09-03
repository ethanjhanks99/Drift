set(PROJECT drift_tools)
set(HEADER_FILES
  AST.hpp
  AssignOp.hpp
  BinaryOp.hpp
  NodeType.hpp
  SourceLocation.hpp
  OwnershipMod.hpp
  Type.hpp
  UnaryOp.hpp
  VisMod.hpp
  ParseError.hpp)


set(SOURCE_FILES
  AST.cpp
  AssignOp.cpp
  BinaryOp.cpp
  SourceLocation.cpp
  Type.cpp
  UnaryOp.cpp)

add_library(${PROJECT})

target_sources(${PROJECT}
  PRIVATE ${SOURCE_FILES}
  PUBLIC
    FILE_SET HEADERS
    FILES ${HEADER_FILES}
)

target_include_directories(${PROJECT} PUBLIC ../)

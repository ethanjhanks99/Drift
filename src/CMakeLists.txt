cmake_minimum_required(VERSION 3.21)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(FetchContent)

FetchContent_Declare(
  Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG v3.8.1
)

FetchContent_MakeAvailable(Catch2)

set(PROJECT driftc)

set(TESTS drift_tests)

project(${PROJECT})

add_executable(${PROJECT} main.cpp)

add_executable(${TESTS}
  tests/lexer/lexer_tests.cpp)

add_subdirectory(lexer)
add_subdirectory(parser)

target_link_libraries(${PROJECT} drift_lexer drift_parser)
target_link_libraries(${TESTS}
  PRIVATE
    drift_lexer
    drift_parser
    Catch2::Catch2WithMain
)

list(APPEND CMAKE_MODULE_PATH ${catch2_SOURCE_DIR}/extras)

include(CTest)
include(Catch)
catch_discover_tests(${TESTS})

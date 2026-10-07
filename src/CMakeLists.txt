cmake_minimum_required(VERSION 3.23)
set(PROJECT driftc)
set(TESTS drift_tests)

project(${PROJECT})

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(FetchContent)

FetchContent_Declare(
  Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG v3.8.1
)

FetchContent_MakeAvailable(Catch2)

add_subdirectory(lexer)
add_subdirectory(parser)
add_subdirectory(tools)
add_subdirectory(error)

add_executable(${PROJECT} main.cpp)

add_executable(${TESTS}
  tests/lexer/lexer_tests.cpp)

target_link_libraries(${PROJECT}
  PRIVATE
    drift_parser
)
target_link_libraries(${TESTS}
  PRIVATE
    drift_parser
    Catch2::Catch2WithMain
)

list(APPEND CMAKE_MODULE_PATH ${catch2_SOURCE_DIR}/extras)

include(CTest)
include(Catch)
catch_discover_tests(${TESTS})

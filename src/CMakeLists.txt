cmake_minimum_required(VERSION 3.21)

set(PROJECT driftc)

project(${PROJECT})

add_executable(${PROJECT} main.cpp)

add_subdirectory(drift_lexer)


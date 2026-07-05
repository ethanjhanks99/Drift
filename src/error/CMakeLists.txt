set(PROJECT drift_error)
set(HEADER_FILES
    ErrorHandler.hpp)

set(SOURCE_FILES
    ErrorHandler.cpp)

add_library(${PROJECT})

target_sources(${PROJECT}
  PRIVATE ${SOURCE_FILES}
  PUBLIC
    FILE_SET HEADERS
    FILES ${HEADER_FILES}
)

target_include_directories(${PROJECT} PUBLIC ../)

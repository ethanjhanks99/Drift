set(PROJECT drift_tools)
set(HEADER_FILES)
set(SOURCE_FILES)

add_library(${PROJECT})

target_sources(${PROJECT}
  PRIVATE ${SOURCE_FILES}
  PUBLIC
    FILE_SET HEADERS
    FILES ${HEADER_FILES}
)

target_include_directories(${PROJECT} PUBLIC ../)

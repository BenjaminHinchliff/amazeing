set(INCLUDE_DIR "${PROJECT_SOURCE_DIR}/include/")
set(INCLUDES)

list(TRANSFORM INCLUDES PREPEND ${INCLUDE_DIR})

set(SRCS
    "main.cpp")

add_executable(amazeing ${SRCS} ${INCLUDES})
target_include_directories(amazeing PRIVATE ${INCLUDE_DIR})

target_link_libraries(amazeing PRIVATE stb)


set(INCLUDE_DIR "${PROJECT_SOURCE_DIR}/include/")
set(
  INCLUDES
  "maze.hpp"
)

list(TRANSFORM INCLUDES PREPEND ${INCLUDE_DIR})

set(
  SRCS
  "main.cpp"
  "maze.cpp"
)

add_executable(amazeing ${SRCS} ${INCLUDES})
target_compile_features(amazeing PRIVATE cxx_std_17)
set_target_properties(amazeing PROPERTIES CXX_EXTENSIONS OFF)
target_include_directories(amazeing PRIVATE ${INCLUDE_DIR})

target_link_libraries(amazeing PRIVATE stb)


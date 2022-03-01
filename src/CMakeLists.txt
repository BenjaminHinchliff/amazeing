set(INCLUDE_DIR "${PROJECT_SOURCE_DIR}/include/")
set(
  INCLUDES
  "maze.hpp"
  "grid.hpp"
  "pathfinder.hpp"
)

list(TRANSFORM INCLUDES PREPEND ${INCLUDE_DIR})

set(
  SRCS
  "main.cpp"
  "maze.cpp"
  "grid.cpp"
  "pathfinder.cpp"
)

add_executable(amazeing ${SRCS} ${INCLUDES})
target_compile_features(amazeing PRIVATE cxx_std_17)
set_target_properties(amazeing PROPERTIES CXX_EXTENSIONS OFF)
# Turn on more warnings
target_compile_options(amazeing PRIVATE
  $<$<CXX_COMPILER_ID:MSVC>:/W4>
  $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -Wpedantic>
)

target_include_directories(amazeing PRIVATE ${INCLUDE_DIR})

target_link_libraries(amazeing PRIVATE stb)


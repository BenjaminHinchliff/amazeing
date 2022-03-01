#include "maze.hpp"
#include "grid.hpp"
#include "pathfinder.hpp"

int main() {
  Maze maze(20, 20);

  std::cout << maze << '\n';

  const std::vector<Point> path = a_star(maze);

  Grid grid(maze, path);
  grid.write_image("maze.png");

  return 0;
}

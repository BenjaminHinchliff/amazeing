#include "maze.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char **argv) {
  if (argc != 3) {
    fprintf(stderr, "USAGE: %s WIDTH HEIGHT\n", argv[0]);
    return 1;
  }

  int width = atoi(argv[1]);
  int height = atoi(argv[2]);

  srand(time(NULL));

  Maze maze;
  if (!maze_new(&maze, width, height)) {
    fprintf(stderr, "failed to create maze\n");
    return 1;
  }

  Grid grid;
  if (!maze_to_grid(maze, &grid)) {
    fprintf(stderr, "failed to create grid\n");
    return 1;
  }

  grid_set(grid, 1, 1, GRID_CELL_START);
  grid_set(grid, grid.width - 2, grid.height - 2, GRID_CELL_GOAL);

  uint8_t *image = grid_to_image(grid);
  stbi_write_png("maze.png", grid.width, grid.height, 3, image, grid.width * 3);

  grid_free(grid);
  maze_free(maze);
  return 0;
}


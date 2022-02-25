#ifndef MAZE_H
#define MAZE_H

#include "grid.h"

#include <stdbool.h>

typedef enum MazeDir {
  MAZE_DIR_NULL = 0,
  MAZE_DIR_NORTH = 1,
  MAZE_DIR_SOUTH = 2,
  MAZE_DIR_EAST = 4,
  MAZE_DIR_WEST = 8,
} MazeDir;

typedef struct Maze {
  MazeDir *data;
  int width;
  int height;
} Maze;

bool maze_new(Maze *maze, int width, int height);

MazeDir maze_get(const Maze maze, int x, int y);
void maze_set(Maze maze, int x, int y, MazeDir value);

void maze_debug_print(const Maze maze);

bool maze_to_grid(const Maze maze, Grid *grid);

void maze_free(Maze maze);

#endif

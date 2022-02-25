#include "maze.h"

#include <stdio.h>
#include <stdlib.h>

static void cell_dir_to_delta(MazeDir dir, int *dx, int *dy) {
  switch (dir) {
  case MAZE_DIR_NORTH:
    *dx = 0;
    *dy = -1;
    break;
  case MAZE_DIR_SOUTH:
    *dx = 0;
    *dy = 1;
  break;
  case MAZE_DIR_EAST:
    *dx = 1;
    *dy = 0;
    break;
  case MAZE_DIR_WEST:
    *dx = -1;
    *dy = 0;
    break;
  default:
    *dx = 0;
    *dy = 0;
    break;
  }
}

static MazeDir cell_dir_opposite(MazeDir dir) {
  switch (dir) {
  case MAZE_DIR_NORTH:
    return MAZE_DIR_SOUTH;
  case MAZE_DIR_SOUTH:
    return MAZE_DIR_NORTH;
  case MAZE_DIR_EAST:
    return MAZE_DIR_WEST;
  case MAZE_DIR_WEST:
    return MAZE_DIR_EAST;
  default:
    return MAZE_DIR_NULL;
  }
}

static void shuffle_dirs(MazeDir *dirs, size_t dirs_len) {
  for (int i = dirs_len - 1; i > 0; --i) {
    int j = rand() % (i + 1);
    MazeDir tmp = dirs[j];
    dirs[j] = dirs[i];
    dirs[i] = tmp;
  }
}

static void carve_maze(Maze maze, int cx, int cy) {
  MazeDir dirs[] = {MAZE_DIR_NORTH, MAZE_DIR_SOUTH, MAZE_DIR_WEST,
                     MAZE_DIR_EAST};
  size_t dirs_len = sizeof(dirs) / sizeof(*dirs);
  shuffle_dirs(dirs, dirs_len);

  for (int i = 0; i < dirs_len; ++i) {
    MazeDir dir = dirs[i];

    int dx, dy;
    cell_dir_to_delta(dir, &dx, &dy);

    int nx = cx + dx;
    int ny = cy + dy;

    if (0 <= nx && nx < maze.width && 0 <= ny && ny < maze.height &&
        maze_get(maze, nx, ny) == MAZE_DIR_NULL) {
      maze_set(maze, cx, cy, maze_get(maze, cx, cy) | dir);
      maze_set(maze, nx, ny, maze_get(maze, nx, ny) | cell_dir_opposite(dir));
      carve_maze(maze, nx, ny);
    }
  }
}

bool maze_new(Maze *maze, int width, int height) {
  MazeDir *data = malloc(sizeof(*maze) * width * height);
  if (data == NULL) {
    return false;
  }

  maze->width = width;
  maze->height = height;
  maze->data = data;

  for (int i = 0; i < width * height; ++i) {
    maze->data[i] = MAZE_DIR_NULL;
  }

  carve_maze(*maze, 0, 0);

  return true;
}

MazeDir maze_get(const Maze maze, int x, int y) {
  return maze.data[maze.width * y + x];
}

void maze_set(Maze maze, int x, int y, MazeDir value) {
  maze.data[maze.width * y + x] = value;
}

void maze_debug_print(const Maze maze) {
  printf(" ");
  for (int i = 0; i < maze.width * 2 - 1; ++i) {
    printf("_");
  }
  printf("\n");

  for (int y = 0; y < maze.height; ++y) {
    printf("|");
    for (int x = 0; x < maze.width; ++x) {
      printf((maze_get(maze, x, y) & MAZE_DIR_SOUTH) ? " " : "_");
      if (maze_get(maze, x, y) & MAZE_DIR_EAST) {
        printf(((maze_get(maze, x, y) | maze_get(maze, x + 1, y)) &
                MAZE_DIR_SOUTH)
                   ? " "
                   : "_");
      } else {
        printf("|");
      }
    }
    printf("\n");
  }
}

bool maze_to_grid(const Maze maze, Grid *grid) {
  if (!grid_new(grid, maze.width * 2 + 1, maze.height * 2 + 1)) {
    return false;
  }

  for (int y = 0; y < maze.height; ++y) {
    for (int x = 0; x < maze.width; ++x) {
      int gx = x * 2 + 1;
      int gy = y * 2 + 1;
      grid_set(*grid, gx, gy, GRID_CELL_EMPTY);
      if (maze_get(maze, x, y) & MAZE_DIR_NORTH) {
        grid_set(*grid, gx, gy - 1, GRID_CELL_EMPTY);
      }
      if (maze_get(maze, x, y) & MAZE_DIR_SOUTH) {
        grid_set(*grid, gx, gy + 1, GRID_CELL_EMPTY);
      }
      if (maze_get(maze, x, y) & MAZE_DIR_EAST) {
        grid_set(*grid, gx + 1, gy, GRID_CELL_EMPTY);
      }
      if (maze_get(maze, x, y) & MAZE_DIR_WEST) {
        grid_set(*grid, gx - 1, gy, GRID_CELL_EMPTY);
      }
    }
  }

  return true;
}

void maze_free(Maze maze) { free(maze.data); }


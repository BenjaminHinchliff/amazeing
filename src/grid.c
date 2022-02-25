#include "grid.h"

#include <stdio.h>
#include <stdlib.h>

bool grid_new(Grid *grid, int width, int height) {
  GridCell *data = malloc(sizeof(*data) * width * height);
  if (data == NULL) {
    return false;
  }

  grid->data = data;
  grid->width = width;
  grid->height = height;

  for (int i = 0; i < width * height; ++i) {
    grid->data[i] = GRID_CELL_WALL;
  }

  return true;
}

GridCell grid_get(const Grid grid, int x, int y) {
  return grid.data[grid.width * y + x];
}

void grid_set(Grid grid, int x, int y, GridCell cell) {
  grid.data[grid.width * y + x] = cell;
}

void grid_debug_print(const Grid grid) {
  for (int y = 0; y < grid.height; ++y) {
    for (int x = 0; x < grid.width; ++x) {
      printf((grid_get(grid, x, y) == GRID_CELL_WALL) ? "#" : " ");
    }
    printf("\n");
  }
}

static void write_color(uint8_t *image, uint8_t r, uint8_t g, uint8_t b) {
  image[0] = r;
  image[1] = g;
  image[2] = b;
}

uint8_t *grid_to_image(const Grid grid) {
  uint8_t *image = malloc(sizeof(*image) * grid.width * grid.height * 3);
  if (image == NULL) {
    return NULL;
  }

  for (int i = 0; i < grid.width * grid.height; ++i) {
    switch (grid.data[i]) {
    case GRID_CELL_WALL:
      write_color(&image[i * 3], 0, 0, 0);
      break;
    case GRID_CELL_EMPTY:
      write_color(&image[i * 3], 255, 255, 255);
      break;
    case GRID_CELL_START:
      write_color(&image[i * 3], 199, 0, 0);
      break;
    case GRID_CELL_GOAL:
      write_color(&image[i * 3], 23, 199, 0);
      break;
    }
  }

  return image;
}

void grid_free(Grid grid) { free(grid.data); }


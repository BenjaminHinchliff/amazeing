#ifndef GRID_H
#define GRID_H

#include <stdbool.h>
#include <stdint.h>

typedef enum GridCell {
    GRID_CELL_EMPTY,
    GRID_CELL_WALL,
    GRID_CELL_START,
    GRID_CELL_GOAL,
} GridCell;

typedef struct Grid {
    GridCell *data;
    int width;
    int height;
} Grid;

bool grid_new(Grid *grid, int width, int height);

GridCell grid_get(const Grid grid, int x, int y);
void grid_set(Grid grid, int x, int y, GridCell cell);

void grid_debug_print(const Grid grid);

uint8_t *grid_to_image(const Grid grid);

void grid_free(Grid grid);

#endif


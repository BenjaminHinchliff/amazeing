#include "grid.hpp"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>

#include <algorithm>

Grid::Grid(size_t width, size_t height, Point start, Point goal,
           const std::vector<Point> &path)
    : Grid(width, height, EMPTY, start, goal, path) {}

Grid::Grid(size_t width, size_t height, Cell fill, Point start, Point goal,
           const std::vector<Point> &path)
    : data(height, std::vector<Cell>(width, fill)), start(start), goal(goal),
      path(path) {}

Grid::Grid(const Maze &maze, const std::vector<Point> &path)
    : Grid(maze.width() * 2 + 1, maze.height() * 2 + 1, WALL, {1, 1},
           {maze.width() * 2 - 1, maze.height() * 2 - 1}, path) {
  carve_passages(maze);
  make_path();
  data[start.second][start.first] = START;
  data[goal.second][goal.first] = GOAL;
}

void Grid::carve_passages(const Maze &maze) {
  for (size_t y = 0; y < maze.height(); ++y) {
    for (size_t x = 0; x < maze.width(); ++x) {
      const auto cell = maze[y][x];
      size_t gy = y * 2 + 1;
      size_t gx = x * 2 + 1;
      data[gy][gx] = EMPTY;
      if (cell & Direction::NORTH) {
        data[gy - 1][gx] = EMPTY;
      }
      if (cell & Direction::SOUTH) {
        data[gy + 1][gx] = EMPTY;
      }
      if (cell & Direction::EAST) {
        data[gy][gx + 1] = EMPTY;
      }
      if (cell & Direction::WEST) {
        data[gy][gx - 1] = EMPTY;
      }
    }
  }
}

void Grid::make_path() {
  for (size_t i = 1; i < path.size(); ++i) {
    const Point g1_pt = maze_to_grid(path[i - 1]);
    const Point g2_pt = maze_to_grid(path[i]);
    data[g1_pt.second][g1_pt.first] = PATH;
    data[(g1_pt.second + g2_pt.second) / 2][(g1_pt.first + g2_pt.first) / 2] =
        PATH;
  }
}

Point Grid::maze_to_grid(const Point &point) {
  return {point.first * 2 + 1, point.second * 2 + 1};
}

void Grid::write_image(const std::string &name) const {
  std::vector<uint8_t> image;
  for (const auto &row : data) {
    for (const Cell cell : row) {
      switch (cell) {
      case WALL:
        image.insert(image.end(), {0, 0, 0});
        break;
      case EMPTY:
        image.insert(image.end(), {255, 255, 255});
        break;
      case START:
        image.insert(image.end(), {0, 184, 0});
        break;
      case GOAL:
        image.insert(image.end(), {0, 0, 184});
        break;
      case PATH:
        image.insert(image.end(), {184, 0, 0});
        break;
      default:
        throw std::runtime_error("invalid cell!");
      }
    }
  }

  stbi_write_png(name.c_str(), width(), height(), 3, image.data(), width() * 3);
}

std::ostream &operator<<(std::ostream &os, const Grid &grid) {
  for (size_t y = 0; y < grid.height(); ++y) {
    const auto &row = grid[y];
    for (const auto cell : row) {
      os << ((cell == Grid::WALL) ? '#' : ' ');
    }
    if (y != grid.height() - 1) {
      os << '\n';
    }
  }

  return os;
}

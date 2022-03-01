#pragma once

#include "maze.hpp"

#include <vector>

class Grid {
public:
  enum Cell : uint8_t {
    EMPTY,
    WALL,
    START,
    GOAL,
    PATH,
  };

  Grid(size_t width, size_t height, Point start, Point goal,
       const std::vector<Point> &path);
  Grid(size_t width, size_t height, Cell fill, Point start, Point goal,
       const std::vector<Point> &path);
  Grid(const Maze &maze, const std::vector<Point> &path);

  size_t width() const { return data[0].size(); }
  size_t height() const { return data.size(); }

  std::vector<Cell> &operator[](size_t i) { return data[i]; }
  const std::vector<Cell> &operator[](size_t i) const { return data[i]; }

  void write_image(const std::string &path) const;

  friend std::ostream &operator<<(std::ostream &os, const Grid &grid);

private:
  void carve_passages(const Maze &maze);
  void make_path();
  static Point maze_to_grid(const Point &point);

  std::vector<std::vector<Cell>> data;
  Point start;
  Point goal;
  std::vector<Point> path;
};

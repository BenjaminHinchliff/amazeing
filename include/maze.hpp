#pragma once

#include <cstdint>
#include <iostream>
#include <random>
#include <utility>
#include <vector>

using Point = std::pair<int, int>;

class Direction {
public:
  enum Value : uint8_t {
    NORTH = 1,
    SOUTH = 2,
    EAST = 4,
    WEST = 8,
  };

  Direction() = default;
  constexpr Direction(Value value);

  constexpr operator Value() const;
  explicit operator bool() = delete;

  constexpr Point delta() const;
  constexpr Direction opposite() const;

private:
  Value value;
};

class Maze {
public:
  using Cell = uint8_t;

  Maze(size_t width, size_t height);
  Maze(size_t width, size_t height, Point start, Point end);

  size_t width() const { return data[0].size(); }
  size_t height() const { return data.size(); }
  const Point &getStart() const { return start; }
  const Point &getGoal() const { return goal; }
  std::vector<Point> neighbors(const Point &point) const;

  std::vector<Cell> &operator[](size_t i) { return data[i]; }
  const std::vector<Cell> &operator[](size_t i) const { return data[i]; }

  friend std::ostream &operator<<(std::ostream &os, const Maze &maze);

private:
  void carve_from(long cx, long cy);

  std::vector<std::vector<Cell>> data;
  std::default_random_engine rand_engine;
  Point start;
  Point goal;
};


#pragma once

#include <cstdint>
#include <iostream>
#include <random>
#include <utility>
#include <vector>

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

  constexpr std::pair<int, int> delta() const;
  constexpr Direction opposite() const;

private:
  Value value;
};

class Maze {
public:
  Maze(int width, int height);

  friend std::ostream &operator<<(std::ostream &os, const Maze &maze);

private:
  void carve_from(int cx, int cy);

  std::vector<std::vector<uint8_t>> data;
  std::default_random_engine rand_engine;
};


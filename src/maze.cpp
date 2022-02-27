#include "maze.hpp"

#include <algorithm>
#include <array>
#include <chrono>
#include <string>

constexpr Direction::Direction(Value value) : value(value) {}

constexpr Direction::operator Value() const { return value; }

constexpr std::pair<int, int> Direction::delta() const {
  switch (value) {
  case NORTH:
    return {0, -1};
  case SOUTH:
    return {0, 1};
  case EAST:
    return {1, 0};
  case WEST:
    return {-1, 0};
  default:
    throw std::runtime_error("invalid direction!");
  }
}

constexpr Direction Direction::opposite() const {
  switch (value) {
  case NORTH:
    return SOUTH;
  case SOUTH:
    return NORTH;
  case EAST:
    return WEST;
  case WEST:
    return EAST;
  default:
    throw std::runtime_error("invalid direction!");
  }
}

Maze::Maze(int width, int height)
    : data(height, std::vector<uint8_t>(width, 0)),
      rand_engine(std::chrono::system_clock::now().time_since_epoch().count()) {
  carve_from(0, 0);
}

void Maze::carve_from(int cx, int cy) {
  std::array<Direction, 4> dirs = {Direction::NORTH, Direction::SOUTH,
                                   Direction::EAST, Direction::WEST};
  std::shuffle(dirs.begin(), dirs.end(), rand_engine);

  for (Direction dir : dirs) {
    const auto [dx, dy] = dir.delta();

    int nx = cx + dx;
    int ny = cy + dy;

    if (0 <= ny && ny < data.size() && 0 <= nx && nx < data[ny].size() &&
        data[ny][nx] == 0) {
      data[cy][cx] |= dir;
      data[ny][nx] |= dir.opposite();
      carve_from(nx, ny);
    }
  }
}

std::ostream &operator<<(std::ostream &os, const Maze &maze) {
  os << ' ' << std::string(maze.data[0].size() * 2 - 1, '_') << '\n';

  for (size_t y = 0; y < maze.data.size(); ++y) {
    os << '|';
    for (size_t x = 0; x < maze.data[y].size(); ++x) {
      uint8_t cell = maze.data[y][x];
      os << ((cell & Direction::SOUTH) ? ' ' : '_');
      if (cell & Direction::EAST) {
        os << (((cell | maze.data[y][x + 1]) & Direction::SOUTH) ? ' ' : '_');
      } else {
        os << '|';
      }
    }
    if (y != maze.data.size() - 1) {
      os << '\n';
    }
  }

  return os;
}


#include "maze.hpp"

#include <algorithm>
#include <array>
#include <chrono>
#include <string>

constexpr Direction::Direction(Value value) : value(value) {}

constexpr Direction::operator Value() const { return value; }

constexpr Point Direction::delta() const {
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

Maze::Maze(size_t width, size_t height)
    : Maze(width, height, {0, 0}, {width - 1, height - 1}) {}

Maze::Maze(size_t width, size_t height, Point start, Point goal)
    : data(height, std::vector<Cell>(width, 0)),
      rand_engine(std::chrono::system_clock::now().time_since_epoch().count()),
      start(start), goal(goal) {
  carve_from(0, 0);
}

std::vector<Point> Maze::neighbors(const Point &point) const {
  std::vector<Point> neighbors;
  uint8_t cell = data[point.second][point.first];
  if (cell & Direction::NORTH) {
    neighbors.emplace_back(point.first, point.second - 1);
  }
  if (cell & Direction::SOUTH) {
    neighbors.emplace_back(point.first, point.second + 1);
  }
  if (cell & Direction::EAST) {
    neighbors.emplace_back(point.first + 1, point.second);
  }
  if (cell & Direction::WEST) {
    neighbors.emplace_back(point.first - 1, point.second);
  }
  return neighbors;
}

void Maze::carve_from(long cx, long cy) {
  std::array<Direction, 4> dirs = {Direction::NORTH, Direction::SOUTH,
                                   Direction::EAST, Direction::WEST};
  std::shuffle(dirs.begin(), dirs.end(), rand_engine);

  for (Direction dir : dirs) {
    const auto [dx, dy] = dir.delta();

    long nx = cx + dx;
    long ny = cy + dy;

    if (0 <= ny && ny < static_cast<long>(height()) && 0 <= nx &&
        nx < static_cast<long>(width()) && data[ny][nx] == 0) {
      data[cy][cx] |= dir;
      data[ny][nx] |= dir.opposite();
      carve_from(nx, ny);
    }
  }
}

std::ostream &operator<<(std::ostream &os, const Maze &maze) {
  os << ' ' << std::string(maze.data[0].size() * 2 - 1, '_') << '\n';

  for (size_t y = 0; y < maze.height(); ++y) {
    os << '|';
    for (size_t x = 0; x < maze.width(); ++x) {
      Maze::Cell cell = maze[y][x];
      os << ((cell & Direction::SOUTH) ? ' ' : '_');
      if (cell & Direction::EAST) {
        os << (((cell | maze.data[y][x + 1]) & Direction::SOUTH) ? ' ' : '_');
      } else {
        os << '|';
      }
    }
    if (y != maze.height() - 1) {
      os << '\n';
    }
  }

  return os;
}


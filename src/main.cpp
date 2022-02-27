#include <algorithm>
#include <array>
#include <chrono>
#include <iostream>
#include <pthread.h>
#include <random>
#include <string>
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
  constexpr Direction(Value value) : value(value) {}

  constexpr operator Value() const { return value; }

  explicit operator bool() = delete;

  constexpr std::pair<int, int> delta() const {
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

  constexpr Direction opposite() const {
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

private:
  Value value;
};

class Maze {
public:
  Maze(int width, int height)
      : data(height, std::vector<uint8_t>(width, 0)),
        rand_engine(
            std::chrono::system_clock::now().time_since_epoch().count()) {
    carve_from(0, 0);
  }

  friend std::ostream &operator<<(std::ostream &os, const Maze &maze);

private:
  void carve_from(int cx, int cy) {
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

  std::vector<std::vector<uint8_t>> data;
  std::default_random_engine rand_engine;
};

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

int main() {
  Maze maze(20, 20);

  std::cout << maze << '\n';

  return 0;
}

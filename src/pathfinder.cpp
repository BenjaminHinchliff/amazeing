#include "pathfinder.hpp"

#include <algorithm>
#include <queue>
#include <unordered_map>

struct PointHash {
  std::size_t operator()(const Point &point) const {
    return std::hash<int>{}(point.first) ^
           (std::hash<int>{}(point.second) << 1);
  }
};

int manhattan_distance(const Point &a, const Point &b) {
  return std::abs(a.first - b.first) + std::abs(a.second - b.second);
}

using came_from_t = std::unordered_map<Point, Point, PointHash>;

std::vector<Point> reconstruct_path(const Point &start, const Point &goal,
                                    const came_from_t &came_from) {
  std::vector<Point> path;
  Point current = goal;
  while (current != start) {
    path.push_back(current);
    current = came_from.at(current);
  }
  path.push_back(current);
  std::reverse(path.begin(), path.end());
  return path;
}

std::vector<Point> a_star(const Maze &maze) {
  const Point &start = maze.getStart();
  const Point &goal = maze.getGoal();

  using PriorityNode = std::pair<int, Point>;
  std::priority_queue<PriorityNode, std::vector<PriorityNode>,
                      std::greater<PriorityNode>>
      openSet;
  openSet.emplace(0, start);

  came_from_t came_from;

  std::unordered_map<Point, int, PointHash> g_score;
  g_score[start] = 0;

  while (!openSet.empty()) {
    const Point current = openSet.top().second;
    openSet.pop();

    if (current == goal) {
      return reconstruct_path(start, goal, came_from);
    }

    for (const Point &neighbor : maze.neighbors(current)) {
      int new_g_score = g_score[current] + 1;
      if (auto g_score_it = g_score.find(neighbor);
          g_score_it == g_score.end() || new_g_score < g_score_it->second) {
        g_score[neighbor] = new_g_score;
        int f_score = new_g_score + manhattan_distance(neighbor, goal);
        openSet.emplace(f_score, neighbor);
        came_from[neighbor] = current;
      }
    }
  }

  return {};
}

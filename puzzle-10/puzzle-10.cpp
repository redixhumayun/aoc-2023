#include <fstream>
#include <iostream>
#include <queue>
#include <set>
#include <sstream>
#include <vector>

#include "debug.h"

using namespace std;

using Direction = pair<int, int>;

//  these connections are incorrect
//  for example, | should accept connections from the previous row and the next
//  row but currently it accepts it from the left column and the right column
//  check all the others as
unordered_map<char, pair<set<Direction>, set<Direction>>> pipe_connections{
    {'|', {{{-1, 0}, {1, 0}}, {{1, 0}, {-1, 0}}}},
    {'-', {{{0, -1}, {0, 1}}, {{0, 1}, {0, -1}}}},
    {'L', {{{-1, 0}, {0, 1}}, {{0, 1}, {-1, 0}}}},
    {'J', {{{-1, 0}, {0, -1}}, {{0, -1}, {-1, 0}}}},
    {'7', {{{0, -1}, {1, 0}}, {{1, 0}, {0, -1}}}},
    {'F', {{{0, 1}, {1, 0}}, {{1, 0}, {0, 1}}}},
    {'S',
     {{{-1, 0}, {0, -1}, {0, 1}, {1, 0}}, {{0, -1}, {0, 1}, {1, 0}, {-1, 0}}}}};

std::vector<std::vector<char>> maze;
std::vector<std::vector<char>> boundary;

struct bfs_state {
  int x;
  int y;
  int steps;
};

bool is_valid_move(char current_pipe, int dx, int dy, char next_pipe) {
  auto outputs = pipe_connections[current_pipe].second;
  auto inputs = pipe_connections[next_pipe].first;
  return outputs.find({dx, dy}) != outputs.end() &&
         inputs.find({-dx, -dy}) != inputs.end();
}

auto bfs(const pair<int, int>& start) -> vector<vector<int>> {
  int rows = maze.size();
  int cols = maze[0].size();
  vector<vector<int>> steps(rows, vector<int>(cols, -1));
  queue<bfs_state> q;

  q.push({start.first, start.second, 0});
  steps[start.first][start.second] = 0;

  while (!q.empty()) {
    bfs_state current = q.front();
    boundary[current.x][current.y] = '#';
    q.pop();

    vector<vector<int>> directions = {{0, 1}, {0, -1}, {1, 0}, {-1, 0}};
    for (const auto& direction : directions) {
      int dx = direction[0];
      int dy = direction[1];
      int new_x = current.x + dx;
      int new_y = current.y + dy;

      if (new_x < 0 || new_x >= rows || new_y < 0 || new_y >= cols) {
        continue;
      }

      char current_pipe = maze[current.x][current.y];
      char new_pipe = maze[new_x][new_y];

      if (!is_valid_move(current_pipe, dx, dy, new_pipe)) {
        continue;
      }

      if (steps[new_x][new_y] == -1 ||
          current.steps + 1 < steps[new_x][new_y]) {
        steps[new_x][new_y] = current.steps + 1;
        q.push({.x = new_x, .y = new_y, .steps = current.steps + 1});
      }
    }
  }

  //  print out the steps array
  debug_log("The steps array", true, "");
  for (const auto& row : steps) {
    for (const auto& pipe : row) {
      debug_log(" ", false, pipe);
    }
    debug_log("", true, "");
  }

  //  print out the boundary
  debug_log("The boundary", true, "");
  for (const auto& row : boundary) {
    for (const auto& pipe : row) {
      debug_log(" ", false, pipe);
    }
    debug_log("", true, "");
  }

  // Find the furthest tile from the starting point
  int max_steps = 0;
  for (const auto& row : steps) {
    for (int step : row) {
      if (step > max_steps) {
        max_steps = step;
      }
    }
  }

  debug_log("Furthest point from the start is ", false, max_steps,
            " steps away.");
  return steps;
}

vector<vector<int>> expandMaze(const vector<vector<char>>& maze) {
  int rows = maze.size();
  int cols = maze[0].size();
  vector<vector<int>> expandedMaze(rows * 3, vector<int>(cols * 3, 0));

  for (int y = 0; y < rows; ++y) {
    for (int x = 0; x < cols; ++x) {
      int baseRow = y * 3;
      int baseCol = x * 3;
      char tile = maze[y][x];

      // Mark the center as impassable
      expandedMaze[baseRow + 1][baseCol + 1] = 1;

      // For each pipe, open paths in the corresponding directions
      auto links = pipe_connections[tile].first;
      for (auto& dir : links) {
        expandedMaze[baseRow + 1 + dir.first][baseCol + 1 + dir.second] = 1;
      }
    }
  }

  //  print the expanded maze
  debug_log("The expanded maze", true, "");
  for (const auto& row : expandedMaze) {
    for (const auto& pipe : row) {
      debug_log(" ", false, pipe);
    }
    debug_log("", true, "");
  }
  return expandedMaze;
}

void floodFillFromBorder(vector<vector<int>>& maze, int x, int y) {
  if (x < 0 || x >= maze[0].size() || y < 0 || y >= maze.size() ||
      maze[y][x] != 0) {
    return;
  }

  maze[y][x] = 2;  // Mark as visited
  floodFillFromBorder(maze, x + 1, y);
  floodFillFromBorder(maze, x - 1, y);
  floodFillFromBorder(maze, x, y + 1);
  floodFillFromBorder(maze, x, y - 1);
}

void performFloodFillFromBorders(vector<vector<int>>& expandedMaze) {
  int rows = expandedMaze.size();
  int cols = expandedMaze[0].size();

  // Flood fill from the top and bottom of each column
  for (int x = 0; x < cols; ++x) {
    floodFillFromBorder(expandedMaze, x, 0);  // Top edge of each column
    floodFillFromBorder(expandedMaze, x,
                        rows - 1);  // Bottom edge of each column
  }

  // Flood fill from the left and right of each row
  for (int y = 0; y < rows; ++y) {
    floodFillFromBorder(expandedMaze, 0, y);         // Left edge of each row
    floodFillFromBorder(expandedMaze, cols - 1, y);  // Right edge of each row
  }
}

int countEnclosedTiles(const vector<vector<char>>& maze,
                       const vector<vector<int>>& expandedMaze,
                       vector<vector<int>> steps) {
  int count = 0;
  for (int row = 0; row < maze.size(); row++) {
    for (int col = 0; col < maze[0].size(); col++) {
      if (steps[row][col] == -1 && expandedMaze[row * 3][col * 3] != 2) {
        count++;
      }
    }
  }
  return count;
}

int main() {
  std::ifstream input_file("puzzle-10-input.txt");
  if (!input_file) {
    std::cerr << "Error opening file\n";
    return 1;
  }

  std::string line;

  while (getline(input_file, line)) {
    std::istringstream line_stream(line);

    char pipe;
    std::vector<char> pipes;
    while (line_stream >> pipe) {
      pipes.push_back(pipe);
    }
    maze.push_back(pipes);
    boundary.push_back(pipes);
  }

  //  print the maze
  for (const auto& row : maze) {
    for (const auto& pipe : row) {
      debug_log(" ", false, pipe);
    }
    debug_log("", true, "");
  }

  //  find the starting point "S" in the maze
  pair<int, int> starting_point;
  for (int i = 0; i < maze.size(); i++) {
    for (int j = 0; j < maze[i].size(); j++) {
      if (maze[i][j] == 'S') {
        starting_point = {i, j};
        break;
      }
    }
  }

  debug_log("The starting point is ", true, starting_point.first, ",",
            starting_point.second, ".");

  auto steps = bfs(starting_point);

  vector<vector<int>> expandedMaze = expandMaze(maze);
  performFloodFillFromBorders(expandedMaze);

  //  print the expanded maze here after its been flood filled
  debug_log("The expanded maze after flood fill", true, "");
  for (const auto& row : expandedMaze) {
    for (const auto& pipe : row) {
      debug_log(" ", false, pipe);
    }
    debug_log("", true, "");
  }

  int enclosedTiles = countEnclosedTiles(maze, expandedMaze, steps);

  debug_log("Number of tiles enclosed by the loop: ", false, enclosedTiles);
}
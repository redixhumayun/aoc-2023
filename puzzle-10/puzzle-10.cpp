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

struct bfs_state {
  int x;
  int y;
  int steps;
};

bool is_valid_move(char current_pipe, int dx, int dy, char next_pipe) {
  debug_log("Checking if ", true, dx, ",", dy, " is a valid move from pipe ",
            current_pipe, " to pipe ", next_pipe, ".");
  auto outputs = pipe_connections[current_pipe].second;
  debug_log("The outputs are ", true, "");
  for (const auto& output : outputs) {
    debug_log(" ", false, output.first, ",", output.second);
  }
  debug_log("", true, "");
  auto inputs = pipe_connections[next_pipe].first;
  debug_log("The inputs are ", true, "");
  for (const auto& input : inputs) {
    debug_log(" ", false, input.first, ",", input.second);
  }
  debug_log("", true, "");
  return outputs.find({dx, dy}) != outputs.end() &&
         inputs.find({-dx, -dy}) != inputs.end();
}

void bfs(const pair<int, int>& start) {
  int rows = maze.size();
  int cols = maze[0].size();
  vector<vector<int>> steps(rows, vector<int>(cols, -1));
  queue<bfs_state> q;

  q.push({start.first, start.second, 0});
  steps[start.first][start.second] = 0;

  while (!q.empty()) {
    bfs_state current = q.front();
    debug_log("The current state is ", true, current.x, ",", current.y,
              " with ", current.steps, " steps.");
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

      debug_log("Moving to ", true, new_x, ", ", new_y, " with pipe ", new_pipe,
                " from pipe ", current_pipe, ".");

      if (!is_valid_move(current_pipe, dx, dy, new_pipe)) {
        debug_log("Invalid move", true, "");
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
  for (const auto& row : steps) {
    for (const auto& step : row) {
      debug_log(" ", false, step);
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

  bfs(starting_point);
}
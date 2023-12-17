#include <fstream>
#include <iostream>
#include <queue>
#include <sstream>
#include <string>
#include <tuple>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "debug.h"

using namespace std;

const int MAX_MOVES = 10;
const int MIN_MOVES = 4;

struct State {
  int x;
  int y;
  int direction;
  int count;
};

bool operator==(const State& a, const State& b) {
  return tie(a.x, a.y, a.direction, a.count) ==
         tie(b.x, b.y, b.direction, b.count);
}

namespace std {
template <>
struct hash<State> {
  size_t operator()(const State& s) const {
    return hash<int>()(s.x) ^ hash<int>()(s.y) ^ hash<int>()(s.direction) ^
           hash<int>()(s.count);
  }
};

template <>
struct hash<pair<int, int>> {
  size_t operator()(const pair<int, int>& p) const {
    return hash<int>()(p.first) ^ hash<int>()(p.second);
  }
};
}  // namespace std

struct Compare {
  bool operator()(const pair<int, State>& a, const pair<int, State>& b) const {
    return a.first > b.first;
  }
};

bool is_current_state_valid(State current_state, int rows, int cols) {
  return current_state.x >= 0 && current_state.x < rows &&
         current_state.y >= 0 && current_state.y < cols;
}

int find_least_heat_loss_path(const vector<vector<int>>& grid) {
  int rows = grid.size();
  int cols = grid[0].size();
  priority_queue<pair<int, State>, vector<pair<int, State>>, Compare> pq;
  unordered_set<State> visited;
  unordered_map<State, int> cost_map;

  const vector<pair<int, int>> directions = {
      {{0, 1}, {1, 0}, {0, -1}, {-1, 0}}};  // Right, Down, Left, Up

  pq.push({0, {0, 0, -1, -1}});

  while (!pq.empty()) {
    auto [cost, state] = pq.top();
    pq.pop();

    if (visited.find(state) != visited.end()) {
      continue;
    }

    visited.insert(state);
    cost_map[state] = cost;

    for (int i = 0; i < directions.size(); ++i) {
      auto [d_row, d_col] = directions[i];
      int new_row = state.x + d_row;
      int new_col = state.y + d_col;
      int new_direction = i;
      int count = (new_direction != state.direction) ? 1 : state.count + 1;

      bool is_not_same_dir = ((new_direction + 2) % 4) != state.direction;

      // bool is_valid = count <= MAX_MOVES;
      bool is_valid =
          count <= MAX_MOVES && (new_direction == state.direction ||
                                 count >= MIN_MOVES || state.count == -1);

      if (new_row < 0 || new_row >= rows || new_col < 0 || new_col >= cols ||
          !is_not_same_dir || !is_valid) {
        continue;
      }

      int new_cost = grid[new_row][new_col];
      pq.push({cost + new_cost, {new_row, new_col, new_direction, count}});
    }
  }

  int ans = numeric_limits<int>::max();
  for (const auto& [state, v] : cost_map) {
    if (state.x == rows - 1 && state.y == cols - 1) {
      ans = min(ans, v);
    }
  }
  return ans;
}

int main() {
  ifstream input_file("puzzle-17-test.txt");
  if (!input_file) {
    cerr << "Could not open the file!" << endl;
    return 1;
  }
  string line;

  vector<vector<int>> grid;
  while (getline(input_file, line)) {
    vector<int> row;
    int number;

    for (char ch : line) {
      row.push_back(ch - '0');
    }
    grid.push_back(row);
  }

  //  print out the grid
  debug_log("The grid", true, "");
  for (const auto& row : grid) {
    for (const auto& number : row) {
      debug_log(" ", false, number);
    }
    debug_log("", true, "");
  }

  int output = find_least_heat_loss_path(grid);
  debug_log("The least heat loss path is ", true, output);
}
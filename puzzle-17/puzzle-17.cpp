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

const int MAX_MOVES = 3;
const int INF = numeric_limits<int>::max();

struct State {
  int x;
  int y;
  pair<int, int> direction;
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
    return hash<int>()(s.x) ^ hash<int>()(s.y) ^
           hash<int>()(s.direction.first) ^ hash<int>()(s.direction.second) ^
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

unordered_map<pair<int, int>, pair<int, int>> predecessor_map;
unordered_set<pair<int, int>> visited;

struct CompareState {
  unordered_map<State, int>* cost_map;

  CompareState() : cost_map(nullptr) {}

  void set_cost_map(unordered_map<State, int>& map) { cost_map = &map; }

  bool operator()(const State& a, const State& b) const {
    return cost_map->at(a) > cost_map->at(b);
  }
};

bool is_current_state_valid(State current_state, int rows, int cols) {
  return current_state.x >= 0 && current_state.x < rows &&
         current_state.y >= 0 && current_state.y < cols;
}

int find_least_heat_loss_path(const vector<vector<int>>& grid) {
  int rows = grid.size();
  int cols = grid[0].size();
  unordered_map<State, int> cost_map;
  CompareState comp_state;
  comp_state.set_cost_map(cost_map);
  priority_queue<State, vector<State>, CompareState> pq(comp_state);

  const vector<pair<int, int>> directions = {
      {{0, 1}, {1, 0}, {0, -1}, {-1, 0}}};  // Right, Down, Left, Up

  //  initialize the cost map
  for (int x = 0; x < rows; x++) {
    for (int y = 0; y < cols; y++) {
      for (const auto& dir : directions) {
        State state{x, y, dir, 0};
        cost_map[state] = INF;
      }
    }
  }

  //  initialize the cost map and the priority queue
  for (const auto& direction : directions) {
    State start_state{.x = 0, .y = 0, .direction = direction, .count = 0};
    cost_map[start_state] = 0;
    pq.push(start_state);
  }

  while (!pq.empty()) {
    // get the state at the top of the queue
    auto current_state = pq.top();
    pq.pop();

    pair<int, int> current_position = {current_state.x, current_state.y};

    //  check if the current state has been visited
    if (visited.find(current_position) != visited.end()) {
      continue;
    }

    //  found the end state
    if (current_state.x == rows - 1 && current_state.y == cols - 1) {
      return cost_map[current_state];
    }

    //  check if the current state is valid
    if (!is_current_state_valid(current_state, rows, cols)) {
      continue;
    }

    //  find all neighbours and push valid ones onto the stack
    for (const auto& direction : directions) {
      auto new_x = current_state.x + direction.first;
      auto new_y = current_state.y + direction.second;

      if (visited.find({new_x, new_y}) != visited.end()) {
        continue;
      }

      int new_count =
          (direction == current_state.direction) ? current_state.count + 1 : 1;

      if (new_count > MAX_MOVES || new_x < 0 || new_x >= rows || new_y < 0 ||
          new_y >= cols) {
        continue;
      }

      State new_state = {
          .x = new_x,
          .y = new_y,
          .direction = direction,
          .count = new_count,
      };
      if (cost_map.find(new_state) == cost_map.end()) {
        cost_map[new_state] = INF;
      }

      int new_cost = cost_map[current_state] + grid[new_x][new_y];

      if (new_cost < cost_map[new_state]) {
        cost_map[new_state] = new_cost;
        predecessor_map[{new_x, new_y}] =
            pair<int, int>(current_state.x, current_state.y);
        debug_log("Updating predecessor of ", false, new_x, ", ", new_y);
        debug_log(" to ", true, current_state.x, ", ", current_state.y);
        pq.push(new_state);
      }
    }

    visited.insert(current_position);
  }

  debug_log("Couldn't find a path to the bottom right, returning INF", true,
            "");
  return INF;
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

  vector<pair<int, int>> path;
  pair<int, int> current_pair = {grid.size() - 1, grid[0].size() - 1};
  while (current_pair != pair<int, int>{0, 0}) {
    debug_log("Current pair is ", true, current_pair.first, ",",
              current_pair.second);
    path.push_back(current_pair);
    current_pair = predecessor_map[current_pair];
  }
  reverse(path.begin(), path.end());

  debug_log("The path is ", true, "");
  for (const auto& pair : path) {
    debug_log(" ", true, pair.first, ",", pair.second);
    debug_log("The element here is ", true, grid[pair.first][pair.second]);
  }
}
#include <fstream>
#include <iostream>
#include <limits>
#include <queue>
#include <tuple>
#include <unordered_map>
#include <vector>

using namespace std;

struct State {
  int r, c, dir, indir;
  bool operator==(const State& other) const {
    return tie(r, c, dir, indir) ==
           tie(other.r, other.c, other.dir, other.indir);
  }
};

namespace std {
template <>
struct hash<State> {
  size_t operator()(const State& s) const {
    return hash<int>()(s.r) ^ hash<int>()(s.c) ^ hash<int>()(s.dir) ^
           hash<int>()(s.indir);
  }
};
}  // namespace std

// Custom comparator for the priority queue
struct Compare {
  bool operator()(const pair<int, State>& a, const pair<int, State>& b) const {
    return a.first > b.first;
  }
};

int solve(const vector<vector<int>>& G, bool part2) {
  int R = G.size();
  int C = G[0].size();
  priority_queue<pair<int, State>, vector<pair<int, State>>, Compare> Q;
  unordered_map<State, int> D;

  Q.push({0, {0, 0, -1, -1}});

  while (!Q.empty()) {
    auto [dist, state] = Q.top();
    Q.pop();

    if (D.find(state) != D.end()) {
      continue;
    }

    D[state] = dist;

    static const vector<pair<int, int>> directions = {
        {-1, 0}, {0, 1}, {1, 0}, {0, -1}};
    for (int i = 0; i < directions.size(); ++i) {
      auto [dr, dc] = directions[i];
      int rr = state.r + dr;
      int cc = state.c + dc;
      int new_dir = i;
      int new_indir = (new_dir != state.dir) ? 1 : state.indir + 1;

      bool isnt_reverse = ((new_dir + 2) % 4 != state.dir);
      bool isvalid_part1 = (new_indir <= 3);
      bool isvalid_part2 =
          (new_indir <= 10 &&
           (new_dir == state.dir || state.indir >= 4 || state.indir == -1));
      bool isvalid = part2 ? isvalid_part2 : isvalid_part1;

      if (0 <= rr && rr < R && 0 <= cc && cc < C && isnt_reverse && isvalid) {
        int cost = G[rr][cc];
        Q.push({dist + cost, {rr, cc, new_dir, new_indir}});
      }
    }
  }

  int ans = numeric_limits<int>::max();
  for (const auto& [state, v] : D) {
    if (state.r == R - 1 && state.c == C - 1) {
      ans = min(ans, v);
    }
  }

  return ans;
}

int main(int argc, char* argv[]) {
  if (argc < 2) {
    cerr << "Usage: " << argv[0] << " <filename>" << endl;
    return 1;
  }

  ifstream file(argv[1]);
  if (!file) {
    cerr << "Could not open file " << argv[1] << endl;
    return 1;
  }

  string line;
  vector<vector<int>> G;

  while (getline(file, line)) {
    vector<int> row;
    for (char c : line) {
      row.push_back(c - '0');
    }
    G.push_back(row);
  }

  // cout << solve(G, false) << endl;
  cout << solve(G, true) << endl;

  return 0;
}

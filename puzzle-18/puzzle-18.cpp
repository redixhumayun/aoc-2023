#include <fstream>
#include <iostream>
#include <map>
#include <queue>
#include <set>
#include <sstream>

#include "debug.h"

using namespace std;

enum Direction { UP, DOWN, LEFT, RIGHT };

std::map<char, Direction> direction_map = {
    {'U', UP}, {'D', DOWN}, {'L', LEFT}, {'R', RIGHT}};

struct Instruction {
  Direction direction;
  int distance;
};

long long calculate_area_using_shoelace_formula(
    const std::vector<std::pair<long long, long long>>& vertices) {
  long long area = 0;
  int n = vertices.size();
  for (int i = 0; i < n; i++) {
    int j = (i + 1) % n;
    area += vertices[i].first * vertices[j].second;
    area -= vertices[i].second * vertices[j].first;
  }
  return std::abs(area / 2);
}

int main() {
  ifstream input_file("puzzle-18-input.txt");
  if (!input_file) {
    cerr << "Could not open the file!" << endl;
    return 1;
  }

  std::vector<Instruction> instructions;

  std::string line;
  while (getline(input_file, line)) {
    std::istringstream line_stream(line);
    char direction;
    long long distance;
    std::string color;

    line_stream >> direction >> distance >> color;
    Instruction instruction;

    // switch (direction) {
    //   case 'U':
    //     instruction.direction = UP;
    //     break;
    //   case 'D':
    //     instruction.direction = DOWN;
    //     break;
    //   case 'L':
    //     instruction.direction = LEFT;
    //     break;
    //   case 'R':
    //     instruction.direction = RIGHT;
    //     break;
    // }
    char new_direction = color[color.size() - 2];
    switch (new_direction) {
      case '0':
        instruction.direction = RIGHT;
        break;
      case '1':
        instruction.direction = DOWN;
        break;
      case '2':
        instruction.direction = LEFT;
        break;
      case '3':
        instruction.direction = UP;
        break;
    }
    distance = std::stoll(color.substr(2, 5), nullptr, 16);
    instruction.distance = distance;
    instructions.push_back(instruction);
  }

  //  print out instructions
  debug_log("The instructions", true, "");
  for (const auto& instruction : instructions) {
    debug_log("", true, instruction.direction, " ", instruction.distance);
  }

  long long x, y = 0;  //  initial coordinates
  std::vector<std::pair<long long, long long>> vertices;
  vertices.push_back({x, y});

  long long perimeter_count = 0;
  for (const auto& instruction : instructions) {
    switch (instruction.direction) {
      case UP:
        y -= instruction.distance;
        break;
      case DOWN:
        y += instruction.distance;
        break;
      case LEFT:
        x -= instruction.distance;
        break;
      case RIGHT:
        x += instruction.distance;
        break;
    }
    perimeter_count += instruction.distance;
    vertices.push_back({x, y});
  }

  debug_log("The perimeter count is ", true, perimeter_count);
  long long area = calculate_area_using_shoelace_formula(vertices);
  debug_log("The internal area is ", true, area);
  long long ans = area + perimeter_count / 2 + 1;
  debug_log("The area is ", true, ans);
}
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

void trace_path(std::vector<std::vector<bool>>* grid,
                std::vector<Instruction> instructions, int x, int y) {
  for (const auto& instruction : instructions) {
    for (int i = 0; i < instruction.distance; i++) {
      grid->at(y).at(x) = true;
      switch (instruction.direction) {
        case UP:
          y -= 1;
          break;
        case DOWN:
          y += 1;
          break;
        case LEFT:
          x -= 1;
          break;
        case RIGHT:
          x += 1;
          break;
      }
    }
  }
}

int calculate_outline_area(std::vector<std::vector<bool>>* grid) {
  int area = 0;
  for (int i = 0; i < grid->size(); i++) {
    for (int j = 0; j < grid->at(i).size(); j++) {
      if (grid->at(i).at(j) == 1) {
        area++;
      }
    }
  }
  return area;
}

int calculate_interior_area(std::vector<std::vector<bool>>* grid) {
  int area = 0;
  for (int i = 0; i < grid->size(); i++) {
    for (int j = 0; j < grid->at(i).size(); j++) {
      if (grid->at(i).at(j) == 0) {
        area++;
      }
    }
  }
  return area;
}

void flood_fill_grid(std::vector<std::vector<bool>>* grid, int x, int y) {
  int height = grid->size();
  int width = grid->at(0).size();
  std::queue<pair<int, int>> q;
  q.push({x, y});
  grid->at(y).at(x) = true;

  std::vector<std::pair<int, int>> directions = {
      {0, 1}, {0, -1}, {1, 0}, {-1, 0}};

  while (!q.empty()) {
    pair<int, int> pair = q.front();
    q.pop();

    for (const auto& [dx, dy] : directions) {
      int new_x = pair.first + dx;
      int new_y = pair.second + dy;

      if (new_x < 0 || new_x >= width || new_y < 0 || new_y >= height) {
        continue;
      }

      if (grid->at(new_y).at(new_x) == 1) {
        continue;
      }

      grid->at(new_y).at(new_x) = true;
      q.push({new_x, new_y});
    }
  }
}

void flood_fill_boundary(std::vector<std::vector<bool>>* grid) {
  int height = grid->size();
  int width = grid->at(0).size();

  for (int x = 0; x < width; x++) {
    if (!grid->at(0).at(x)) flood_fill_grid(grid, x, 0);
    if (!grid->at(height - 1).at(x)) flood_fill_grid(grid, x, height - 1);
  }

  for (int y = 0; y < height; y++) {
    if (!grid->at(y).at(0)) flood_fill_grid(grid, 0, y);
    if (!grid->at(y).at(width - 1)) flood_fill_grid(grid, width - 1, y);
  }
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
    int distance;
    std::string color;

    line_stream >> direction >> distance >> color;
    Instruction instruction;

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
    distance = std::stoi(color.substr(2, 6), nullptr, 16);
    instruction.distance = distance;
    instructions.push_back(instruction);
  }

  //  print out instructions
  debug_log("The instructions", true, "");
  for (const auto& instruction : instructions) {
    debug_log("", true, instruction.direction, " ", instruction.distance);
  }

  int min_x, min_y, max_x, max_y = 0;
  int x, y = 0;
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
    min_x = std::min(x, min_x);
    min_y = std::min(y, min_y);
    max_x = std::max(x, max_x);
    max_y = std::max(y, max_y);
  }

  int width = max_x - min_x + 1;   //  adding the additional 1 to allow the
                                   //  flood fill to actually have effect
  int height = max_y - min_y + 1;  //  adding the additional 1 to allow the
                                   //  flood fill to actually have effect

  std::vector<std::vector<bool>> grid(height, std::vector<bool>(width, false));
  trace_path(&grid, instructions, -min_x, -min_y);

  //  print out the grid
  debug_log("The grid", true, "");
  for (const auto& row : grid) {
    for (const auto& number : row) {
      debug_log(" ", false, number);
    }
    debug_log("", true, "");
  }

  //  print out the min and max values of x and y
  debug_log("The min and max values of x and y", true, "");
  debug_log("", true, "min_x: ", min_x, " max_x: ", max_x);
  debug_log("", true, "min_y: ", min_y, " max_y: ", max_y);

  int trace_area = calculate_outline_area(&grid);
  debug_log("The trace area is ", true, trace_area);

  flood_fill_boundary(&grid);
  debug_log("The grid", true, "");
  for (const auto& row : grid) {
    for (const auto& number : row) {
      debug_log(" ", false, number);
    }
    debug_log("", true, "");
  }
  int interior_area = calculate_interior_area(&grid);
  debug_log("The interior area is ", true, interior_area);

  int total_area = trace_area + interior_area;
  debug_log("The total area is ", false, total_area);
}
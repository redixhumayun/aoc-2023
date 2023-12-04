//
// Created by Zaid Humayun on 04/12/23.
//
#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <vector>

#include "debug.h"

//  directions: up, down, left, right, diagonal up-left, diagonal up-right,
//  diagonal down-left, diagonal down-right
std::pair<int, int> directions[] = {{0, -1},  {0, 1},  {-1, 0}, {1, 0},
                                    {-1, -1}, {-1, 1}, {1, -1}, {1, 1}};
std::map<std::pair<int, int>, bool> visited;

bool is_gear(char c) { return c == '*'; }

bool is_special_char(char c) {
  return c == '@' || c == '#' || c == '$' || c == '%' || c == '^' || c == '&' ||
         c == '*' || c == '(' || c == ')' || c == '-' || c == '+' || c == '=' ||
         c == '/' || c == '\\' || c == '|' || c == '~' || c == '`' ||
         c == '<' || c == '>' || c == '?' || c == '!' || c == ',' || c == ';' ||
         c == ':' || c == '\'' || c == '\"' || c == '[' || c == ']' ||
         c == '{' || c == '}';
}

bool is_digit(char c) { return c >= '0' && c <= '9'; }

std::vector<int> get_numbers_around_gear(int row, int col, int row_size,
                                         int col_size,
                                         std::vector<std::string>& lines) {
  std::vector<int> numbers{};
  for (const auto& direction : directions) {
    int new_row = row + direction.first;
    int new_col = col + direction.second;

    if (new_row < 0 || new_row >= row_size || new_col < 0 ||
        new_col >= col_size) {
      continue;
    }

    if (is_digit(lines[new_row][new_col])) {
      int left_col = new_col;
      int right_col = new_col;

      while (left_col >= 0 && lines[new_row][left_col] != '.' &&
             !is_special_char(lines[new_row][left_col])) {
        left_col--;
      }
      while (right_col < col_size && lines[new_row][right_col] != '.' &&
             !is_special_char(lines[new_row][right_col])) {
        right_col++;
      }

      left_col += 1;   //  this is the first digit
      right_col -= 1;  //  this is the last digit

      //  check if this number has been visited before
      if (visited[{new_row, left_col}]) {
        continue;
      }

      //  add this number as visited
      visited[{new_row, left_col}] = true;

      std::string num_string =
          lines[new_row].substr(left_col, right_col - left_col + 1);

      int number = std::stoi(num_string);
      numbers.push_back(number);
    }
  }
  return numbers;
}

int main() {
  std::ifstream input_file("puzzle-3-input.txt");
  std::string line;

  if (!input_file) {
    std::cerr << "Error opening file\n";
    return 1;
  }

  std::vector<std::string> lines;

  while (std::getline(input_file, line)) {
    lines.push_back(line);
  }

  int sum = 0;
  for (int row = 0; row < lines.size(); row++) {
    debug_log("Row ", true, row);
    for (int col = 0; col < lines[row].size(); col++) {
      debug_log("", false, lines[row][col], " ");
      if (is_gear(lines[row][col])) {
        auto numbers = get_numbers_around_gear(row, col, lines.size(),
                                               lines[row].size(), lines);

        //  print out the numbers
        for (auto& number : numbers) {
          debug_log("Number ", false, number, " ");
        }
        if (numbers.size() == 2) {
          int prod = numbers[0] * numbers[1];
          sum += prod;
        }
      }
    }
    debug_log("", true, "");
  }
  std::cout << "Sum " << sum << '\n';
}
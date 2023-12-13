#include <algorithm>
#include <fstream>
#include <iostream>
#include <numeric>
#include <sstream>
#include <unordered_map>
#include <utility>
#include <vector>

#include "debug.h"

std::vector<char> instructions;
std::vector<std::string> starting_keys;
std::unordered_map<std::string, std::pair<std::string, std::string>> map;

long long calculate_lcm(const std::vector<int>& cycle_lengths) {
  if (cycle_lengths.empty()) {
    return 0;
  }

  long long lcm = cycle_lengths[0];
  for (int i = 1; i < cycle_lengths.size(); ++i) {
    long long gcd = std::gcd(lcm, static_cast<long long>(cycle_lengths[i]));
    lcm = lcm / gcd *
          cycle_lengths[i];  // Rearranged to reduce the chance of overflow
  }

  return std::abs(lcm);  // Ensure the LCM is positive
}

int calculate_cycle_length(std::string current_node) {
  int cycle_length = 0;
  int instructions_size = instructions.size();
  while (current_node.back() != 'Z') {
    int instruction_to_use = instructions[cycle_length % instructions_size];
    const auto& pair = map.find(current_node);
    if (instruction_to_use == 'L') {
      current_node = pair->second.first;
    } else {
      current_node = pair->second.second;
    }
    cycle_length++;
  }
  return cycle_length;
}

int main() {
  std::ifstream input_file("puzzle-8-input.txt");

  if (!input_file) {
    std::cerr << "Error opening file\n";
    return 1;
  }

  std::string line;

  // Read the first line for instructions
  if (getline(input_file, line)) {
    std::istringstream line_stream(line);
    char instruction;
    while (line_stream >> instruction) {
      instructions.push_back(instruction);
    }
  }

  // Read the remaining lines for node mappings
  while (getline(input_file, line)) {
    if (line.empty()) {
      continue;
    }

    std::istringstream line_stream(line);
    std::string node, left, right, equals, temp;

    // Parse the line
    if (line_stream >> node >> equals) {
      // Read left and right nodes
      getline(line_stream, temp, '(');
      getline(line_stream, left, ',');
      getline(line_stream, right, ')');

      // Trim any whitespace
      left.erase(remove(left.begin(), left.end(), ' '), left.end());
      right.erase(remove(right.begin(), right.end(), ' '), right.end());

      // Add to the map
      map[node] = {left, right};

      if (node.back() == 'A') {
        starting_keys.push_back(node);
      }
    }
  }

  //  print out instructions
  debug_log("Instructions ", true, " ");
  for (const auto& instruction : instructions) {
    debug_log("", false, instruction);
  }
  debug_log("\n", false, "");

  //  print out map
  for (auto& pair : map) {
    debug_log("node ", true, pair.first, " left ", pair.second.first, " right ",
              pair.second.second);
  }

  std::vector<int> cycle_lengths;
  for (const auto& starting_key : starting_keys) {
    int cycle_length = calculate_cycle_length(starting_key);
    debug_log("Cycle length ", true, cycle_length);
    cycle_lengths.push_back(cycle_length);
  }

  //  print the cycle lengths
  debug_log("Cycle lengths ", true, " ");
  for (const auto& cycle_length : cycle_lengths) {
    debug_log(" ", false, cycle_length);
  }
  debug_log("\n", false, "");

  long long lcm = calculate_lcm(cycle_lengths);
  debug_log("LCM ", true, lcm);
  return 0;
}

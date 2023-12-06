#include <fstream>
#include <iostream>
#include <limits>
#include <map>
#include <sstream>
#include <string>
#include <vector>

#include "debug.h"

using MappingType = std::map<int, std::vector<std::vector<long>>>;

auto recurse(MappingType& mappings, int index, long value) -> long {
  auto it = mappings.find(index);
  //  base case
  if (it == mappings.end()) {
    return value;
  }
  auto mapping = it->second;
  for (const auto& map : mapping) {
    if (value >= map[0] && value < map[0] + map[1]) {
      // debug_log("Map ", true, map[0], " ", map[1], " ", map[2], " ");
      long diff = value - map[0];
      // debug_log("Diff ", true, diff);
      long computed_value = map[2] + diff;
      // debug_log("Computed value ", true, computed_value);
      return recurse(mappings, ++index, computed_value);
    }
  }
  // debug_log("Value ", true, value);
  return recurse(mappings, ++index, value);
}

std::vector<std::string> split(const std::string& str) {
  std::istringstream iss(str);
  std::vector<std::string> tokens;
  std::string token;
  while (iss >> token) {
    tokens.push_back(token);
  }
  return tokens;
}

int main() {
  std::ifstream file("puzzle-5-input.txt");
  std::string line;

  if (!file) {
    std::cerr << "Error opening file\n";
    return 1;
  }

  MappingType mappings;
  std::vector<long> seed_ranges;
  int currentCategory = -1;

  while (getline(file, line)) {
    // Skip empty lines
    if (line.empty()) continue;

    // Get the seeds
    if (line.find("seeds:") != std::string::npos) {
      std::istringstream line_stream(line);
      std::string temp;
      long seed;

      // Skip the word "seeds:"
      line_stream >> temp;

      // Extract each seed number
      while (line_stream >> seed) {
        seed_ranges.push_back(seed);
      }
    }

    //  print the seeds
    debug_log("Seeds ", true);
    for (const auto& seed : seed_ranges) {
      debug_log("", false, seed, " ");
    }
    debug_log("", true, "");

    // Check if the line is a header (contains "map:")
    if (line.find("map:") != std::string::npos) {
      currentCategory++;  // Move to the next category
      continue;
    }

    // Parse the mapping line
    auto tokens = split(line);
    std::cout << "Line " << line << '\n';
    if (tokens.size() == 3) {
      long destStart = std::stoll(tokens[0]);
      long srcStart = std::stoll(tokens[1]);
      long length = std::stoll(tokens[2]);
      mappings[currentCategory].push_back({srcStart, length, destStart});
    }
  }

  // Output the parsed mappings (for demonstration purposes)
  for (const auto& category : mappings) {
    std::cout << "Category " << category.first << ":\n";
    for (const auto& mapping : category.second) {
      std::cout << "  Source Start: " << mapping[0]
                << ", Length: " << mapping[1]
                << ", Destination Start: " << mapping[2] << "\n";
    }
  }

  long min_value = std::numeric_limits<long>::max();
  for (const auto& seed : seed_ranges) {
    long seed_value = recurse(mappings, 0, seed);
    debug_log("Seed ", true, seed, " -> ", seed_value);
    if (seed_value < min_value) {
      min_value = seed_value;
    }
  }

  debug_log("Final answer ", true, min_value);

  return 0;
}

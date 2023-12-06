#include <fstream>
#include <iostream>
#include <limits>
#include <map>
#include <sstream>
#include <string>
#include <vector>

#include "debug.h"

using MappingType = std::map<int, std::vector<std::vector<long>>>;

long iterateMappings(const MappingType& mappings, long seed) {
  long value = seed;
  for (int index = 0; index < mappings.size(); ++index) {
    auto it = mappings.find(index);
    if (it == mappings.end()) {
      // If no mapping is found for the current index, continue to the next
      // index
      continue;
    }

    const auto& mapping = it->second;
    bool matched = false;

    for (const auto& map : mapping) {
      if (value >= map[0] && value < map[0] + map[1]) {
        long diff = value - map[0];
        value = map[2] + diff;
        matched = true;
        break;  // Break out of the inner loop once a match is found
      }
    }

    // If no match was found in this mapping, continue to the next mapping with
    // the current value
  }

  return value;  // Return the final computed value
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
  std::vector<long> seeds;
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

    // Check if the line is a header (contains "map:")
    if (line.find("map:") != std::string::npos) {
      currentCategory++;  // Move to the next category
      continue;
    }

    // Parse the mapping line
    auto tokens = split(line);
    if (tokens.size() == 3) {
      long destStart = std::stoll(tokens[0]);
      long srcStart = std::stoll(tokens[1]);
      long length = std::stoll(tokens[2]);
      mappings[currentCategory].push_back({srcStart, length, destStart});
    }
  }

  //  build the actual seed values from the seed ranges
  for (long i = 0; i < seed_ranges.size() - 1; i += 2) {
    long start_seed = seed_ranges[i];
    long range = seed_ranges[i + 1];
    debug_log("Build the seeds from ", true, start_seed, " to ",
              start_seed + range);
    for (long j = start_seed; j <= start_seed + range; j++) {
      seeds.push_back(j);
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
  for (const auto& seed : seeds) {
    long seed_value = iterateMappings(mappings, seed);
    // debug_log("Seed ", true, seed, " -> ", seed_value);
    if (seed_value < min_value) {
      min_value = seed_value;
    }
  }

  debug_log("Final answer ", true, min_value);

  return 0;
}

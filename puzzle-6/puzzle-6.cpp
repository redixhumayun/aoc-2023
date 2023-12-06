#include <fstream>
#include <iostream>
#include <numeric>
#include <sstream>
#include <vector>

#include "debug.h"

int main() {
  std::ifstream file("puzzle-6-input.txt");
  std::string line;

  if (!file) {
    std::cerr << "Error opening file\n";
    return 1;
  }

  std::vector<std::string> time_str;
  std::vector<std::string> distance_str;

  while (getline(file, line)) {
    if (line.find("Time:") != std::string::npos) {
      std::istringstream line_stream(line);

      std::string temp;
      line_stream >> temp;  //  skip the word "Time:"

      std::string time_instance;
      while (line_stream >> time_instance) {
        time_str.push_back(time_instance);
      }
    } else if (line.find("Distance:") != std::string::npos) {
      std::istringstream line_stream(line);

      std::string temp;
      line_stream >> temp;  // skip the word "Distance:"

      std::string distance_instance;
      while (line_stream >> distance_instance) {
        distance_str.push_back(distance_instance);
      }
    }
  }

  std::string concatenated_time =
      std::accumulate(time_str.begin(), time_str.end(), std::string(""));

  std::string concatenated_distance = std::accumulate(
      distance_str.begin(), distance_str.end(), std::string(""));

  long time = std::stol(concatenated_time);
  long distance = std::stol(concatenated_distance);

  debug_log("Time: ", true, time);

  debug_log("Distance: ", true, distance);

  int total_ways = 0;
  for (long curr_time = 0; curr_time <= time; curr_time++) {
    long speed = 0;
    long time_left = time - curr_time;
    speed += curr_time * 1;
    long distance_covered = speed * time_left;
    if (distance_covered > distance) {
      total_ways++;
    }
  }
  debug_log("Total ways ", true, total_ways);
}
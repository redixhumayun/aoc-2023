#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

#include "debug.h"

std::vector<std::vector<long long>> sequences;

bool check_if_all_zeroes(std::vector<long long> vector) {
  bool flag = true;
  for (const auto& number : vector) {
    if (number != 0) {
      flag = false;
      break;
    }
  }
  return flag;
}

long long find_next_in_sequence(const std::vector<long long>& sequence) {
  std::vector<std::vector<long long>> sequences;
  sequences.push_back(sequence);

  // Function to generate a difference sequence
  auto generate_diff_sequence = [](const std::vector<long long>& seq) {
    std::vector<long long> diff;
    for (size_t i = 0; i < seq.size() - 1; ++i) {
      diff.push_back(seq[i + 1] - seq[i]);
    }
    return diff;
  };

  // Generate difference sequences until all elements are zero
  sequences.push_back(generate_diff_sequence(sequence));
  while (!std::all_of(sequences.back().begin(), sequences.back().end(),
                      [](long long i) { return i == 0; })) {
    sequences.push_back(generate_diff_sequence(sequences.back()));
  }

  long long next_value = sequences.back().front();
  for (int i = sequences.size() - 1; i >= 0; i--) {
    next_value = sequences[i].front() - next_value;
  }

  return next_value;
}

int main() {
  std::ifstream input_file("puzzle-9-input.txt");
  if (!input_file) {
    std::cerr << "Error opening file\n";
    return 1;
  }

  std::string line;

  while (getline(input_file, line)) {
    if (line.empty()) {
      continue;
    }

    std::istringstream line_stream(line);
    long long number;

    std::vector<long long> temp_sequence;

    while (line_stream >> number) {
      temp_sequence.push_back(number);
    }

    sequences.push_back(temp_sequence);
  }

  // print out the sequences
  for (const auto& sequence : sequences) {
    for (const auto& number : sequence) {
      debug_log(" ", false, number);
    }
    debug_log("", true, "");
  }

  long long sum_of_next_in_sequence = 0;
  for (const auto& sequence : sequences) {
    sum_of_next_in_sequence += find_next_in_sequence(sequence);
  }

  debug_log("Sum of next in sequence: ", false, sum_of_next_in_sequence);
}
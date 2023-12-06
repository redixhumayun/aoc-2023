#include <cmath>
#include <fstream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

#include "debug.h"

struct Card {
  std::vector<int> winning_numbers;
  std::vector<int> my_numbers;
  int copies;
  int matches;
};

auto parse_numbers(const std::string& str) -> std::vector<int> {
  std::vector<int> numbers;
  std::istringstream line_stream(str);
  int num;

  while (line_stream >> num) {
    numbers.push_back(num);
  }
  return numbers;
}

auto append_game_state(std::map<int, Card>* game_state,
                       const std::vector<int>& winning_numbers,
                       const std::vector<int>& my_numbers, int card_id)
    -> void {
  game_state->insert({card_id, {winning_numbers, my_numbers, 1, 0}});
}

auto calculate_game_score(std::map<int, Card>* game_state) -> int {
  int total_score = 0;
  for (auto& [card_id, card] : *game_state) {
    for (auto& my_number : card.my_numbers) {
      for (auto& winning_number : card.winning_numbers) {
        if (my_number == winning_number) {
          card.matches++;
        }
      }
    }
    int curr_index = card_id;
    for (int new_index = curr_index + 1; new_index <= curr_index + card.matches;
         new_index++) {
      auto it = game_state->find(new_index);
      if (it != game_state->end()) {
        it->second.copies += card.copies;
      }
    }
  }
  for (auto& [card_id, card] : *game_state) {
    total_score += card.copies;
  }
  return total_score;
}

int main() {
  std::ifstream input_file("puzzle-4-input.txt");
  std::string line;

  if (!input_file) {
    std::cerr << "Error opening file\n";
    return 1;
  }

  std::unique_ptr<std::map<int, Card>> game_state =
      std::make_unique<std::map<int, Card>>();

  while (getline(input_file, line)) {
    debug_log("Line ", true, line);

    std::istringstream line_stream(line);
    std::string part;

    getline(line_stream, part, ' ');
    getline(line_stream, part, ':');
    int card_id = std::stoi(part);

    size_t delimiter_pos = line.find('|');
    size_t start_pos = line.find(':');
    std::string winning_numbers_str =
        line.substr(start_pos + 1, delimiter_pos - start_pos - 1);
    std::string my_numbers_str = line.substr(delimiter_pos + 1);

    std::vector<int> winning_numbers = parse_numbers(winning_numbers_str);
    std::vector<int> my_numbers = parse_numbers(my_numbers_str);
    append_game_state(game_state.get(), winning_numbers, my_numbers, card_id);
  }

  input_file.close();

  int total_score = calculate_game_score(game_state.get());
  debug_log("Total score ", true, total_score);
}
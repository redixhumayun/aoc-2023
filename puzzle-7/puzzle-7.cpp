#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>

#include "debug.h"

struct Hand {
  std::string cards;
  int bid;
  int score;
};

const std::unordered_map<char, int> card_precedence{
    {'A', 14}, {'K', 13}, {'Q', 12}, {'T', 11}, {'9', 10}, {'8', 9}, {'7', 8},
    {'6', 7},  {'5', 6},  {'4', 5},  {'3', 4},  {'2', 3},  {'J', 2}};

const std::unordered_map<int, int> hand_scores = {
    {5, 12}, {4, 10}, {3, 5}, {2, 2}, {1, 0}};

auto sort_cards(const Hand& a, const Hand& b) -> bool {
  if (a.score != b.score) {
    return a.score < b.score;
  }

  //  compare the cards character by character
  for (int i = 0; i < a.cards.size(); i++) {
    const char& card_1 = a.cards[i];
    const char& card_2 = b.cards[i];
    if (card_precedence.at(card_1) != card_precedence.at(card_2)) {
      return card_precedence.at(card_1) < card_precedence.at(card_2);
    }
  }
  return false;
}

auto decide_score(std::vector<int> counts, int joker_counts) -> int {
  int total_slots = 5;
  int max_count;
  if (counts.empty()) {
    max_count = 0;
  } else {
    max_count = counts[0];
  }
  int remaining_slots = total_slots - max_count;

  int usable_jokers = std::min(joker_counts, remaining_slots);
  max_count += usable_jokers;

  int score = hand_scores.at(max_count);
  if (max_count == 3 && counts[1] == 2) {
    return score + hand_scores.at(counts[1]);
  }

  if (max_count == 2 && counts[1] == 2) {
    return score + hand_scores.at(counts[1]);
  }

  return score;
}

auto calculate_scores(std::vector<Hand>& hands) -> std::vector<Hand> {
  for (auto& hand : hands) {
    std::vector<int> counts;
    std::unordered_map<char, int> card_count{};
    int j_count = 0;
    for (const char& card : hand.cards) {
      if (card != 'J') {
        card_count[card]++;
      } else {
        j_count++;
      }
    }

    debug_log("Hand ", true, hand.cards);
    for (auto& pair : card_count) {
      counts.push_back(pair.second);
    }

    std::sort(counts.begin(), counts.end(), std::greater<int>());
    hand.score = decide_score(counts, j_count);
  }
  return hands;
}

int main() {
  std::ifstream file("puzzle-7-input.txt");

  if (!file) {
    std::cerr << "Error opening file\n";
    return 1;
  }

  std::string line;

  std::vector<Hand> hands;
  while (getline(file, line)) {
    std::istringstream line_stream(line);
    std::string part;

    getline(line_stream, part, ' ');

    std::string cards = part;

    getline(line_stream, part);
    int bid = std::stoi(part);

    auto hand = Hand{cards, bid, 0};
    hands.push_back(hand);
  }

  //  print out hands
  for (auto& hand : hands) {
    debug_log("Cards ", true, hand.cards, " Bid ", hand.bid);
  }

  std::vector<Hand> hands_with_score = calculate_scores(hands);

  std::sort(hands_with_score.begin(), hands_with_score.end(), sort_cards);

  debug_log("*****", true, " ");
  //  print out the list of hands_with_score
  for (auto& hand : hands_with_score) {
    debug_log("Cards ", true, hand.cards, " Bid ", hand.bid, " Score ",
              hand.score);
  }

  //  calculate the final score
  int final_score = 0;
  for (int i = 0; i < hands_with_score.size(); i++) {
    const auto& hand = hands_with_score[i];
    final_score += (i + 1) * hand.bid;
  }
  debug_log("Final score ", true, final_score);
  return 0;
}
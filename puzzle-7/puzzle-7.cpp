#include <fstream>
#include <iostream>
#include <sstream>

#include "debug.h"

struct Hand {
  std::string cards;
  int bid;
  int score;
};

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

  //  calculate the score of each hand
  for (auto& hand : hands) {
    std::vector<int> counts;
    std::unordered_map<char, int> card_count;
    for (const char& card : hand.cards) {
      card_count[card]++;
    }

    for (auto& pair : card_count) {
      counts.push_back(pair.second);
    }

    std::sort(counts.begin(), counts.end(), std::greater<int>());

    if (counts[0] == 5) {
      //  5 of a kind
      hand.score = 7;
    } else if (counts[0] == 4) {
      //  4 of a kind
      hand.score = 6;
    } else if (counts[0] == 3) {
      if (counts[1] == 2) {
        //  full house
        hand.score = 5;
      } else {
        //  trips
        hand.score = 4;
      }
    } else if (counts[0] == 2) {
      if (counts[1] == 2) {
        //  double pair
        hand.score = 3;
      } else {
        //  pair
        hand.score = 2;
      }
    } else {
      //  high card
      hand.score = 1;
    }
  }

  std::unordered_map<char, int> card_precedence{
      {'A', 14}, {'K', 13}, {'Q', 12}, {'J', 11}, {'T', 10}, {'9', 9}, {'8', 8},
      {'7', 7},  {'6', 6},  {'5', 5},  {'4', 4},  {'3', 3},  {'2', 2},
  };

  std::sort(hands.begin(), hands.end(),
            [&card_precedence](const Hand& a, const Hand& b) {
              if (a.score != b.score) {
                return a.score < b.score;
              }

              //  compare the cards character by character
              for (int i = 0; i < a.cards.size(); i++) {
                const char& card_1 = a.cards[i];
                const char& card_2 = b.cards[i];
                if (card_precedence[card_1] != card_precedence[card_2]) {
                  return card_precedence[card_1] < card_precedence[card_2];
                }
              }
              return false;
            });

  debug_log("*****", true, " ");
  //  print out the list of hands
  for (auto& hand : hands) {
    debug_log("Cards ", true, hand.cards, " Bid ", hand.bid, " Score ",
              hand.score);
  }

  //  calculate the final score
  int final_score = 0;
  for (int i = 0; i < hands.size(); i++) {
    const auto& hand = hands[i];
    final_score += (i + 1) * hand.bid;
  }
  debug_log("Final score ", true, final_score);
  return 0;
}
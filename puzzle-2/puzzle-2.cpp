//
// Created by Zaid Humayun on 04/12/23.
//

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <unordered_map>

std::unordered_map<std::string, int> possible = {
        { "red", 12 }, { "green", 13 }, { "blue", 14 }
};

struct Game {
    int game_id{};
    std::vector<std::unordered_map<std::string, int>> sets;
};

template<typename... Args>
void debug_log(const std::string& message, bool new_line, Args&&... args) {
#ifdef DEBUG
    std::ostringstream stream;
    stream << message;
    (stream << ... << std::forward<Args>(args)); // Fold expression (C++17)
    if (new_line) {
        stream << std::endl;
    }
    std::cout << stream.str();
    if (!new_line) {
        std::cout.flush();
    }
#endif
}

int main() {
    std::ifstream inputFile("puzzle-2-input.txt");
    std::string line;

    if (!inputFile) {
        std::cerr << "Error opening file\n";
        return 1;
    }

    std::vector<Game> games;
    while (std::getline(inputFile, line)) {
        debug_log("Line ", true, line);

        std::istringstream line_stream(line);
        std::string part;
        Game game;

        getline(line_stream, part, ' ');
        getline(line_stream, part, ':');
        int game_id = std::stoi(part);
        game.game_id = game_id;

        while (getline(line_stream, part, ';')) {
            std::istringstream set_stream(part);
            std::unordered_map<std::string, int> set;
            std::string colour;
            int count;

            while(set_stream >> count >> colour) {
                if (colour.back() == ',') {
                    colour.pop_back();
                }
                set[colour] += count;
            }
            game.sets.push_back(set);
        }
        games.push_back(game);
    }

    inputFile.close();

    int total_score = 0;
    for (const Game& game : games) {
        debug_log("Game id ", true, game.game_id);
        std::unordered_map<std::string, int> max_set = {
                { "red", 0 }, { "green", 0 }, { "blue", 0 }
        };
        for (const auto& set : game.sets) {
            debug_log("Set ", false, ' ');
            for (const auto& pair : set) {
                debug_log("{ ", false, pair.first, ": ", pair.second, " }, ");
                auto it = max_set.find(pair.first);
                if (it != max_set.end() && pair.second > it->second) {
                    it->second = pair.second;
                }
            }
            debug_log("", true, "");
        }

        int score = 1;
        for (const auto& pair : max_set) {
            debug_log("{ ", false, pair.first, ": ", pair.second, " }, ");
            debug_log("", true, "");    //  new line
            score *= pair.second;
        }
        debug_log("Score ", true, score);
        total_score += score;
    }
    std::cout << "Total: " << total_score << '\n';
    return 0;
}

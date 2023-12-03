//
// Created by Zaid Humayun on 03/12/23.
//

#include <iostream>
#include <fstream>
#include <string>
#include <cctype>
#include <unordered_map>

int getDigits(std::string& string) {
    std::unordered_map<std::string, int> numMap = {
            {"zero", '0'}, {"one", '1'}, {"two", '2'}, {"three", '3'}, {"four", '4'},
            {"five", '5'}, {"six", '6'}, {"seven", '7'}, {"eight", '8'}, {"nine", '9'}
    };

    std::vector<char> number;

    for (int i = 0; i < string.size(); i++) {
        if (std::isdigit(string[i])) {
            number.push_back(string[i]);
            continue;
        }

        for (const auto& pair : numMap) {
            const std::string& numWord = pair.first;
            if (string.substr(i, numWord.length()) == numWord) {
                number.push_back(pair.second);
                break;
            }
        }
    }

    if (number.size() == 1) {
        number.push_back(number.front());
    }

    if (number.empty()) {
        return 0;
    }

    std::string numString = {number.front(), number.back()};
    return std::stoi(numString);
}

int main() {
    std::ifstream inputFile("puzzle-1-input.txt");
    std::string word;

    if (!inputFile) {
        std::cerr << "Error opening file\n";
        return 1;
    }

    int sum = 0;
    while(inputFile >> word) {
        std::cout << "Word " << word << '\n';
        int numericRep = getDigits(word);
        std::cout << "Number " << numericRep << '\n';
        sum += numericRep;
    }

    inputFile.close();
    std::cout << "Sum: " << sum << '\n';
    return 0;
}
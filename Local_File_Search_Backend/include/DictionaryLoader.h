#ifndef DICTIONARYLOADER_H
#define DICTIONARYLOADER_H
#pragma once
#include <unordered_map>
#include <fstream>
#include <regex>
#include <string>
#include <algorithm>

std::unordered_map<std::string, int> loadWordFrequencies(const std::string& filename) {
    std::ifstream file(filename);
    std::unordered_map<std::string, int> frequencies;

    std::string line, word;
    std::regex wordRegex(R"(\w+)");
    while (std::getline(file, line)) {
        auto wordsBegin = std::sregex_iterator(line.begin(), line.end(), wordRegex);
        auto wordsEnd = std::sregex_iterator();
        for (std::sregex_iterator i = wordsBegin; i != wordsEnd; ++i) {
            word = i->str();
            std::transform(word.begin(), word.end(), word.begin(), ::tolower);
            ++frequencies[word];
        }
    }
    return frequencies;
}

std::unordered_map<std::string, int> loadColorFrequencies() {
    std::vector<std::string> colorNames = {
            "black", "white", "red", "green", "blue", "yellow", "cyan", "magenta",
            "orange", "purple", "pink", "gray", "brown", "navy", "teal",
            "lime", "indigo", "maroon", "olive"
    };

    std::unordered_map<std::string, int> colorMap;
    for (const auto& c : colorNames) {
        colorMap[c] = 1;
    }
    return colorMap;
}

#endif //DICTIONARYLOADER_H

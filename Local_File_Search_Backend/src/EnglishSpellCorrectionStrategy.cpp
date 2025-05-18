#include <sstream>
#include <unordered_set>
#include "../include/EnglishSpellCorrectionStrategy.h"

EnglishSpellCorrectionStrategy::EnglishSpellCorrectionStrategy(const std::unordered_map<std::string, int>& dict,
                                                                     const std::unordered_map<std::string, int>& colorDict)
        : dict(dict), colorDict(colorDict) {}

std::unordered_set<std::string> EnglishSpellCorrectionStrategy::edits1(const std::string& word) const {
    std::unordered_set<std::string> edits;
    const std::string letters = "abcdefghijklmnopqrstuvwxyz";

    for (size_t i = 0; i <= word.size(); ++i) {
        std::string L = word.substr(0, i);
        std::string R = word.substr(i);

        if (!R.empty()) edits.insert(L + R.substr(1));
        for (char c : letters) {
            edits.insert(L + c + R);
            if (!R.empty()) edits.insert(L + c + R.substr(1));
        }
        if (R.size() > 1) edits.insert(L + R[1] + R[0] + R.substr(2));
    }

    return edits;
}

std::string EnglishSpellCorrectionStrategy::bestMatch(const std::string& word, const std::unordered_map<std::string, int>& pool) const {
    if (pool.count(word)) return word;

    auto candidatesEdit1 = edits1(word);
    std::string best = word;
    int maxFreq = 0;
    std::set<std::string> candidatesEdit2;

    for (const auto& c : candidatesEdit1) {
        auto it = pool.find(c);
        if (it != pool.end() && it->second > maxFreq) {
            best = it->first;
            maxFreq = it->second;
        }
        auto newCandidates = edits1(c);
        candidatesEdit2.insert(newCandidates.begin(),newCandidates.end());
    }

    for (const auto& c : candidatesEdit2) {
        auto it = pool.find(c);
        if (it != pool.end() && it->second > maxFreq) {
            best = it->first;
            maxFreq = it->second;
        }
    }

    return best;
}

std::unordered_map<std::string, std::vector<std::string>>
EnglishSpellCorrectionStrategy::correct(const std::unordered_map<std::string, std::vector<std::string>>& parsedQuery,
                            std::string& correctedQueryStringOut) const {
    auto correctedQuery = parsedQuery;
    std::ostringstream oss;

    if (correctedQuery.count("path")) {
        oss << "path: ";
        auto& words = correctedQuery["path"];
        for (auto& w : words) {
            oss << w << " ";
        }
    }

    if (correctedQuery.count("content")) {
        oss << "content: ";
        auto& words = correctedQuery["content"];
        for (auto& w : words) {
            w = bestMatch(w, dict);
            oss << w << " ";
        }
    }

    if (correctedQuery.count("color")) {
        oss << "color: ";
        auto& colors = correctedQuery["color"];
        if (!colors.empty()) {
            colors[0] = bestMatch(colors[0], colorDict);
            oss << colors[0];
        }
    }

    correctedQueryStringOut = oss.str();
    return correctedQuery;
}

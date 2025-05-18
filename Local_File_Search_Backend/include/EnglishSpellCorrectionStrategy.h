
#ifndef ENGLISHSPELLCORRECTIONSTRATEGY_H
#define ENGLISHSPELLCORRECTIONSTRATEGY_H
#pragma once
#include "ISpellCorrectionStrategy.h"
#include <unordered_map>
#include <vector>
#include <set>
#include <algorithm>

class EnglishSpellCorrectionStrategy : public ISpellCorrectionStrategy {
public:
    EnglishSpellCorrectionStrategy(const std::unordered_map<std::string, int>& dict,
                       const std::unordered_map<std::string, int>& colorDict);

    std::unordered_map<std::string, std::vector<std::string>>
    correct(const std::unordered_map<std::string, std::vector<std::string>>& parsedQuery,
            std::string& correctedQueryStringOut) const override;

private:
    std::unordered_map<std::string, int> dict;
    std::unordered_map<std::string, int> colorDict;

    std::unordered_set<std::string> edits1(const std::string& word) const;
    std::string bestMatch(const std::string& word, const std::unordered_map<std::string, int>& pool) const;
};
#endif //ENGLISHSPELLCORRECTIONSTRATEGY_H

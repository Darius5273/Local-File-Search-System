#ifndef ISPELLCORRECTIONSTRATEGY_H
#define ISPELLCORRECTIONSTRATEGY_H
#pragma once
#include <unordered_map>
#include <vector>
#include <string>

class ISpellCorrectionStrategy {
public:
    virtual ~ISpellCorrectionStrategy() = default;
    virtual std::unordered_map<std::string, std::vector<std::string>>
    correct(const std::unordered_map<std::string, std::vector<std::string>>& parsedQuery,
            std::string& correctedQueryStringOut) const = 0;
};

#endif //ISPELLCORRECTIONSTRATEGY_H

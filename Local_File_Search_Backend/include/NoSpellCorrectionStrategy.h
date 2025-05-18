
#ifndef NOSPELLCORRECTIONSTRATEGY_H
#define NOSPELLCORRECTIONSTRATEGY_H
#pragma once
#include "ISpellCorrectionStrategy.h"
#include <sstream>

class NoSpellCorrectionStrategy : public ISpellCorrectionStrategy {
public:
    std::unordered_map<std::string, std::vector<std::string>>
    correct(const std::unordered_map<std::string, std::vector<std::string>>& parsedQuery,
            std::string& correctedQueryStringOut) const override {

        return parsedQuery;
    }
};
#endif //NOSPELLCORRECTIONSTRATEGY_H

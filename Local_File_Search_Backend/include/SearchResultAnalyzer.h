#ifndef SEARCHRESULTANALYSER_H
#define SEARCHRESULTANALYSER_H
#pragma once
#include "SearchResult.h"
#include <unordered_map>
#include <vector>
#include <string>
#include <nlohmann/json.hpp>

class SearchResultAnalyzer {
public:
    nlohmann::json analyzeMetadata(const std::vector<SearchResult>& results) const;
};

#endif //SEARCHRESULTANALYSER_H

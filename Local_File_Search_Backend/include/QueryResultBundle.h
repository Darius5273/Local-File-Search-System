#ifndef QUERYRESULTBUNDLE_H
#define QUERYRESULTBUNDLE_H
#pragma once
#include "SearchResult.h"
#include <vector>
#include <string>

struct QueryResultBundle {
    std::vector<std::string> suggestions;
    std::vector<SearchResult> rankingResults;
};

#endif //QUERYRESULTBUNDLE_H

#ifndef ISEARCH_H
#define ISEARCH_H
#pragma once
#include "QueryResultBundle.h"
#include <unordered_map>
#include <string>

class ISearch {
public:
    ISearch() = default;
    virtual ~ISearch() = default;
    virtual QueryResultBundle search(const std::unordered_map<std::string, std::vector<std::string>>& parsedQuery) = 0;
};


#endif //ISEARCH_H

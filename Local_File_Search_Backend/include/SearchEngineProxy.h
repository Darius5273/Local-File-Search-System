
#ifndef SEARCHENGINEPROXY_H
#define SEARCHENGINEPROXY_H
#pragma once
#include "ISearch.h"
#include "SearchEngine.h"
#include "QueryResultBundle.h"
#include "LRUCache.h"

class SearchEngineProxy : public ISearch {
public:
    SearchEngineProxy(SearchEngine& engine, size_t capacity);

    QueryResultBundle search(const std::unordered_map<std::string, std::vector<std::string>>& parsedQuery) override;

private:
    SearchEngine& engine;
    LRUCache<std::string, QueryResultBundle> cache;

    std::string buildKey(const std::unordered_map<std::string, std::vector<std::string>>& parsedQuery);
};

#endif //SEARCHENGINEPROXY_H

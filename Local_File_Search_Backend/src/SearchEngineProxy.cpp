#include "../include/SearchEngineProxy.h"
#include <sstream>
#include <algorithm>

SearchEngineProxy::SearchEngineProxy(SearchEngine& engine, size_t capacity)
        : engine(engine), cache(capacity) {}

QueryResultBundle SearchEngineProxy::search(const std::unordered_map<std::string, std::vector<std::string>>& parsedQuery) {
    std::string key = buildKey(parsedQuery);
    QueryResultBundle result;

    if (cache.get(key, result)) {
        return result;
    }

    result = engine.search(parsedQuery);
    cache.put(key, result);
    return result;
}

std::string SearchEngineProxy::buildKey(const std::unordered_map<std::string, std::vector<std::string>>& parsedQuery) {
    std::ostringstream oss;

    if (parsedQuery.find("path") != parsedQuery.end()) {
        auto paths = parsedQuery.at("path");
        std::sort(paths.begin(), paths.end());
        for (const auto& p : paths) {
            oss << "path=" << p << ";";
        }
    }

    if (parsedQuery.find("content") != parsedQuery.end()) {
        auto contents = parsedQuery.at("content");
        std::sort(contents.begin(), contents.end());
        for (const auto& c : contents) {
            oss << "content=" << c << ";";
        }
    }

    if (parsedQuery.find("color") != parsedQuery.end() && !parsedQuery.at("color").empty()) {
        oss << "color=" << parsedQuery.at("color")[0] << ";";
    }

    return oss.str();
}

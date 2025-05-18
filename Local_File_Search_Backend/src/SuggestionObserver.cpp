#include "../include/SuggestionObserver.h"
#include <sstream>
#include <algorithm>
#include <iostream>


 struct CompareByFrequency {
     std::unordered_map<std::string, int> freqMap;
     CompareByFrequency(std::unordered_map<std::string, int> freqMap) : freqMap(freqMap) {}
     bool operator()(const std::string &a, const std::string &b) {
         return freqMap[a] > freqMap[b];
     }
 };

struct SuggestionHeap {
    std::priority_queue<
            std::string,
            std::vector<std::string>,
            CompareByFrequency> queue;

    SuggestionHeap(const std::unordered_map<std::string, int>& freqMap)
            : queue(CompareByFrequency(freqMap)) {}
};

void SuggestionObserver::update() {
    lastQueryComponent.query = createKey(lastParsedQuery);
    lastQueryComponent.frequency = ++freqMap[lastQueryComponent.query];
    recentKeys.push_back(lastQueryComponent);
    if (recentKeys.size() >= 100) {
        auto oldest = recentKeys.front();
        recentKeys.pop_front();
        freqMap[oldest.query]--;

        if (freqMap[oldest.query] == 0) {
            freqMap.erase(oldest.query);
            removeFromIndexes(oldest.query);
        }
    }


    for (const auto &[key, values]: lastParsedQuery) {
        if (key == "path") {
            indexComponent(values, pathIndex, lastQueryComponent.query);
        } else if (key == "content") {
            indexComponent(values, contentIndex, lastQueryComponent.query);
        } else {
            colorIndex[values.front()].insert(lastQueryComponent.query);
        }
    }
//    printSuggestions();
//    printRecentKeys();
//    printLastQueryComponent();
//    printLastParsedQuery();
//    printFreqMap();
//    printPathIndex();
//    printContentIndex();

}


std::vector<std::string> SuggestionObserver::getSuggestions(const std::unordered_map<std::string, std::vector<std::string>>& parsedQuery) {
    lastParsedQuery = parsedQuery;
    std::vector<std::string> paths;
    if (parsedQuery.find("path") != parsedQuery.end())
        paths = parsedQuery.at("path");
    std::vector<std::string> contents;
    if (parsedQuery.find("content") != parsedQuery.end())
        contents = parsedQuery.at("content");
    auto color = parsedQuery.find("color") != parsedQuery.end() ? parsedQuery.at("color").front() : "";


    std::set<std::string> candidateKeys;
    collectPrefixMatches(paths, pathIndex, candidateKeys);
    collectPrefixMatches(contents, contentIndex, candidateKeys);
//    std::priority_queue<QueryComponent, std::vector<QueryComponent>> results;
//    collectPrefixMatches(paths, pathIndex, results);
//    collectPrefixMatches(contents, contentIndex, results);
    for (const auto &candidate: colorIndex[color]) {
        candidateKeys.insert(candidate);
    }

    SuggestionHeap results(freqMap);
    for (const auto &candidate: candidateKeys) {
        results.queue.push(candidate);
        if (results.queue.size() > 3) {
            results.queue.pop();
        }
    }
    suggestions.clear();
    while (!results.queue.empty()) {
        auto suggestion = results.queue.top();
        formatSuggestion(suggestion);
        suggestions.push_back(suggestion);
        results.queue.pop();
    }

    std::reverse(suggestions.begin(), suggestions.end());
    return suggestions;
}

void SuggestionObserver::collectPrefixMatches(const std::vector<std::string>& parts,
                                              std::unordered_map<std::string, std::set<std::string>>& index,
                                              std::set<std::string>& candidates) {


    for (const auto &part: parts) {
        for (size_t i = 1; i <= part.size(); ++i) {
            auto it = index.find(part.substr(0, i));
            if (it != index.end()) {
                candidates.insert(it->second.begin(), it->second.end());
            }
        }
    }
}


void SuggestionObserver::formatSuggestion(std::string& suggestion){
    std::replace(suggestion.begin(), suggestion.end(), '|', ' ');
}

void SuggestionObserver::indexComponent(const std::vector<std::string>& parts,
                                      std::unordered_map<std::string, std::set<std::string>>& index,
                                      const std::string& key) {
    for (const auto& part : parts) {
        for (size_t i = 1; i <= part.size(); ++i) {
            index[part.substr(0, i)].insert(key);
        }
    }
}

void SuggestionObserver::removeFromIndexes(const std::string& key) {
    for (auto& [prefix, keys] : pathIndex) keys.erase(key);
    for (auto& [prefix, keys] : contentIndex) keys.erase(key);
    for (auto& [color, keys] : colorIndex) keys.erase(key);
}

std::string SuggestionObserver::createKey(const std::unordered_map<std::string, std::vector<std::string>>& query) {
    std::string key;

    if (auto it = query.find("path"); it != query.end()) {
        std::vector<std::string> sortedPath = it->second;
        std::sort(sortedPath.begin(), sortedPath.end());
        key += "path:";
        for (const auto& p : sortedPath) {
            key += p + "|";
        }
    }

    if (auto it = query.find("content"); it != query.end()) {
        std::vector<std::string> sortedContent = it->second;
        std::sort(sortedContent.begin(), sortedContent.end());
        key += "content:";
        for (const auto& c : sortedContent) {
            key += c + "|";
        }
    }

    if (auto it = query.find("color"); it != query.end()) {
        key += "color:" + it->second.front();
    }

    return key;
}

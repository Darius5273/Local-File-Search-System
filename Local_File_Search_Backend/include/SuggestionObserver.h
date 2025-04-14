#ifndef SUGGESTIONOBSERVER_H
#define SUGGESTIONOBSERVER_H

#include "IObserver.h"
#include <unordered_map>
#include <vector>
#include <string>
#include <ctime>
#include <set>
#include <deque>
#include <queue>
#include <iostream>


class SuggestionObserver : public IObserver {
public:
    struct QueryComponent {
        std::string query;
        int frequency = 0;

        bool operator<(const QueryComponent& other) const {
            return frequency > other.frequency;
        }
    };
    void update() override;
    std::vector<std::string> SuggestionObserver::getSuggestions(const std::unordered_map<std::string, std::vector<std::string>>& parsedQuery);

private:
    std::vector<std::string> suggestions;

    std::unordered_map<std::string, int> freqMap;
    std::unordered_map<std::string, std::set<std::string>> pathIndex;
    std::unordered_map<std::string, std::set<std::string>> contentIndex;
    std::unordered_map<std::string, std::set<std::string>> colorIndex;

    std::deque<QueryComponent> recentKeys;
    QueryComponent lastQueryComponent;
    std::unordered_map<std::string, std::vector<std::string>> lastParsedQuery;

    std::string createKey(const std::unordered_map<std::string, std::vector<std::string>>& query);
    void indexComponent(const std::vector<std::string>& parts,
                                            std::unordered_map<std::string, std::set<std::string>>& index,
                                            const std::string& key);
    void collectPrefixMatches(const std::vector<std::string>& parts,
                                                  std::unordered_map<std::string, std::set<std::string>>& index,
                                                  std::set<std::string>& candidates);
    void removeFromIndexes(const std::string& key);
    void formatSuggestion(std::string& suggestion);


//
//    void printSuggestions() const {
//        std::cout << "Suggestions:" << std::endl;
//        for (const auto& suggestion : suggestions) {
//            std::cout << "- " << suggestion << std::endl;
//        }
//    }
//
//    void printFreqMap() const {
//        std::cout << "Frequency Map:" << std::endl;
//        for (const auto& pair : freqMap) {
//            std::cout << pair.first << ": " << pair.second << std::endl;
//        }
//    }
//
//    void printPathIndex() const {
//        std::cout << "Path Index:" << std::endl;
//        for (const auto& pair : pathIndex) {
//            std::cout << "Path: " << pair.first << std::endl;
//            for (const auto& queryComp : pair.second) {
//                printQueryComponent(queryComp);
//            }
//        }
//    }
//
//    void printContentIndex() const {
//        std::cout << "Content Index:" << std::endl;
//        for (const auto& pair : contentIndex) {
//            std::cout << "Content: " << pair.first << std::endl;
//            for (const auto& queryComp : pair.second) {
//                printQueryComponent(queryComp);
//            }
//        }
//    }
//
//    void printColorIndex() const {
//        std::cout << "Color Index:" << std::endl;
//        for (const auto& pair : colorIndex) {
//            std::cout << "Color: " << pair.first << std::endl;
//            for (const auto& queryComp : pair.second) {
//                printQueryComponent(queryComp);
//            }
//        }
//    }
//
//    void printRecentKeys() const {
//        std::cout << "Recent Keys:" << std::endl;
//        for (const auto& queryComp : recentKeys) {
//            printQueryComponent(queryComp);
//        }
//    }
//
//    void printLastQueryComponent() const {
//        std::cout << "Last Query Component:" << std::endl;
//        printQueryComponent(lastQueryComponent);
//    }
//
//    void printLastParsedQuery() const {
//        std::cout << "Last Parsed Query:" << std::endl;
//        for (const auto& pair : lastParsedQuery) {
//            std::cout << pair.first << " => [";
//            for (const auto& value : pair.second) {
//                std::cout << value << ", ";
//            }
//            std::cout << "]" << std::endl;
//        }
//    }
//
//private:
//    void printQueryComponent(const QueryComponent& queryComp) const {
//        std::cout << "Query: " << queryComp.query << ", Frequency: " << queryComp.frequency
//                  << ", Timestamp: " << std::ctime(&queryComp.timestamp);
//    }
};

#endif // SUGGESTIONOBSERVER_H

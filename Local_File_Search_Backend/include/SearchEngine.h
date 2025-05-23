#ifndef SEARCH_ENGINE_H
#define SEARCH_ENGINE_H

#include <string>
#include <vector>
#include <iostream>
#include "DatabaseConnector.h"
#include "QueryParser.h"
#include "ObserverSubject.h"
#include "../include/SuggestionObserver.h"
#include "../include/RankingObserver.h"
#include "ISearch.h"

class SearchEngine : public ISearch {
private:
    DatabaseConnector* db;
    QueryParser queryParser;
    ObserverSubject observerContext;
    SuggestionObserver* suggestionObserver;
    RankingObserver* rankingObserver;
    bool searchContent;

public:
    SearchEngine(DatabaseConnector* database);

    void setSearchContent(bool flag);

    void update();
    QueryResultBundle search(const std::unordered_map<std::string, std::vector<std::string>>& parsedQuery) override;

private:
    std::vector<std::string> getSuggestions(std::unordered_map<std::string, std::vector<std::string>>& parsedQuery);

    std::vector<SearchResult> getRankingResults(std::unordered_map<std::string, std::vector<std::string>>& parsedQuery);
};

#endif // SEARCH_ENGINE_H
